#include <iostream>

#include "memory.hpp"
#include "display.hpp"

int main() {
    Memory memory;
    Display display;

    display.init();
    display.setPixel(20, 20, 1);
    display.setPixel(20, 21, 1);
    display.setPixel(20, 22, 1);
    while (display.isShowing()) { 
        display.update();
    }

    return 0;
}