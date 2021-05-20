#pragma once
#include "opengl_libs.h"

class Shader
{
public:
    unsigned int shaderProgram;

    unsigned int vertexShader;
    unsigned int geometryShader;
    unsigned int fragmentShader;

    const char* vertexShaderSource;
    const char* geometryShaderSource;
    const char* fragmentShaderSource;

    void make_shader(const char* shaderPath, GLenum shaderType);

    //public:
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
    ~Shader();
    void use(); // Uses shader
    void dontuse();

    // Pass info to shaders
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
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