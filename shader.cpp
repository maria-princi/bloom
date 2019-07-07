#include "shader.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertex_shd_path, const GLchar* fragment_shd_path)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_res;
    std::string fragment_res;
    std::ifstream vertex_shd_file;
    std::ifstream fragment_shd_file;
    // ensure ifstream objects can throw exceptions:
    vertex_shd_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragment_shd_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vertex_shd_file.open(vertex_shd_path);
        fragment_shd_file.open(fragment_shd_path);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertex_shd_file.rdbuf();
        fShaderStream << fragment_shd_file.rdbuf();
        // close file handlers
        vertex_shd_file.close();
        fragment_shd_file.close();
        // convert stream into string
        vertex_res   = vShaderStream.str();
        fragment_res = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertex_shd_src = vertex_res.c_str();
    const char * fragment_shd_src = fragment_res.c_str();

    // shaders creation
    int vertex_shd = this->create_shd(vertex_shd_src, GL_VERTEX_SHADER);
    int fragment_shd = this->create_shd(fragment_shd_src, GL_FRAGMENT_SHADER);

    // shaders linking and program creation
    program_ = this->link_shd(vertex_shd, fragment_shd);
}


int Shader::link_shd(int vertex_shd, int fragment_shd)
{
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shd);
    glAttachShader(shaderProgram, fragment_shd);

    glLinkProgram(shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex_shd);
    glDeleteShader(fragment_shd);

    return shaderProgram;
}

int Shader::create_shd(const char *shd_src, GLenum shd_type)
{
    int shd = glCreateShader(shd_type);
    glShaderSource(shd, 1, &shd_src, NULL);
    glCompileShader(shd);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shd, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shd, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shd;
}

void Shader::use()
{
    glUseProgram(program_);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}
