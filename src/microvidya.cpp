#define PROJECT_NAME "microvidya"
#define GLM_ENABLE_EXPERIMENTAL

#include "flecs.h"
#include "imgui.h"
#include "mv/mv.h"
#include "soloud.h"
#include "soloud_error.h"
#include "soloud_freeverbfilter.h"
#include "soloud_speech.h"
#include <epoxy/gl.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <math.h>
#include <memory>
#include <stdio.h>
#include <string>
#include <unordered_map>

float time_elapsed;

flecs::entity build_sprite_prefab(flecs::world &w) {
    flecs::entity sprite;

    sprite = w.prefab("Sprite");
    sprite.set<mv::CPosition>({0, 0});
    sprite.set<mv::CScale>({1, 1});
    sprite.set<mv::CAngleDegrees>({0});
    sprite.set<mv::CColor>({1, 1, 1, 1});
    sprite.set<mv::CTexture>(nullptr);
    sprite.set<mv::CMatrix>({1.0f});
    sprite.add<mv::CVertexQuad>();

    return sprite;
}

void setup_render_system(flecs::world &w, mv::Renderer &r) {
    using namespace mv;

    w.system<CPosition, CTexture, CVertexQuad>("UpdVertexPos")
        .kind(flecs::PostUpdate)
        .iter([](flecs::iter it, CPosition *pos, CTexture *t, CVertexQuad *q) {
            for (auto i : it) {
                auto tex = t[i];
                auto size = tex->get_tex_size();

                q[i].v[0].pos = {pos[i].x, pos[i].y};
                q[i].v[1].pos = {pos[i].x + size.x, pos[i].y};
                q[i].v[2].pos = {pos[i].x + size.x, pos[i].y + size.y};
                q[i].v[3].pos = {pos[i].x, pos[i].y + size.y};
            }
        });

    w.system<CColor, CVertexQuad>("UpdVertexColor")
        .kind(flecs::PostUpdate)
        .iter([](flecs::iter it, CColor *col, CVertexQuad *q) {
            for (auto i : it) {
                q[i].v[0].color = col[i];
                q[i].v[1].color = col[i];
                q[i].v[2].color = col[i];
                q[i].v[3].color = col[i];
            }
        });

    w.system<CTexture, CVertexQuad>("UpdVertexUV")
        .kind(flecs::PostUpdate)
        .iter([](flecs::iter it, CTexture *t, CVertexQuad *q) {
            for (auto i : it) {
                auto quad = t[i]->get_quad();
                auto coords = quad.get_texcoords();

                q[i].v[0].uv = coords[0];
                q[i].v[1].uv = coords[1];
                q[i].v[2].uv = coords[2];
                q[i].v[3].uv = coords[3];
            }
        });

    w.system<CPosition, CScale, CAngleDegrees, CTexture, CMatrix, CVertexQuad>(
         "Render")
        .kind(flecs::PostUpdate)
        .iter([&r](flecs::iter it, CPosition *p, CScale *sc, CAngleDegrees *ang,
                   CTexture *tex, CMatrix *mat, CVertexQuad *quad) {
            for (auto i : it) {
                auto tex_ref = tex[i];
                tex_ref->bind();

                mat[i] = glm::mat3(1.0f);
                float rads = ang[i].a * M_PI / 180.0f;
                float c = cosf(rads);
                float s = sinf(rads);

                mat[i] = glm::translate(mat[i], p[i]);
                mat[i] = glm::scale(mat[i], sc[i]);
                mat[i] = glm::rotate(mat[i], rads);
                mat[i] = glm::translate(mat[i], -p[i]);
                // mat[i][0][0] = c * sc[i].x;
                // mat[i][1][0] = s * sc[i].x;
                // mat[i][0][1] = -s * sc[i].y;
                // mat[i][1][1] = c * sc[i].y;
                // mat[i][0][2] = p[i].x * mat[i][0][0] - p[i].y * mat[i][0][1];
                // mat[i][1][2] = p[i].y * mat[i][1][0] - p[i].y * mat[i][1][1];

                for (Vertex &v : quad[i].v) {
                    float fx = v.pos.x * mat[i][0][0] + v.pos.y * mat[i][0][1] +
                               mat[i][0][2];
                    float fy = v.pos.x * mat[i][1][0] + v.pos.y * mat[i][1][1] +
                               mat[i][1][2];

                    v.pos.x = fx;
                    v.pos.y = fy;
                }

                r.push_quad(quad[i].v[0], quad[i].v[1], quad[i].v[2],
                            quad[i].v[3], tex[i]->get_id());
            }
        });
}

