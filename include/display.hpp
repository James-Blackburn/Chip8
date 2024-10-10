#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderProgram.hpp"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

class Display {
    private:
        unsigned char pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT]{0};
        float textureData[DISPLAY_WIDTH * DISPLAY_HEIGHT * 3]{0.0f};
        int fbWidth, fbHeight;
        bool initialised = false;
        bool pendingTextureUpdate = false;
        
        GLFWwindow* window;
        ShaderProgram quadShader;
        unsigned int quadVaoId;
        unsigned int textureId;

        void createWindow();
        void loadShaders();
        void createRenderData();
        void renderPixels();

        friend class Keyboard;
    public:
        ~Display();

        void init();
        void update();
        void setPixel(int px, int py, unsigned char value);
        unsigned char readPixel(int px, int py);
        void clear();
        
        inline bool isShowing() const;
        inline void pollEvents() const;
};

bool Display::isShowing() const { 
    return !glfwWindowShouldClose(window);
}

void Display::pollEvents() const {
    glfwPollEvents();
}