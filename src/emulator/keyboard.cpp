#include "keyboard.hpp"

#include <stdexcept>
#include <string>

void Keyboard::reset() { 
    for (int i = 0; i < 16; i++) { 
        keys[i] = false;
    }
}

bool Keyboard::getKeyPressed(unsigned char key) const {
    if (key >= 0 && key < 16){ 
        return keys[key];
    } else {
        throw std::out_of_range("Invalid key requested: " + std::to_string(+key));
    }
}

void Keyboard::setKeyPressed(unsigned char key, bool value) { 
    if (key >= 0 && key < 16){ 
        keys[key] = value;
    } else {
        throw std::out_of_range("Invalid key set: " + std::to_string(+key));
    }
}