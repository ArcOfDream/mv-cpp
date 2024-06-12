#include "mv/resource/texture.h"
#include "mv/graphics/quad.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rwops.h>
#include <epoxy/gl.h>
#include <string>

namespace mv {

Texture::Texture() { type = INVALID; }

Texture::Texture(std::string resource_name) {
    type = IMAGE;
    name = resource_name;
}

unsigned int Texture::get_id() { return tex_id; }

void Texture::gen_id() { glGenTextures(1, &tex_id); }

void Texture::gen_with(const void *pixels, int width, int height,
                       GLenum fmt) {
    tex_size = {width, height};
    quad = Quad(0, 0, tex_size.x, tex_size.y, width, height);
    format = fmt;

    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, flag_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, flag_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, tex_id); }

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

Texture load_texture_raw(std::string name, int w, int h, GLenum format,
                                 const void *pixels) {
    Texture t{name};
    t.gen_id();
    t.gen_with(pixels, w, h, format);
    t.unbind();

    return t;
}

Texture load_texture_from_file(std::string name, std::string path) {
    SDL_RWops *file = SDL_RWFromFile(path.c_str(), "r");
    if (!file) {
        return {};
    }

    auto img = IMG_Load_RW(file, 1);
    if (!img) {
        return {};
    }

    GLenum format;
    //unsigned int channels;

    switch (img->format->Amask) {
    case 0:
        format = GL_RGB;
        //channels = 3;
        break;

    default:
        format = GL_RGBA;
        //channels = 4;
        break;
    }

    return load_texture_raw(name, img->w, img->h, format, img->pixels);
}

Texture load_texture_from_source(std::string name, const void *bytes,
                                 unsigned long size) {
    auto file = SDL_RWFromConstMem(bytes, size);
    if (!file) { return {}; }

    auto img = IMG_Load_RW(file, 1);
    if (!img) {
        return {};
    }

    GLenum format;
    // unsigned int channels;

    switch (img->format->Amask) {
    case 0:
        format = GL_RGB;
        //channels = 3;
        break;

    default:
        format = GL_RGBA;
        //channels = 4;
        break;
    }

    return load_texture_raw(name, img->w, img->h, format, img->pixels);
}
} // namespace mv
