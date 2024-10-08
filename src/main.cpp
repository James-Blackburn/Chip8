#include <iostream>

#include "emulator.hpp"

void terminateHandler() {
    try {
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Uncaught exception: " << e.what() << std::endl;
    }
    std::abort();
}

int main() {
    std::set_terminate(terminateHandler);
    
    Emulator chip8;
    chip8.init();
    chip8.run();
    
    return 0;
}