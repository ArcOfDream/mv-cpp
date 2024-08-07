#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <condition_variable>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include "soloud.h"
#include "graphics/renderer.h"
#include "resource/resource.h"
#include "wrenbind17/wrenbind17.hpp"

#pragma once

namespace wren = wrenbind17;

namespace mv {
class Context : public std::enable_shared_from_this<Context> {

    int window_height;
    int window_width;
    std::string window_title;

    std::unordered_map<std::string, IResource> data;
    unsigned int fps;
    unsigned int target_fps = 60;
    unsigned long min_ticks = 1000/target_fps;
    bool should_close = false;
    unsigned long old_time = SDL_GetTicks();
    unsigned long runtime_fps = 0;
    unsigned long step_time = old_time;
    double delta_time = 0;


    void engine_input();
    void engine_update(double dt);
    void engine_draw();
    void engine_exit();

  protected:
    Renderer &renderer = Renderer::get();
    SoLoud::Soloud soloud;
    SDL_GLContext gl_context;
    SDL_Window *window;

    wren::VM wren_vm;

    std::mutex mutex;
    std::condition_variable cv;
    std::condition_variable cv_main;
    std::thread draw_thread;
    bool draw_ready = false;
    bool draw_complete = false;
    bool draw_running = false;

  public:
    Context();
    ~Context();
    Context(int width, int height, std::string title);
    std::shared_ptr<Renderer> get_renderer();
    void set_target_fps(unsigned int);
    void engine_init();
    void run();
    void stop();
    void main_loop();
    void draw_loop();
    void em_try_exit();

    virtual void register_wren_types(wren::VM&) {}
    virtual void init() {}
    virtual void input(SDL_Event&) {}
    virtual void update(double) {}
    virtual void draw() {}
    virtual void exit() {}

    std::shared_ptr<Context> getptr() {
      return shared_from_this();
    }

    int get_fps() { return fps; }
};

#ifdef __EMSCRIPTEN__
void mv_em_main_loop(void *ctx);
#endif

} // namespace mv
