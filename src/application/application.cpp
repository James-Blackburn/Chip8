#include "application.hpp"

Application::Application() { 
    // Initialize glfw
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise glfw.");
    } else { 
        // No action needed
    }
    
    // Configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // create window with glfw
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chip8 Emulator", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create glfw window.");
    } else { 
        // No action needed
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glfwSwapInterval(1);

    // load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad.");
    } else {
        // No action needed
    }

    glViewport(0, 0, fbWidth, fbHeight);

    // Initialise ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    loadShaders();
    createRenderData();
}

Application::~Application() { 
    // Clean up ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // Clean up OpenGL objects
    glDeleteVertexArrays(1, &quadVaoId);
    
    // Clean up GLFW 
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::loadShaders() { 
    // Load shader
    quadShader.create({
        {"./res/shaders/quad.vert", GL_VERTEX_SHADER},
        {"./res/shaders/quad.frag", GL_FRAGMENT_SHADER}
    });
}

void Application::createRenderData() {
    // Create fullscreen quad render data
    constexpr GLfloat quadVertices[] = {
        -1.0f, -1.0f,
        -1.0f, +1.0f,
        +1.0f, +1.0f,
        +1.0f, +1.0f,
        +1.0f, -1.0f,
        -1.0f, -1.0f
    };

    glGenVertexArrays(1, &quadVaoId);
    glBindVertexArray(quadVaoId);

    GLuint quadVboId;
    glGenBuffers(1, &quadVboId);
    glBindBuffer(GL_ARRAY_BUFFER, quadVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    
    glBindVertexArray(GL_NONE);

    // Generate and bind the texture
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 
        GL_RGB, GL_FLOAT, emulator.display.getTextureData());

    // Set the texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) { 
        // Poll events
        glfwPollEvents();

        // Set emulator keys
        updateEmulatorKeyboard();

        // Get viewport size
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        
        // Clear the window, setting to black
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render emulator and UI
        if (emulator.getRunning()) {
            int menuBarHeight = static_cast<int>(ImGui::GetFrameHeight());
            glViewport(0, 0, fbWidth, fbHeight - menuBarHeight);
            renderEmulator();
        }
        glViewport(0, 0, fbWidth, fbHeight);
        renderInterface();
        
        // Swap window buffers, updating the window
        glfwSwapBuffers(window);
    }
    
    stopEmulation();
}

void Application::updateEmulatorKeyboard() { 
    emulator.keyboard.setKeyPressed(0x0, glfwGetKey(window, GLFW_KEY_X));
    emulator.keyboard.setKeyPressed(0x1, glfwGetKey(window, GLFW_KEY_1));
    emulator.keyboard.setKeyPressed(0x2, glfwGetKey(window, GLFW_KEY_2));
    emulator.keyboard.setKeyPressed(0x3, glfwGetKey(window, GLFW_KEY_3));
    emulator.keyboard.setKeyPressed(0x4, glfwGetKey(window, GLFW_KEY_Q));
    emulator.keyboard.setKeyPressed(0x5, glfwGetKey(window, GLFW_KEY_W));
    emulator.keyboard.setKeyPressed(0x6, glfwGetKey(window, GLFW_KEY_E));
    emulator.keyboard.setKeyPressed(0x7, glfwGetKey(window, GLFW_KEY_A));
    emulator.keyboard.setKeyPressed(0x8, glfwGetKey(window, GLFW_KEY_S));
    emulator.keyboard.setKeyPressed(0x9, glfwGetKey(window, GLFW_KEY_D));
    emulator.keyboard.setKeyPressed(0xA, glfwGetKey(window, GLFW_KEY_Z));
    emulator.keyboard.setKeyPressed(0xB, glfwGetKey(window, GLFW_KEY_C));
    emulator.keyboard.setKeyPressed(0xC, glfwGetKey(window, GLFW_KEY_4));
    emulator.keyboard.setKeyPressed(0xD, glfwGetKey(window, GLFW_KEY_R));
    emulator.keyboard.setKeyPressed(0xE, glfwGetKey(window, GLFW_KEY_F));
    emulator.keyboard.setKeyPressed(0xF, glfwGetKey(window, GLFW_KEY_V));
}

#include <iostream>

void Application::renderEmulator() {
    glUseProgram(quadShader.getId());
    glBindVertexArray(quadVaoId);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(quadShader.getId(), "quadTexture"), 0);
    
    // Check if texture needs updating
    if (emulator.display.getTextureUpdated()) { 
        // Copy texture to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 
            GL_RGB, GL_FLOAT, emulator.display.getTextureData());
        emulator.display.setTextureUpdated(false);
    } else {
        // No action needed
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
    glUseProgram(GL_NONE);
}

void Application::renderInterface() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::Button("Select ROM")) { 
                stopEmulation();
                currentRomPath = selectFile();
                emulator.load(currentRomPath);
                emulatorThread = std::thread(&Emulator::run, &emulator);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Emulation")) {
            // Cpu Frequency slider
            int frequency = emulator.getCpuFrequency();
            ImGui::SliderInt("Clock Speed", &frequency, 60, 1000);
            if (frequency != emulator.getCpuFrequency()) { 
                emulator.setCpuFrequency(frequency);
            }

            // Reset button
            if (ImGui::Button("Reset")) { 
                stopEmulation();
                emulator.reset();
                emulator.load(currentRomPath);
                emulatorThread = std::thread(&Emulator::run, &emulator);
            }
            
            ImGui::EndMenu();
        }

        // Display current ROM
        float windowWidth = ImGui::GetWindowWidth();
        float textWidth = ImGui::CalcTextSize(currentRomPath.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(currentRomPath.c_str());

        ImGui::EndMainMenuBar();
    }
    

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string Application::selectFile() { 
    #ifdef WIN32
        char filename[260];   // Buffer for file name
        OPENFILENAME ofn;   // Common dialog box structure
        HWND hwnd;  // Owner window
        HANDLE hf;  // File handle
        ZeroMemory(&ofn, sizeof(ofn));
        ZeroMemory(&hwnd, sizeof(hwnd));
        ZeroMemory(&hf, sizeof(hf));
        
        // Initialize OPENFILENAME
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = filename;

        // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
        // use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(filename);
        ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Store current working directory
        wchar_t currentDir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, currentDir);

        if (GetOpenFileName(&ofn) == TRUE) {
            SetCurrentDirectoryW(currentDir);
            return std::string(filename);
        } else {
            SetCurrentDirectoryW(currentDir);
            throw std::runtime_error("Error opening file");
        }
    #endif
}

void Application::stopEmulation() { 
    if (emulator.getRunning()) {
        emulator.setRunning(false);
        emulatorThread.join();
        emulator.reset();
    }
}