#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include "emulator.hpp"
#include "shaderProgram.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <thread>
#include <string>

class Application {
    private:
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 640;

    Emulator emulator;
    GLFWwindow* window;
    ShaderProgram quadShader;
    
    int fbWidth, fbHeight;
    GLuint quadVaoId;
    GLuint textureId;
    
    std::thread emulatorThread;
    std::string currentRomPath;

    void loadShaders();
    void createRenderData();
    void renderEmulator();
    void renderInterface();
    void updateEmulatorKeyboard();
    std::string selectFile();
    void stopEmulation();
    
    public:
    Application();
    ~Application();

    void run();
};