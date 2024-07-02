#include "mv/graphics/vertex.h"
#include <GLES2/gl2.h>
#include <cstddef>
#define GLM_ENABLE_EXPERIMENTAL

#include "mv/components/graphics.h"
#include "mv/config.h"
#include "mv/gl.h"
#include "mv/graphics/graphics.h"
#include <SDL2/SDL.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace mv {

Renderer::~Renderer() {
    for (DrawCall &dc : drawcalls) {
        dc.~DrawCall();
    }
}

void Renderer::init() {
    default_shader = std::shared_ptr<Shader>(new Shader());

    for (unsigned long i = 0; i < MAX_DRAWCALLS; i++) {
        drawcalls[i].vbo.buffer_data(0, MAX_VERTICES * sizeof(Vertex));
    }

    // drawcall_amount = drawcalls.size();
    projection = glm::mat3(glm::ortho(0.0f, width, height, 0.0f));

    glEnable(GL_BLEND);
    glDepthFunc(GL_NEVER);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef MV_GL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&gl_message_callback, 0);
#endif
}

void Renderer::set_context(SDL_GLContext ctx) { gl_context = ctx; }

void Renderer::set_camera(Camera2D *cam) { active_camera = cam; }

Camera2D *Renderer::get_camera() { return active_camera; }

void Renderer::clear_frame() {
    glViewport(0, 0, width, height);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::begin_frame() {
    projection = glm::mat3(glm::ortho(0.0f, width, height, 0.0f));
    if (active_camera) {
        active_camera->update();
        projection *= active_camera->view_transform;
        // projection = active_camera->view_transform * projection;
    }
}

void Renderer::next_drawcall() {
    active_drawcall++;

    if (active_drawcall >= drawcall_amount) {
        flush_drawcalls();
        active_drawcall = 0;
    }
}

void Renderer::flush_drawcalls() {
    for (DrawCall &dc : drawcalls) {
        if (dc.vertex_count == 0) {
            dc.active_texture = 0;
            continue;
        }

        glBindTexture(GL_TEXTURE_2D, dc.active_texture);
        dc.vbo.bind();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)(offsetof(Vertex, uv)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)(offsetof(Vertex, color)));

        glBufferSubData(GL_ARRAY_BUFFER, 0, dc.vertex_count * sizeof(Vertex),
                        dc.vertices);

        if (dc.shader) {
            dc.shader->use();
            dc.shader->set_int("tex", dc.active_texture);
            dc.shader->set_mat3("projection", projection);
        }
        total_verts += dc.vertex_count;

        glDrawArrays(GL_TRIANGLES, 0, dc.vertex_count);

        dc.vertex_count = 0;
        dc.active_texture = 0;
        dc.shader = default_shader;
    }
    total_verts = 0;
}

void Renderer::end_frame() {
    flush_drawcalls();
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::push_triangle(glm::vec2 apos, glm::vec2 bpos, glm::vec2 cpos,
                             glm::vec4 acol, glm::vec4 bcol, glm::vec4 ccol,
                             glm::vec2 auv, glm::vec2 buv, glm::vec2 cuv,
                             unsigned int tex_id) {

    if (tex_id != drawcalls[active_drawcall].active_texture) {
        if (drawcalls[active_drawcall].active_texture != 0) {
            next_drawcall();
        }
        drawcalls[active_drawcall].active_texture = tex_id;
    }

    if (drawcalls[active_drawcall].vertex_count >= MAX_VERTICES) {
        next_drawcall();
    }

    auto pos = drawcalls[active_drawcall].vertex_count;
    drawcalls[active_drawcall].vertices[pos + 0].pos = apos;
    drawcalls[active_drawcall].vertices[pos + 0].uv = auv;
    drawcalls[active_drawcall].vertices[pos + 0].color = acol;
    drawcalls[active_drawcall].vertices[pos + 1].pos = bpos;
    drawcalls[active_drawcall].vertices[pos + 1].uv = buv;
    drawcalls[active_drawcall].vertices[pos + 1].color = bcol;
    drawcalls[active_drawcall].vertices[pos + 2].pos = cpos;
    drawcalls[active_drawcall].vertices[pos + 2].uv = cuv;
    drawcalls[active_drawcall].vertices[pos + 2].color = ccol;

    drawcalls[active_drawcall].vertex_count += 3;
}

void Renderer::push_quad(glm::vec2 pos, glm::vec2 size, Quad &uv,
                         glm::vec4 tint, unsigned int tex) {
    glm::vec2 *texcoords = uv.get_texcoords();
    push_triangle({pos.x, pos.y}, {pos.x + size.x, pos.y},
                  {pos.x + size.x, pos.y + size.y}, tint, tint, tint,
                  texcoords[0], texcoords[1], texcoords[2], tex);
    push_triangle({pos.x, pos.y}, {pos.x, pos.y + size.y},
                  {pos.x + size.x, pos.y + size.y}, tint, tint, tint,
                  texcoords[0], texcoords[3], texcoords[2], tex);
}

void Renderer::push_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 tint,
                         unsigned int tex) {
    push_triangle({pos.x, pos.y}, {pos.x + size.x, pos.y},
                  {pos.x + size.x, pos.y + size.y}, tint, tint, tint,
                  {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, tex);
    push_triangle({pos.x, pos.y}, {pos.x, pos.y + size.y},
                  {pos.x + size.x, pos.y + size.y}, tint, tint, tint,
                  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, tex);
}

void Renderer::push_quad(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4,
                         unsigned int tex) {
    push_triangle(v1.pos, v2.pos, v3.pos, v1.color, v2.color, v3.color, v1.uv,
                  v2.uv, v3.uv, tex);
    push_triangle(v1.pos, v4.pos, v3.pos, v1.color, v4.color, v3.color, v1.uv,
                  v4.uv, v3.uv, tex);
}

} // namespace mv
