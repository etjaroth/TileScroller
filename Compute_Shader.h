#pragma once

#include "opengl_libs.h"

class Compute_Shader
{
private:
    unsigned int shader_program;
    unsigned int compute_shader;
    const char* shader_source;

    int tex_x, tex_y, tex_z;

public:
    Compute_Shader(const char* filename, int x, int y, int z);
    void use() const;
    void dontuse() const;

    // 
    void fill_texture(GLuint tex);
    GLuint generate_texture(bool fill);
    void print_info();

    // Pass info to shaders
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_uint(const std::string& name, unsigned int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec2(const std::string& name, const glm::vec2& value) const;
    void set_vec2(const std::string& name, float x, float y) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_vec3(const std::string& name, float x, float y, float z) const;
    void set_vec4(const std::string& name, const glm::vec4& value) const;
    void set_vec4(const std::string& name, float x, float y, float z, float w);
    void set_mat2(const std::string& name, const glm::mat2& mat) const;
    void set_mat3(const std::string& name, const glm::mat3& mat) const;
    void set_mat4(const std::string& name, const glm::mat4& mat) const;
};

