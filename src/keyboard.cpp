#include "keyboard.hpp"

#include "emulator.hpp"

#include <stdexcept>
#include <map>

void Keyboard::setDevice(Emulator* emulator) { 
    device = emulator;
}

bool Keyboard::getKeyPressed(unsigned char key) {
    if (key >= 0 && key < 16){ 
        return glfwGetKey(device->display.window, mapKey(key));
    } else {
        throw std::out_of_range("Invalid key requested: " + std::to_string(+key));
    }
}

int Keyboard::mapKey(unsigned char hexKey) {
    static const std::map<unsigned char, int> keyboardLayout = {
        {0x1, GLFW_KEY_1}, {0x2, GLFW_KEY_2}, {0x3, GLFW_KEY_3}, {0xC, GLFW_KEY_4},
        {0x4, GLFW_KEY_Q}, {0x5, GLFW_KEY_W}, {0x6, GLFW_KEY_E}, {0xD, GLFW_KEY_R},
        {0x7, GLFW_KEY_A}, {0x8, GLFW_KEY_S}, {0x9, GLFW_KEY_D}, {0xE, GLFW_KEY_F},
        {0xA, GLFW_KEY_Z}, {0x0, GLFW_KEY_X}, {0xB, GLFW_KEY_C}, {0xF, GLFW_KEY_V}
    };
    return keyboardLayout.at(hexKey);
}