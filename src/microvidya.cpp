#include "SDL_video.h"
#include <X11/X.h>
#define PROJECT_NAME "microvidya"
#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include "mv/mv.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_error.h"
#include "soloud_freeverbfilter.h"
#include "soloud_speech.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <math.h>
#include <memory>
#include <stdio.h>
#include <string>
#include <unordered_map>

float time_elapsed;

using namespace mv;

class MyGame : public Context {
    // soloud engine
    SoLoud::Speech speech;
    SoLoud::PXTone pxt;
    SoLoud::FreeverbFilter verb;

    std::shared_ptr<mv::Texture> kleines_ptr;
    std::unique_ptr<Sprite> kleines_root;
    Sprite *kleines_child = nullptr;

    Camera2D cam;
    float time = 0.0f;
    float separation = 0.0f;

    bool speak_already_pressed = false;
    // unsigned int speak_handle = 0;
    unsigned int pxt_handle = 0;
    char speak_buffer[128] = "assets/pantest.ptcop";
    size_t speak_size = sizeof(speak_buffer);

  public:
    using Context::Context;

    void on_talk_press() {
        soloud.stop(pxt_handle);
        // speech.setText(speak_buffer);
        unsigned int err = pxt.load(speak_buffer);
        if (err == SoLoud::SO_NO_ERROR)
            pxt_handle = soloud.play(pxt, 0.9f, 0.0f);
    }

    void init() override {
        kleines_ptr =
            load_texture_from_source("kleines", kleines_png, kleines_png_size);

        renderer.set_camera(&cam);

        soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF |
                        SoLoud::Soloud::ENABLE_VISUALIZATION,
                    SoLoud::Soloud::AUTO, 44100, 1024, 2);

        kleines_root = std::make_unique<Sprite>("kleines root", kleines_ptr->getptr());
        kleines_child = kleines_root->add_child<Sprite>("kleines child", kleines_ptr->getptr());

        kleines_child->set_pos({200, 0});
        kleines_child->set_scale({0.5f, 0.5f});
    }

    void update(double dt) override {
        time += dt;
        Sprite &k = *kleines_root;

        k.set_angle_degrees(time * 0.01);
        // k.set_scale({sinf(time*0.2), 1});
        k.set_pos({cosf(time * 1.3f) * 200, sinf(time * 0.77f) * 200});
        kleines_child->set_angle_degrees(time * 0.01);
        // cam.position.x = sinf(time*0.25f) * 100;
        // cam.position.y = cosf(time*0.25f) * 100;
        // cam.position = {0, sinf(time*0.5f)*100};
        // cam.rotation = M_PI/4;

        kleines_root->_update(dt);
    }

    void draw() override {
        renderer.push_quad({0, 0}, {64, 64}, {0.5,0.5,1,1}, kleines_ptr->get_id());
        kleines_root->_draw();

        ImGui::Begin("Kleines");

        ImGui::SeparatorText("Camera");
        ImGui::Text("Position: [%f, %f]", cam.position.x, cam.position.y);
        ImGui::Text("Rotation: %f", cam.rotation);

        ImGui::SeparatorText("Kleines");
        ImGui::SliderFloat("Separation", &separation, -200.0f, 200.0f);

        ImGui::SeparatorText("SoLoud");
        ImGui::InputText("Path", speak_buffer, speak_size);

        if (ImGui::Button("Play!") && !speak_already_pressed) {
            on_talk_press();
            speak_already_pressed = true;
        } else {
            speak_already_pressed = false;
        }

        ImGui::End();
    }
};

int main(int, char**)  {
    printf("The project name is %s\n", PROJECT_NAME);

    auto game = std::make_shared<MyGame>(320, 240, "microvidya");

    game->engine_init();
    game->run();

    return 0;
}
