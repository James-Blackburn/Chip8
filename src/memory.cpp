#include "memory.hpp"

#include <fstream>
#include <filesystem>
#include <stdexcept>

void Memory::loadProgram(const std::string& programLocation) { 
    // Check size of file
    uintmax_t fileSize = std::filesystem::file_size(programLocation);
    if (fileSize > PROGRAM_MEMORY) { 
        throw std::runtime_error("Too large to load. Failed to read ROM: " + programLocation);
    }

    // Open file
    std::ifstream file(programLocation, std::ios::binary);
    if (!file.is_open()) { 
        throw std::runtime_error("Failed to open ROM: " + programLocation);
    } 

    // Read file into a temporary buffer
    char buffer[PROGRAM_MEMORY];
    if (!file.read(buffer, fileSize)) {
        throw std::runtime_error("Failed to read ROM: " + programLocation);
    }
    file.close();

    // load buffer into memory after address 0x200 (512)
    for (int i = 0; i < fileSize; i++) { 
        memory[i + RESERVED_MEMORY] = buffer[i];
    }
}

unsigned char Memory::read(int location) { 
    if (location > -1 && location < ADDRESSABLE_MEMORY) {
        return memory[location];
    } else {
        throw std::out_of_range("Memory location invalid: " + std::to_string(location));
    }
}

void Memory::write(int location, unsigned char data) {
    if (location >= RESERVED_MEMORY && location < ADDRESSABLE_MEMORY) {
        memory[location] = data;
    } else {
        throw std::out_of_range("Memory location invalid: " + std::to_string(location));
    }
}