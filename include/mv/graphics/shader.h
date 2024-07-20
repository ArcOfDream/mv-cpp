#include <glm/ext/matrix_float3x3.hpp>
#define GLM_FORCE_PURE

#include "mv/gl.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

#pragma once

namespace mv {

enum UniformType {
    BOOL,
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
};

class UniformBase {
public:
    std::string name;
    // UniformType type;
    int location;
};

template<typename Type>
class ShaderUniform : public UniformBase {
public:
    Type value;
};

class Shader {
    GLuint id;
    std::vector<UniformBase> uniforms;

  public:
    Shader();
    Shader(std::shared_ptr<Shader> &);
    Shader(std::string &vs_code, std::string &fs_code);
    void use();
    void set_shader_program(GLuint pid);
    GLuint get_id();
    std::vector<UniformBase> &get_uniforms();

    void set_bool(const std::string &, bool) const;
    void set_bool(ShaderUniform<bool> &, bool) const;

    void set_int(const std::string &, int) const;
    void set_int(ShaderUniform<int> &, int) const;

    void set_int_array(const std::string &, unsigned int, int *) const;
    void set_uint_array(const std::string &, unsigned int, unsigned int *) const;

    void set_float(const std::string &, float) const;
    void set_float(ShaderUniform<float> &, float) const;

    void set_vec2(const std::string &, const glm::vec2 &) const;
    void set_vec2(ShaderUniform<glm::vec2> &, const glm::vec2 &) const;

    void set_vec3(const std::string &, const glm::vec3 &) const;
    void set_vec3(ShaderUniform<glm::vec3> &, const glm::vec3 &) const;

    void set_vec4(const std::string &, const glm::vec4 &) const;
    void set_vec4(ShaderUniform<glm::vec4> &, const glm::vec4 &) const;

    void set_mat3(const std::string &, const glm::mat3 &) const;
    void set_mat3(ShaderUniform<glm::mat3> &, const glm::mat3 &) const;
};

GLuint load_shader(const char *src, GLenum shader_type);
GLuint link_shader_program(GLuint vertex, GLuint fragment);
Shader load_default_shader();
Shader load_shader_file(std::string &vs_path, std::string &fs_path);

} // namespace mv