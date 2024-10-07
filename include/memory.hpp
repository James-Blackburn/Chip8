#pragma once

#include <string>

#define MEMORY_SIZE 4096

class Memory {
    private:
        unsigned char memory[MEMORY_SIZE];
    public:
        void loadProgram(const std::string& programLocation);
        unsigned char read(int location);
        void write(int location, unsigned char data);
};