#include <glm/ext/matrix_float3x3.hpp>
#define GLM_FORCE_PURE

#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>
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

struct ShaderUniform {
    std::string name;
    UniformType type;
    int location;
};

class Shader {
    GLuint id;
    std::vector<ShaderUniform> uniforms;

  public:
    Shader();
    Shader(std::shared_ptr<Shader> &);
    Shader(const char *vs_path, const char *fs_path);
    void use();
    void set_shader_program(GLuint pid);
    GLuint get_id();
    std::vector<ShaderUniform> get_uniforms();

    void set_bool(const std::string &, bool) const;
    void set_bool(const ShaderUniform &, bool) const;

    void set_int(const std::string &, int) const;
    void set_int(const ShaderUniform &, int) const;

    void set_int_array(const std::string &, unsigned int, int *) const;
    void set_uint_array(const std::string &, unsigned int, unsigned int *) const;

    void set_float(const std::string &, float) const;
    void set_float(const ShaderUniform &, float) const;

    void set_vec2(const std::string &, const glm::vec2 &) const;
    void set_vec2(const ShaderUniform &, const glm::vec2 &) const;

    void set_vec3(const std::string &, const glm::vec3 &) const;
    void set_vec3(const ShaderUniform &, const glm::vec3 &) const;

    void set_vec4(const std::string &, const glm::vec4 &) const;
    void set_vec4(const ShaderUniform &, const glm::vec4 &) const;

    void set_mat3(const std::string &, const glm::mat3 &) const;
    void set_mat3(const ShaderUniform &, const glm::mat3 &) const;
};

GLuint load_shader(const char *src, GLenum shader_type);
GLuint link_shader_program(GLuint vertex, GLuint fragment);
Shader load_default_shader();

} // namespace mv