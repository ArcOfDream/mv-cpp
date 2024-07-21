#include "resource.h"
#include "mv/graphics/shader.h"
#include <unordered_map>

#pragma once

namespace mv {

class Material : public IResource {
public:
    std::unordered_map<std::string, UniformBase> uniforms;
    std::shared_ptr<Shader> shader;

    Material(std::shared_ptr<Shader>, std::string&);

    // template <typename T>
    // void add_uniform(std::string &name, T value) {
    //     ShaderUniform<T> u = ShaderUniform<T>();
    //     u.name = name;
    //     u.location = glGetUniformLocation(shader->get_id(), name.c_str());
    //     u.value = value;

    //     uniforms[name] = u;
    // }

    void update_uniforms();
    // void use();
};

class MaterialBuilder {
    std::shared_ptr<Material> mat = nullptr;
public:
    MaterialBuilder(std::string);

    MaterialBuilder &begin(std::string &vs, std::string &fs);
    MaterialBuilder &begin(std::shared_ptr<Shader>);

    MaterialBuilder &uniform_bool(std::string, bool);
    MaterialBuilder &uniform_int(std::string, int);
    MaterialBuilder &uniform_float(std::string, float);
    MaterialBuilder &uniform_vec2(std::string, glm::vec2);
    MaterialBuilder &uniform_vec3(std::string, glm::vec3);
    MaterialBuilder &uniform_vec4(std::string, glm::vec4);
    MaterialBuilder &uniform_mat3(std::string, glm::mat3);

    std::shared_ptr<Material> end();
};

}
