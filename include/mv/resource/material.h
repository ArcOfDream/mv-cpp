#include "resource.h"
#include "mv/graphics/shader.h"
#include <unordered_map>

#pragma once

namespace mv {

class Material : public IResource {
public:
    std::unordered_map<std::string, Uniform> uniforms;
    std::shared_ptr<Shader> shader;

    Material(std::shared_ptr<Shader>, std::string);

    // template <typename T>
    // void add_uniform(std::string &name, T value) {
    //     ShaderUniform<T> u = ShaderUniform<T>();
    //     u.name = name;
    //     u.location = glGetUniformLocation(shader->get_id(), name.c_str());
    //     u.value = value;

    //     uniforms[name] = u;
    // }
    
    template <typename T>
    void set_uniform(std::string name, const T value) {
        if (uniforms.count(name)) {
            uniforms.at(name).value = value;
        }
    }
    
    void update_uniforms();
    void use();

    std::shared_ptr<Material> getptr() { return shared_from_base<Material>(); }
};

class MaterialBuilder {
    std::shared_ptr<Material> mat = nullptr;
public:
    MaterialBuilder(std::string);

    MaterialBuilder &begin(const std::string vs, const std::string fs);
    MaterialBuilder &begin(std::shared_ptr<Shader>);

    MaterialBuilder &uniform_bool(const std::string, const bool);
    MaterialBuilder &uniform_int(const std::string, const int);
    MaterialBuilder &uniform_float(const std::string, const float);
    MaterialBuilder &uniform_vec2(const std::string, const glm::vec2);
    MaterialBuilder &uniform_vec3(const std::string, const glm::vec3);
    MaterialBuilder &uniform_vec4(const std::string, const glm::vec4);
    MaterialBuilder &uniform_mat3(const std::string, const glm::mat3);

    std::shared_ptr<Material> end();
};

}
