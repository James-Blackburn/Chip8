#pragma once

#include "cpu.hpp"
#include "memory.hpp"
#include "display.hpp"
#include "keyboard.hpp"

#include <chrono>
#include <string>

// Frequencies are in hz
#define CPU_FREQUENCY 540
#define DISPLAY_FREQUENCY 60

class Emulator { 
    private:
        const std::chrono::microseconds TARGET_CYCLE_DURATION = std::chrono::microseconds(1000000 / CPU_FREQUENCY);
        const int CYCLES_PER_DISPLAY_UPDATE = CPU_FREQUENCY / DISPLAY_FREQUENCY;
    public:
        Cpu cpu;
        Memory memory;
        Display display;
        Keyboard keyboard;

        Emulator();

        void load(std::string romPath);
        void run();
        void reset();
};