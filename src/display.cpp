#include "display.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

void Display::init() { 
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

    createWindow();
    loadShaders();
    createRenderData();
    
    initialised = true;
}

void Display::createWindow() { 
    // create window with glfw
    window = glfwCreateWindow(640, 320, "Chip8 Emulator", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create glfw window.");
    } else { 
        // No action needed
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glfwSwapInterval(0);

    // load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad.");
    } else {
        // No action needed
    }

    glViewport(0, 0, fbWidth, fbHeight);
}

void Display::loadShaders() { 
    quadShader.create({
        {"./res/shaders/quad.vert", GL_VERTEX_SHADER},
        {"./res/shaders/quad.frag", GL_FRAGMENT_SHADER}
    });
}

void Display::createRenderData() { 
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
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_FLOAT, textureData);

    // Set the texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Display::setPixel(int px, int py, unsigned char value) { 
    if (px >= 0 && px < DISPLAY_WIDTH && py >= 0 && py < DISPLAY_HEIGHT) { 
        const int pixelIndex = py * DISPLAY_WIDTH + px;
        pixels[pixelIndex] = value;
        textureData[pixelIndex * 3] = pixels[pixelIndex] * 255.0f;
        textureData[pixelIndex * 3 + 1] = pixels[pixelIndex] * 255.0f;
        textureData[pixelIndex * 3 + 2] = pixels[pixelIndex] * 255.0f;
        pendingTextureUpdate = true;
    } else { 
        throw std::out_of_range("Display location invalid: " + 
            std::to_string(px) + "," + std::to_string(py));
    }
}

unsigned char Display::readPixel(int px, int py) { 
    if (px >= 0 && px < DISPLAY_WIDTH && py >= 0 && py < DISPLAY_HEIGHT) { 
        return pixels[py * DISPLAY_WIDTH + px];
    } else { 
        throw std::out_of_range("Display location invalid: " + 
            std::to_string(px) + "," + std::to_string(py));
    }
}

void Display::clear() { 
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) { 
        pixels[i] = 0;
    }
}

void Display::update() { 
    glfwPollEvents();

    // Resize viewport
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    
    // Clear the window, setting to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the pixels to window back buffer
    renderPixels();
    
    // Swap window buffers, updating the window
    glfwSwapBuffers(window);
}

void Display::renderPixels() {
    glUseProgram(quadShader.getId());
    glBindVertexArray(quadVaoId);

    glUniform1i(0, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Check if texture needs updating
    if (pendingTextureUpdate) { 
        // Copy texture to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RGB, GL_FLOAT, textureData);
        pendingTextureUpdate = false;
    } else {
        // No action needed
    }
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(GL_NONE);
    glUseProgram(GL_NONE);
}

Display::~Display() { 
    if (initialised) { 
        // Clean resources
        glDeleteVertexArrays(1, &quadVaoId);
        glfwDestroyWindow(window);
        glfwTerminate();
    } else {
        // No action needed
    }
}