#define PROJECT_NAME "microvidya"
#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include "mv/mv.h"
#include "mv/graphics/fontrender.h"
#include "soloud_freeverbfilter.h"
#include "soloud_speech.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <math.h>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#pragma once

using namespace mv;

class MyGame : public Context {
    // soloud engine
    SoLoud::Speech speech;
    SoLoud::PXTone pxt;
    SoLoud::FreeverbFilter verb;

    std::shared_ptr<mv::Texture> kleines_ptr;
    std::shared_ptr<Sprite> kleines_root;
    Sprite *kleines_child = nullptr;

    FontRender f;

    Camera2D cam;
    float time = 0.0f;
    float separation = 0.0f;

    bool speak_already_pressed = false;
    // unsigned int speak_handle = 0;
    unsigned int pxt_handle = 0;
    char speak_buffer[128] = "assets/pantest.ptcop";

    const std::string wren_code = R"( System.print("Hello world!") )";

  public:
    using Context::Context;

    std::shared_ptr<Sprite> get_root_node() { return kleines_root; }
    std::shared_ptr<Texture> get_kleines_texture() { return kleines_ptr->getptr(); }
    void add_node(std::shared_ptr<Node> node) { kleines_root->add_instanced_child(node); }

    void on_talk_press();

    void register_wren_types(wren::VM &vm) override;

    void init() override;

    void update(double dt) override;

    void draw() override;
};