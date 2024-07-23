#include "mv/gl.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#pragma once

namespace mv {

enum UniformType {
    AUTO = -1,
    BOOL,
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT3,
    MAT4,
    SAMPLER2D,
};

struct Uniform {
    std::string name;
    UniformType type;
    int location;
    std::variant<
        bool,int,float,
        glm::vec2,glm::vec3,
        glm::vec4,glm::mat3> value; 
};

class Shader {
    GLuint id;

  public:
    Shader();
    Shader(std::shared_ptr<Shader> &);
    Shader(std::string vs_code, std::string fs_code);
    void use();
    void set_shader_program(GLuint pid);
    GLuint get_id();

    void set_bool(const std::string &, bool) const;
    void set_bool(Uniform &) const;

    void set_int(const std::string &, int) const;
    void set_int(Uniform &) const;

    void set_int_array(const std::string &, unsigned int, int *) const;
    // void set_uint_array(const std::string &, unsigned int, unsigned int *) const;

    void set_float(const std::string &, float) const;
    void set_float(Uniform &) const;

    void set_vec2(const std::string &, const glm::vec2 &) const;
    void set_vec2(Uniform &) const;

    void set_vec3(const std::string &, const glm::vec3 &) const;
    void set_vec3(Uniform &) const;

    void set_vec4(const std::string &, const glm::vec4 &) const;
    void set_vec4(Uniform &) const;

    void set_mat3(const std::string &, const glm::mat3 &) const;
    void set_mat3(Uniform &) const;
};

GLuint load_shader(const char *src, GLenum shader_type);
GLuint link_shader_program(GLuint vertex, GLuint fragment);
Shader load_default_shader();
Shader load_shader_file(std::string vs_path, std::string fs_path);

} // namespace mv