#define GLM_FORCE_PURE

#include "mv/config.h"
#include "camera2d.h"
#include "drawcall.h"
#include "quad.h"
#include "shader.h"
#include "vertex.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <epoxy/gl.h>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>

#pragma once

namespace mv {

class Renderer {
    glm::mat3 projection;
    std::shared_ptr<Camera2D> active_camera = nullptr;

    DrawCall drawcalls[MAX_DRAWCALLS];
    int active_drawcall = 0;
    int drawcall_amount = MAX_DRAWCALLS;
    unsigned int total_verts = 0;

    glm::vec4 clear_color = {0.1f, 0.2f, 0.5f, 1.0f};
    SDL_GLContext gl_context = 0;
    std::shared_ptr<Shader> default_shader = nullptr;

  public:
    float width;
    float height;

    Renderer();
    void init();
    void set_context(SDL_GLContext);
    void set_camera(std::shared_ptr<Camera2D>);
    std::shared_ptr<Camera2D> get_camera();
    void clear_frame();
    void begin_frame();
    void end_frame();
    void next_drawcall();
    void flush_drawcalls();

    void push_triangle(glm::vec2 apos, glm::vec2 bpos, glm::vec2 cpos,
                       glm::vec4 acol, glm::vec4 bcol, glm::vec4 ccol,
                       glm::vec2 auv, glm::vec2 buv, glm::vec2 cuv,
                       unsigned int tex_id);

    void push_quad(glm::vec2 pos, glm::vec2 size, Quad &uv, glm::vec4 tint,
                   unsigned int tex);
    void push_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 tint,
                   unsigned int tex);
    void push_quad(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4,
                   unsigned int tex);
};

} // namespace mv
