#include "fontstash.h"
#include "glm/vec4.hpp"
#include <string>
#include <unordered_map>


#pragma once

namespace mv {
struct FontRender {
    int width, height = 0;
    FONScontext *ctx = nullptr;
    unsigned int tid = 0; // texture id
    glm::vec4 color = {1, 1, 1, 1};

    std::unordered_map<std::string, int> fonts;

    void delete_tex();
    void setup_context();
    void font(std::string);
    int add_font_mem(std::string, const unsigned char*, const long);
};

int render_create(void *, int, int);
void render_update(void *, int *, const unsigned char *);
void render_draw(void *, const float *, const float *, const unsigned int *,
                 int);
void render_delete(void *);
void push_quad(void *, const FONSquad *);

} // namespace mv