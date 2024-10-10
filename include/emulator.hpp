#pragma once

#include "cpu.hpp"
#include "memory.hpp"
#include "display.hpp"
#include "keyboard.hpp"

#include <chrono>

class Emulator { 
    private:
        const int CPU_FREQUENCY = 540; // hz
        const int DISPLAY_FREQUENCY = 60; // hz
        const std::chrono::microseconds TARGET_CYCLE_DURATION = std::chrono::microseconds(1000000 / CPU_FREQUENCY);
        const int CYCLES_PER_DISPLAY_UPDATE = CPU_FREQUENCY / DISPLAY_FREQUENCY;
    public:
        Cpu cpu;
        Memory memory;
        Display display;
        Keyboard keyboard;

        Emulator();

        void init();
        void run();
};