#include "mv/graphics/shader.h"
#include "mv/gl.h"
#include "mv/util.h"
#include "mv/config.h"
#include <assert.h>
#include <charconv>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <string>
#include <variant>
#include <vector>

namespace mv {

Shader::Shader(std::string vs_code, std::string fs_code) {
    GLuint vs, fs;
    vs = load_shader(vs_code.c_str(), GL_VERTEX_SHADER);
    fs = load_shader(fs_code.c_str(), GL_FRAGMENT_SHADER);
    id = link_shader_program(vs, fs);
}

Shader::Shader() {
    GLuint vs, fs;
    vs = load_shader(default_vert.c_str(), GL_VERTEX_SHADER);
    fs = load_shader(default_frag.c_str(), GL_FRAGMENT_SHADER);
    id = link_shader_program(vs, fs);
}

Shader::Shader(std::shared_ptr<Shader> &shd) {
    id = shd->get_id();
}

void Shader::use() { glUseProgram(id); }

void Shader::set_shader_program(GLuint pid) { id = pid; }

GLuint Shader::get_id() { return id; }

void Shader::set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
};

void Shader::set_bool(Uniform &u) const {
    if (const bool* value = std::get_if<bool>(&u.value))
        glUniform1i(u.location, *value);
};

void Shader::set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
};

void Shader::set_int(Uniform &u) const {
    if (const int* value = std::get_if<int>(&u.value)) {
        glUniform1i(u.location, *value);
        printf("%i", *value);
    }
};

void Shader::set_int_array(const std::string &name, unsigned int array_size,
                           int *value) const {
    glUniform1iv(glGetUniformLocation(id, name.c_str()), array_size, value);
}

// void Shader::set_uint_array(const std::string &name, unsigned int array_size,
//                             unsigned int *value) const {
// #ifndef MV_USE_PURE_GLES2
//     glUniform1uiv(glGetUniformLocation(id, name.c_str()), array_size, value);
// #endif
// }

void Shader::set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
};

void Shader::set_float(Uniform &u) const {
    if (const float* value = std::get_if<float>(&u.value))
        glUniform1f(u.location, *value);
};

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
};

void Shader::set_vec2(Uniform &u) const {
    if (const glm::vec2* value = std::get_if<glm::vec2>(&u.value))
        glUniform2f(u.location, value->x, value->y);
};

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y,
                value.z);
};

void Shader::set_vec3(Uniform &u) const {
    if (const glm::vec3* value = std::get_if<glm::vec3>(&u.value))
        glUniform3f(u.location, value->x, value->y, value->z);
};

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y,
                value.z, value.w);
};

void Shader::set_vec4(Uniform &u) const {
    if (const glm::vec4* value = std::get_if<glm::vec4>(&u.value))
        glUniform4f(u.location, value->x, value->y, value->z, value->w);
};

void Shader::set_mat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
};

void Shader::set_mat3(Uniform &u) const {
    if (const glm::mat3* value = std::get_if<glm::mat3>(&u.value)) {        
        auto &m = *value;
        glUniformMatrix3fv(u.location, 1, GL_FALSE, &m[0][0]);
    }
};

GLuint load_shader(const char *src, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    int compiled = 0;

    if (shader == 0 || strlen(src) == 0) {
        return 0;
    }

    const GLint len = strlen(src);
    glShaderSource(shader, 1, &src, &len);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (compiled == 0) {
        int info_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

        printf("shader compilation error!\n");
        if (info_len > 0) {
            char log[MAX_LOG_SIZE]{};

            glGetShaderInfoLog(shader, MAX_LOG_SIZE, &info_len, log);
            printf("log:\n %s\n", log);
        }

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint link_shader_program(GLuint vertex, GLuint fragment) {
    GLuint program = glCreateProgram();
    int success = 0;

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success != 1) {
        int info_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);

        printf("shader program link error!\n");
        if (info_len > 0) {
            char log[MAX_LOG_SIZE]{};

            glGetProgramInfoLog(program, MAX_LOG_SIZE, &info_len, log);
            printf("log:\n %s\n", log);
        }

        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

Shader load_default_shader() { return {}; }

Shader load_shader_file(std::string vs_path, std::string fs_path) {
    std::string vs_code, fs_code, line;
    std::ifstream vs_file, fs_file;

    vs_file.open(vs_path);
    assert(vs_file.is_open());
    fs_file.open(fs_path);
    assert(fs_file.is_open());

    while (!vs_file.eof()) {
        std::getline(vs_file, line);
        vs_code += line;
    }

    line = "";
    while (!fs_file.eof()) {
        std::getline(fs_file, line);
        fs_code += line;
    }

    vs_file.close();
    fs_file.close();
    
    return Shader(vs_code, fs_code);
}
} // namespace mv
