#pragma once

#include "cpu.hpp"
#include "memory.hpp"
#include "display.hpp"
#include "keyboard.hpp"
#include "speaker.hpp"

#include <chrono>
#include <string>
#include <atomic>

class Emulator { 
    private:
        std::atomic<bool> running = false;
        std::atomic<int> cpuFrequency = 540;
        
    public:
        Cpu cpu;
        Memory memory;
        Display display;
        Keyboard keyboard;
        Speaker speaker;

        Emulator();

        void load(std::string romPath);
        void run();
        void reset();
        
        inline bool getRunning() const;
        inline void setRunning(bool status);
        inline int getCpuFrequency() const;
        inline void setCpuFrequency(int frequency);
};

bool Emulator::getRunning() const {
    return running;
}

void Emulator::setRunning(bool status) { 
    running = status;
}

int Emulator::getCpuFrequency() const {
    return cpuFrequency;
}

void Emulator::setCpuFrequency(int frequency) { 
    cpuFrequency = frequency;
}