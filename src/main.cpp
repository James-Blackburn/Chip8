#include <iostream>

#include "memory.hpp"
#include "display.hpp"

int main() {
    Memory memory;
    Display display;

    display.init();
    display.setPixel(20, 20, 255);
    display.setPixel(20, 21, 128);
    display.setPixel(20, 22, 64);
    while (display.isShowing()) { 
        display.update();
    }

    return 0;
}