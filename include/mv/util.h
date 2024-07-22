#include <math.h>
#include <string>

#pragma once

#define deg_to_rad(val) ((val) * M_PI / 180.0)
#define rad_to_deg(val) ((val) * 180.0 / M_PI)

// Base: Node
#define DEFAULT_UPDATE(class_name) void class_name::_update(double dt) {\
    for (auto &child : children) {                                      \
        child->_update(dt);                                             \
    }                                                                   \
                                                                        \
    if (wren_constructed) wren_update(this, dt);                        \
    else update(dt);                                                    \
}

// Base: Sprite
#define DEFAULT_DRAW(class_name) void class_name::_draw() {             \
    if (dirty) {                                                        \
        rebuild_transform();                                            \
        dirty = false;                                                  \
    }                                                                   \
                                                                        \
    if (tex)                                                            \
        Renderer::get().push_quad(verts[0], verts[1],                   \
                                  verts[2], verts[3], tex->get_id());   \
                                                                        \
    for (auto &child : children) {                                      \
        if (Sprite *spr = dynamic_cast<Sprite *>(child.get())) {        \
            spr->_draw();                                               \
        }                                                               \
    }                                                                   \
                                                                        \
    if (wren_constructed) wren_draw(this);                              \
    else draw();                                                        \
}

const std::string default_vert = R"glsl(#version 100
attribute vec2 attribPos;
attribute vec2 attribUV;
attribute vec4 attribColor;

varying mediump vec4 vertexColor;
varying mediump vec2 texUV;

uniform mat3 view;
uniform mat3 projection;

void main() {
	vertexColor = attribColor;
	texUV = attribUV;
   gl_Position = vec4(projection * vec3(attribPos, 1.0), 1.0);
}
)glsl";

const std::string default_frag = R"glsl(#version 100
varying mediump vec4 vertexColor;
varying mediump vec2 texUV;

uniform sampler2D texID;

void main() {
	gl_FragColor = texture2D(texID, texUV) * vertexColor;
}
)glsl";