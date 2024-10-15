#include "application.hpp"

#include <iostream>

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

    Application app;
    app.run();
    
    return 0;
}