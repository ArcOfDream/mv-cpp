
#include "mv/graphics/fontrender.h"

#define FONTSTASH_IMPLEMENTATION
#define FONS_USE_FREETYPE
#include <ft2build.h>
#include "fontstash.h"

#include "mv/binary/slkscr.h"
#include "mv/gl.h"
#include "mv/graphics/renderer.h"

namespace mv {

constexpr int ceilpow2i(int val) {
    int x = val;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

// let's see if we can pull this one off
void FontRender::delete_tex() {
    if (tid != 0)
        glDeleteTextures(1, &tid);
    tid = 0;
}

void FontRender::setup_context() {
    FONSparams params{
        width,         
        height,        
        FONS_ZERO_TOPLEFT,
        this,          
        render_create, 
        render_create,
        render_update, 
        nullptr,   
        render_delete,
        push_quad};
    ctx = fonsCreateInternal(&params);

    fonsAddFontMem(ctx, "silkscreen", (unsigned char*)slkscr_ttf, slkscr_ttf_size, 0);
}

int render_create(void *ptr, int w, int h) {
    auto ctx = (FontRender*) ptr;
    ctx->delete_tex();
    ctx->width = w;
    ctx->height = h;
    glGenTextures(1, &ctx->tid);

    glBindTexture(GL_TEXTURE_2D, ctx->tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 1;
}

void render_update(void *ptr, int *rect, const unsigned char *data) {
    auto ctx = (FontRender*) ptr;
    int h = rect[3] - rect[1];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, ctx->tid);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ctx->width, ceilpow2i(h),
                    GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
}

void render_delete(void *ptr) { 
    auto ctx = (FontRender*) ptr;
    ctx->delete_tex(); 
    }

void render_draw(void *ptr, const float *verts, const float *tcoords,
                 const unsigned int *colors, int nverts) {
    auto ctx = (FontRender*) ptr;   
    Renderer &r = Renderer::get();
    for (int i = 0; i < nverts; i++) {
        r.push_triangle(
            {verts[i + 0], verts[i + 1]},
            {verts[i + 4], verts[i + 5]},
            {verts[i + 2], verts[i + 3]}, 
            ctx->color, ctx->color, ctx->color,
            {tcoords[i + 0], tcoords[i + 1]}, 
            {tcoords[i + 4], tcoords[i + 5]},
            {tcoords[i + 2], tcoords[i + 3]}, 
            ctx->tid);
        r.push_triangle(
                {verts[i + 6], verts[i + 7]},
                {verts[i + 10], verts[i + 11]},
                {verts[i + 8], verts[i + 9]}, 
                ctx->color, ctx->color, ctx->color, 
                {tcoords[i + 6], tcoords[i + 7]},
                {tcoords[i + 10], tcoords[i + 11]},
                {tcoords[i + 8], tcoords[i + 9]}, 
                ctx->tid);
    }
}

void push_quad(void *ptr, const FONSquad *quad) {
    auto ctx = (FontRender*) ptr;
    Renderer &r = Renderer::get();
    auto &q = *quad;
    r.push_triangle({q.x0, q.y0}, {q.x1, q.y0}, {q.x1, q.y1}, ctx->color,
                    ctx->color, ctx->color, {q.s0, q.t0}, {q.s1, q.t0},
                    {q.s1, q.t1}, ctx->tid);
    r.push_triangle({q.x0, q.y0}, {q.x0, q.y1}, {q.x1, q.y1}, ctx->color,
                    ctx->color, ctx->color, {q.s0, q.t0}, {q.s0, q.t1},
                    {q.s1, q.t1}, ctx->tid);
}

} // namespace mv