class MyGame : public mv::Context {
    // soloud engine
    SoLoud::Speech speech;
    SoLoud::PXTone pxt;
    SoLoud::FreeverbFilter verb;

    // flecs
    flecs::entity my_sprite[2];
    flecs::system draw_elements_system;

    mv::Texture kleines;
    std::shared_ptr<mv::Texture> kleines_ptr;

    mv::Camera2D cam;
    float time = 0.0f;
    float separation = 0.0f;

    bool speak_already_pressed = false;
    // unsigned int speak_handle = 0;
    unsigned int pxt_handle = 0;
    char speak_buffer[128] = "assets/pantest.ptcop";
    size_t speak_size = sizeof(speak_buffer);

  public:
    using mv::Context::Context;

    void on_talk_press() {
        soloud.stop(pxt_handle);
        // speech.setText(speak_buffer);
        unsigned int err = pxt.load(speak_buffer);
        if (err == SoLoud::SO_NO_ERROR)
            pxt_handle = soloud.play(pxt, 0.9f, 0.0f);
    }

    void init() override {
        using namespace mv;
        kleines =
            load_texture_from_source("kleines", kleines_png, kleines_png_size);
        kleines_ptr = std::shared_ptr<Texture>(&kleines);

        renderer->set_camera(&cam);

        soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF |
                        SoLoud::Soloud::ENABLE_VISUALIZATION,
                    SoLoud::Soloud::AUTO, 44100, 1024, 2);
        prefabs["Sprite"] = build_sprite_prefab(world);
        setup_render_system(world, *renderer);

        my_sprite[0] = world.entity("my_sprite").is_a(prefabs["Sprite"]);
        my_sprite[0].set<CTexture>(kleines_ptr->getptr());
        my_sprite[0].set<CScale>({1,1});

        my_sprite[1] = world.entity("my_sprite2").is_a(prefabs["Sprite"]);
        my_sprite[1].set<CTexture>(kleines_ptr->getptr());
        my_sprite[1].set<CScale>({3,1});
        my_sprite[1].set<CPosition>({5, 5});
        my_sprite[1].set<CAngleDegrees>({300.0f});
    }

    void update(double dt) override {
        time += dt * 5;
        my_sprite[1].set<mv::CPosition>({sinf(time) * 3, cosf(time) * 3});
        // my_sprite.set<mv::CAngleDegrees>({time * 2});
        // cam.position.x = sinf(time*0.5f) * 200;
        // cam.position.y = cosf(time*0.3f) * 200;
        // cam.rotation += 1.0f * dt;

        world.progress(dt);
        // printf("**\n\nmy_sprite type: %s\n\n", my_sprite.name().c_str());
    }

    void draw() override {
        // kleines.bind();
        // for (int i = 0; i < 12; i++) {
        //     renderer->push_quad({-128 + (separation * i), -128}, {256, 256},
        //                         {1, 1, 1, 1}, kleines.get_id());
        // }

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

int main() {
    printf("The project name is %s\n", PROJECT_NAME);

    auto game = std::make_shared<MyGame>(1440, 900, "microvidya");

    game->engine_init();
    game->run();

    return 0;
}
