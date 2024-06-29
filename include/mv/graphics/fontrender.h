#include "fontstash.h"
#include "glm/vec4.hpp"

#pragma once

namespace mv {
struct FontRender {
    int width, height = 0;
    FONScontext *ctx = nullptr;
    unsigned int tid = 0; // texture id
    glm::vec4 color = {1, 1, 1, 1};

    void delete_tex();
    void setup_context();
};

int render_create(void *, int, int);
void render_update(void *, int *, const unsigned char *);
void render_draw(void *, const float *, const float *, const unsigned int *,
                 int);
void render_delete(void *);
void push_quad(void *, const FONSquad *);

} // namespace mv