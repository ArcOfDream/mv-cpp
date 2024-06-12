#include "soloud_error.h"
#include "soloud_freeverbfilter.h"
#define PROJECT_NAME "microvidya"
#define WITH_SDL2

#include "mv/mv.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_thread.h"
#include <SDL2/SDL_image.h>
#include <epoxy/gl.h>
#include <stdio.h>
#include <memory>

float time_elapsed;

class MyGame : public mv::Context {
    // soloud engine
    SoLoud::Soloud soloud;
    SoLoud::Speech speech;
    SoLoud::PXTone pxt;
    SoLoud::FreeverbFilter verb;


    mv::Texture kleines;
    mv::Camera2D cam = {};
    float time = 0.0f;
    float separation = 0.0f;

    bool speak_already_pressed = false;
    bool stop_time = true;
    unsigned int speak_handle = 0;
    unsigned int pxt_handle = 0;
    char speak_buffer[128] = "assets/pantest.ptcop";
    size_t speak_size = sizeof(speak_buffer);

  public:
    using mv::Context::Context;

    void on_talk_press() {
        soloud.stop(pxt_handle);
        // speech.setText(speak_buffer);
        unsigned int err = pxt.load(speak_buffer);
        if(err == SoLoud::SO_NO_ERROR) pxt_handle = soloud.play(pxt, 0.5f, 0.0f);
    }

    void init() override {
        kleines = mv::load_texture_from_source("kleines", kleines_png,
                                               kleines_png_size);
        renderer.set_camera(std::shared_ptr<mv::Camera2D>(&cam));

        soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION,
                    SoLoud::Soloud::SDL2, 44100, 1024, 2);

        // verb.setParams(1, 0.5f, 0.9f, 1);
        // pxt.setFilter(0, &verb);

        unsigned int err = pxt.load("assets/crawler.ptcop");
        if(err == SoLoud::SO_NO_ERROR) pxt_handle = soloud.play(pxt, 0.5f);
    }

    void update(double dt) override {
        time += dt*5;
        cam.position.x = sinf(time)*200;
        cam.position.y = cosf(time)*200;
        cam.rotation += 1.0f * dt;

        if (time > 15.0 && stop_time) { 
            // pxt.stop(); 

            stop_time = false;
        }
    }

    void draw() override {
        kleines.bind();
        for (int i = 0; i < 12; i++) {
            renderer.push_quad({-128+(separation*i), -128}, {256, 256}, {1, 1, 1, 1},
                            kleines.get_id());
        }
        
        ImGui::Begin("Kleines");

        ImGui::SeparatorText("Camera");
        ImGui::Text("Position: [%f, %f]", cam.position.x, cam.position.y);
        ImGui::Text("Rotation: %f", cam.rotation);

        ImGui::SeparatorText("Kleines");
        ImGui::SliderFloat("Separation", &separation, -200.0f, 200.0f);

        ImGui::SeparatorText("SoLoud");
        ImGui::InputText("Path", speak_buffer, speak_size);
        
        if ( ImGui::Button("Play!") && !speak_already_pressed ) { 
            on_talk_press();
            speak_already_pressed = true;
        }
        else { speak_already_pressed = false; }

        ImGui::End();
    }
};

int main() {
    printf("The project name is %s\n", PROJECT_NAME);

    MyGame game{1440, 800, PROJECT_NAME};

    game.engine_init();
    game.run();

    return 0;
}
