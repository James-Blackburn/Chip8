#include "shaderProgram.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <stdexcept>

void ShaderProgram::create(std::vector<Shader> shaders){
    // Compile shaders
    for (Shader& shader : shaders){
        // compile shader
        shader.glId = glCreateShader(shader.type);
        const char* source = shader.source.c_str();
        glShaderSource(shader.glId, 1, &source, NULL);
        glCompileShader(shader.glId);

        // check for opengl errors after compiling shader
        int status;
        char error[512];
        glGetShaderiv(shader.glId, GL_COMPILE_STATUS, &status);
        
        if(!status){
            glGetShaderInfoLog(shader.glId, 512, NULL, error);
            throw std::runtime_error("Failed to compile shader: \n" + shader.source + "\n" + error);
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