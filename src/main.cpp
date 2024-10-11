#include <iostream>
#include <filesystem>
#include <string>
#include <map>

#include "emulator.hpp"
#include "speaker.hpp"

#include <thread>
#include <chrono>

void terminateHandler() {
    try {
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Uncaught exception: " << e.what() << std::endl;
    }
    std::abort();
}

std::string selectRom() {
    std::string romPath = "./res/rom/";
    std::filesystem::path rootDir = romPath;
    std::map<int, std::filesystem::directory_entry> directories;
    std::map<int, std::filesystem::directory_entry> roms;
    
    // Find Directories
    int i = 0;
    std::cout << "DIRECTORIES" << std::endl;
    for (const auto& dirEntry : std::filesystem::directory_iterator(rootDir)) {
        if (dirEntry.is_directory()) {
            std::cout << i << " : " << dirEntry.path().filename().string() << std::endl;
            directories[i] = dirEntry;
            i++;
        }
    }

    // Select Directory
    int option;
    while (1) {
        std::cout << "Select Directory: " << std::endl;
        std::cin >> option;
        try {
            romPath += directories.at(option).path().filename().string() + "/";
            break;
        } catch (...) {
            std::cout << "Invalid option" << std::endl;
        }
    }

    // Find ROMS
    i = 0;
    std::cout << "ROMS" << std::endl;
    for (const auto& romEntry : std::filesystem::directory_iterator(directories.at(option))) {
        if (romEntry.is_regular_file() && romEntry.path().extension() == ".ch8") {
            std::cout << i << " : " << romEntry.path().filename().string() << std::endl;
            roms[i] = romEntry;
            i++;
        }
    }

    // Select ROM
    while (1) {
        std::cout << "Select ROM: " << std::endl;
        std::cin >> option;
        try {
            romPath += roms.at(option).path().filename().string();
            break;
        } catch (...) {
            std::cout << "Invalid option" << std::endl;
        }
    }

    return romPath;
}

int main() {
    std::set_terminate(terminateHandler);

    std::cout << "Chip8 Emulator\n" << std::endl;

    Emulator chip8;
    while (1) {
        chip8.load(selectRom());
        chip8.run();
        chip8.reset();
    }
    
    return 0;
}