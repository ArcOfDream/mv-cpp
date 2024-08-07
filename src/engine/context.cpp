#include "mv/context.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "mv/gl.h"
#include "mv/graphics/renderer.h"
#include "mv/scripting/register_types.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_timer.h>
#include <assert.h>
// #include <memory>
#include <mutex>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace mv {

#ifdef __EMSCRIPTEN__
void mv_em_main_loop(void *ctx) {
    Context *c = (Context *)ctx;
    c->em_try_exit();
    c->main_loop();
}
#endif

Context::Context() {
    window_width = 640;
    window_height = 480;
    window_title = "Microvidya Window";
}

Context::Context(int width, int height, std::string title) {
    window_width = width;
    window_height = height;
    window_title = title;
}

Context::~Context() {
    stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // renderer.reset();
    renderer.~Renderer();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Context::engine_init() {
    // #ifdef __unix__
    //     SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
    // #endif
    // SDL init
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    assert(result == 0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    window = SDL_CreateWindow(
        window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    assert(window);

    gl_context = SDL_GL_CreateContext(window);
    assert(gl_context);

// instead of using SDL_Delay, we're going to limit the update rate
// to the screen refresh rate.
// Though not all devices may support adaptive vsync, so we
// go ahead and try to use the vsync
#ifndef __EMSCRIPTEN__
    SDL_GL_MakeCurrent(window, gl_context);
    int swap = SDL_GL_SetSwapInterval(-1);
    if (swap != 0)
        SDL_GL_SetSwapInterval(1);
#endif

    // renderer init
    renderer.init();
    renderer.width = window_width;
    renderer.height = window_height;
    renderer.set_context(gl_context);

    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init();

    // wren init
    register_math_types(wren_vm);
    register_glm_types(wren_vm);
    register_core_types(wren_vm);
    register_resource_types(wren_vm);
    register_node_types(wren_vm);
    register_wren_types(wren_vm); // custom types will be registered here

    // and the rest of init
    init();
}

void Context::run() {
    SDL_GL_MakeCurrent(window, nullptr);
    SDL_ShowWindow(window);

    draw_running = true;
    draw_thread = std::thread(&Context::draw_loop, this);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&mv_em_main_loop, this, 0, true);
#else
    while (true) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (should_close) {
                draw_running = false;
                draw_ready = true;
                cv.notify_one();
                break;
            }
        }
        main_loop();
    }

    draw_thread.join();

    exit();
#endif
}

void Context::main_loop() {
    unsigned long new_time = SDL_GetTicks();
    unsigned long time_since_frame = new_time - old_time;

    delta_time = (double)(new_time - step_time) * 0.001;
    step_time = new_time;

    {
        std::unique_lock<std::mutex> lock(mutex);
        cv_main.wait(lock, [this]() { return !draw_ready || draw_complete; });

        engine_input();
        engine_update(delta_time);

        draw_ready = true;
        draw_complete = false;
    }
    cv.notify_one();

    runtime_fps++;
    // SDL_Delay(min_ticks);

    if (time_since_frame > 1000) {
        old_time = new_time;
        fps = runtime_fps;
        runtime_fps = 0;
    }

#ifdef __EMSCRIPTEN__
    // emscripten_sleep(0);
#endif
}

void Context::draw_loop() {
    // it seems on some platforms this won't make the window show
    SDL_ShowWindow(window);

    if (SDL_GL_MakeCurrent(window, gl_context) != 0) {
        printf("Failed to make OpenGL context current in draw thread: %s\n",
               SDL_GetError());
        return;
    }

    while (draw_running) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this]() { return draw_ready && !draw_complete; });

            if (!draw_running)
                break;

            // draw_running = false;
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            engine_draw();

            draw_complete = true;
        }
        cv_main.notify_one();
        // SDL_Delay(min_ticks);
    }
}

void Context::em_try_exit() {
#ifdef __EMSCRIPTEN__
    std::lock_guard<std::mutex> lock(mutex);
    if (should_close) {
        draw_running = false;
        draw_ready = true;
        cv.notify_one();
        draw_thread.join();
        exit();

        emscripten_cancel_main_loop();
    }
#endif
}

void Context::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        draw_running = false;
        draw_ready = true;
    }
    cv.notify_one();
}

void Context::engine_input() {
    SDL_Event ev;

    while (0 < SDL_PollEvent(&ev)) {
        // passing events to imgui
        ImGui_ImplSDL2_ProcessEvent(&ev);

        switch (ev.type) {
        case SDL_QUIT:
            engine_exit();
            break;
        default:
            input(ev);
            break;
        }
    }
}

void Context::engine_update(double dt) { update(dt); }

void Context::engine_draw() {
    SDL_GetWindowSize(window, &window_width, &window_height);
    renderer.width = window_width;
    renderer.height = window_height;

    renderer.clear_frame();
    renderer.begin_frame();

    draw();

    renderer.end_frame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

void Context::engine_exit() { should_close = true; }

// std::shared_ptr<Renderer> Context::get_renderer() {
//     return renderer.getptr();
// }

void Context::set_target_fps(unsigned int value) {
    target_fps = value;
    min_ticks = 1000 / target_fps;
}
} // namespace mv
