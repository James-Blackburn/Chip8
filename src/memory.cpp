#include "memory.hpp"

#include <fstream>
#include <filesystem>
#include <stdexcept>

void Memory::loadProgram(const std::string& programLocation) { 
    std::ifstream file(programLocation, std::ios::binary);
    if (!file.is_open()) { 
        throw std::runtime_error("Failed to open file: " + programLocation);
    }

    uintmax_t fileSize = std::filesystem::file_size(programLocation);

    if (!file.read(reinterpret_cast<char*>(memory), fileSize)) {
        throw std::runtime_error("Failed to read file: " + programLocation);
    }

    file.close();
}

unsigned char Memory::read(int location) { 
    if (location > -1 && location < MEMORY_SIZE) {
        return memory[location];
    } else {
        throw std::out_of_range("Memory location out of range: " + std::to_string(location));
    }
}

void Memory::write(int location, unsigned char data) {
    if (location > -1 && location < MEMORY_SIZE) {
        memory[location] = data;
    } else {
        throw std::out_of_range("Memory location out of range: " + std::to_string(location));
    }
}