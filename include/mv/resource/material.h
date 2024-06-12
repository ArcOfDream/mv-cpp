#include "resource.h"
#include "mv/graphics/shader.h"
#include <vector>
#include <map>

#pragma once

namespace mv {

class Material : public IResource {
    std::map<std::string, ShaderUniform> uniforms;
public:
    Shader &shader;

    Material(Shader, std::string);
    void add_uniform(std::string, UniformType);
    void update_uniforms();
    void use();
};
}