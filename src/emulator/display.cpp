#include "display.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

void Display::setPixel(int px, int py, unsigned char value) { 
    if (px >= 0 && px < DISPLAY_WIDTH && py >= 0 && py < DISPLAY_HEIGHT) { 
        const int pixelIndex = py * DISPLAY_WIDTH + px;
        pixels[pixelIndex] = value;
        textureData[pixelIndex * 3] = pixels[pixelIndex] * 255.0f;
        textureData[pixelIndex * 3 + 1] = pixels[pixelIndex] * 255.0f;
        textureData[pixelIndex * 3 + 2] = pixels[pixelIndex] * 255.0f;
        textureUpdated = true;
    } else { 
        throw std::out_of_range("Display location invalid: " + 
            std::to_string(px) + "," + std::to_string(py));
    }
}

unsigned char Display::readPixel(int px, int py) const { 
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
        textureData[i * 3] = 0.0f;
        textureData[i * 3 + 1] = 0.0f;
        textureData[i * 3 + 2] = 0.0f;
        textureUpdated = true;
    }
}