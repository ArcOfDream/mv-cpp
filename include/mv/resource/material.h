#include "resource.h"
#include "mv/graphics/shader.h"
#include "mv/gl.h"
#include <memory>
#include <vector>
#include <unordered_map>

#pragma once

namespace mv {

class Material : public IResource {
    std::unordered_map<std::string, UniformBase> uniforms;
public:
    std::shared_ptr<Shader> shader;

    Material(std::shared_ptr<Shader>, std::string&);

    template <typename T>
    void add_uniform(std::string &name, T value) {
        ShaderUniform<T> u = ShaderUniform<T>();
        u.name = name;
        u.location = glGetUniformLocation(shader->get_id(), name.c_str());
        u.value = value;

        uniforms[name] = u;
    }

    void update_uniforms();
    // void use();
};

class MaterialBuilder {
    std::shared_ptr<Material> mat = nullptr;
public:
    MaterialBuilder(std::string&);

    MaterialBuilder &begin(std::string &vs, std::string &fs);
    MaterialBuilder &begin(std::shared_ptr<Shader>);

    template <typename T>
    MaterialBuilder &uniform(std::string &name, T value) {
        Material &m = *mat;
        m.add_uniform<T>(name, value);

        return *this;
    }

    std::shared_ptr<Material> end();
};

}
