#include "microvidya.h"

#include "mv/scripting/register_types.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_error.h"
#include "wrenbind17/wrenbind17.hpp"
#include <memory>
#include <stdio.h>

float time_elapsed;

using namespace mv;

namespace wren = wrenbind17;

void MyGame::on_talk_press() {
    soloud.stop(pxt_handle);
    // speech.setText(speak_buffer);
    unsigned int err = pxt.load(speak_buffer);
    if (err == SoLoud::SO_NO_ERROR)
        pxt_handle = soloud.play(pxt, 0.9f, 0.0f);
}

void MyGame::register_wren_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &mygame = module.klass<MyGame>("MyGame");
    mygame.func<&MyGame::add_node>("addNode");
    mygame.func<&MyGame::add_sprite>("addSprite");
    mygame.func<&MyGame::get_root_node>("getRootNode");
    mygame.func<&MyGame::get_kleines_texture>("getKleinesTexture");
}

void MyGame::init() {
    set_target_fps(60);
    kleines_ptr =
        load_texture_from_source("kleines", kleines_png, kleines_png_size);

    renderer.set_camera(&cam);

    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF |
                SoLoud::Soloud::ENABLE_VISUALIZATION,
                SoLoud::Soloud::AUTO, 44100, 1024, 2);

    kleines_root =
        std::make_shared<Sprite>("kleines root", kleines_ptr->getptr());


    for (int i = 0; i < 512; i++) {
        // auto c = kleines_root->add_child<Sprite>("kleines child",
        //                                          kleines_ptr->getptr());
        auto c = std::make_shared<Sprite>("kleines child", kleines_ptr->getptr());

        c->set_pos({(rand() % 1500) - 750, (rand() % 1500) - 750});
        c->set_pos({(rand() % 1500) - 750, (rand() % 1500) - 750});
        c->set_scale({0.5f, 0.5f});
        c->set_color({1,1,1,0.2});

        kleines_root->add_instanced_child(c);
    }
    kleines_child = kleines_root->add_child<Sprite>("kleines child",
                                                    kleines_ptr->getptr());

    kleines_child->set_pos({200, 0});
    kleines_child->set_scale({0.2f, 0.2f});
    kleines_child->set_color({1, 1, 1, 0.2});

    f = {256, 256};
    f.setup_context();
    f.add_font_mem("pixo", Pixolletta8px_ttf, Pixolletta8px_ttf_size);
    f.add_font_mem("silkscreen", slkscr_ttf, slkscr_ttf_size);

    wren_vm.runFromFile("main", "assets/script.wren");

    wren::Method entry = wren_vm.find("main", "Instance").func("entryPoint(_)");
    entry(this);
}

void MyGame::update(double dt) {
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

void MyGame::draw() {
    char str[20];
    snprintf(str, sizeof(str), "fps: %i", get_fps());

    renderer.push_quad({0, 0}, {64, 64}, {0.5, 0.5, 1, 1},
                        kleines_ptr->get_id());
    kleines_root->_draw();

    f.color = {1, 1, 0, 1};
    f.font("pixo");
    fonsSetSize(f.ctx, 16);
    // fonsSetBlur(f.ctx, 2);
    fonsSetAlign(f.ctx, FONS_ALIGN_CENTER);
    fonsDrawText(f.ctx, 0, 0, "methinks this should be ok", nullptr);

    fonsDrawText(f.ctx, 0, 16, str, nullptr);

    fonsDrawDebug(f.ctx, -256, -256);

    /* GUI */
    ImGui::Begin("Kleines");

    ImGui::SeparatorText("Camera");
    ImGui::Text("Position: [%f, %f]", cam.position.x, cam.position.y);
    ImGui::Text("Rotation: %f", cam.rotation);

    ImGui::SeparatorText("Kleines");
    ImGui::SliderFloat("Separation", &separation, -200.0f, 200.0f);

    ImGui::SeparatorText("SoLoud");
    ImGui::InputText("Path", speak_buffer, 128);

    if (ImGui::Button("Play!") && !speak_already_pressed) {
        on_talk_press();
        speak_already_pressed = true;
    } else {
        speak_already_pressed = false;
    }

    ImGui::End();
}

int main(int, char **) {
    printf("The project name is %s\n", PROJECT_NAME);

    auto game = MyGame(640, 480, "microvidya");

    game.engine_init();
    game.run();

    return 0;
}
