#include "shaderProgram.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <stdexcept>

void ShaderProgram::create(std::vector<Shader> shaders){
    // load and compile shaders
    for (Shader& shader : shaders){
        // Open shader file
        std::ifstream file(shader.source);
        if (!file.is_open()) { 
            throw std::runtime_error("Failed to open shader: " + shader.source);
        } else {
            // No action needed
        }

        // Read shader into memory
        std::string line, text;
        while (std::getline(file, line)){
            text += line + "\n";
        }
        const char* data = text.c_str();
        file.close();

        // compile shader
        shader.glId = glCreateShader(shader.type);
        glShaderSource(shader.glId, 1, &data, NULL);
        glCompileShader(shader.glId);

        // check for opengl errors after compiling shader
        int status;
        char error[512];
        glGetShaderiv(shader.glId, GL_COMPILE_STATUS, &status);
        
        if(!status){
            glGetShaderInfoLog(shader.glId, 512, NULL, error);
            throw std::runtime_error("Failed to compile shader: " + shader.source + "\n" + error);
        } else {
            // No action needed
        }
    }

    // link program
    glId = glCreateProgram();
    initialised = true;
    for (Shader& shader : shaders){
        glAttachShader(glId, shader.glId);
    }
    glLinkProgram(glId);
    
    // check for opengl errors after linking shader program
    int status;
    char error[512];
    glGetProgramiv(glId, GL_LINK_STATUS, &status);
    if(!status){
        glGetProgramInfoLog(glId, 512, NULL, error);
        throw std::runtime_error("Failed to link shader program: " + std::string(error));
    } else {
        // No action needed
    }

    // discard uneeded shader objects
    for (Shader& shader : shaders){
        glDeleteShader(shader.glId);
    }
}

ShaderProgram::~ShaderProgram() { 
    if (initialised) { 
        glDeleteProgram(glId);
    } else {
        // No action needed
    }
}