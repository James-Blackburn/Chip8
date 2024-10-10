#include "emulator.hpp"

#include <iostream>
#include <random>
#include <ctime>
#include <thread>

Emulator::Emulator() {
    cpu.setDevice(this);
    keyboard.setDevice(this);
}

void Emulator::load(std::string romPath) { 
    std::srand(static_cast<unsigned int>(std::time(0)));
    memory.loadProgram(romPath);
    cpu.setProgramCounter(0x200);
    display.init();
}

void Emulator::reset() {
    cpu.reset();
    memory.reset();
    display.clear();
}

void Emulator::run() { 
    int cycleCounter = 0;

    while (display.isShowing()) { 
        auto cycleStartTime = std::chrono::high_resolution_clock::now();

        display.pollEvents();

        // Simulate CPU FDE cycle
        cpu.fetchOpcode();
        cpu.executeOpcode();

        // Update display and countdown timers every 60hz
        if (cycleCounter % CYCLES_PER_DISPLAY_UPDATE == 0) {
            cpu.countdownTimers(1);
            display.update();
            cycleCounter = 0;
        }
        cycleCounter++;

        // Limit FDE cycle to 540hz
        auto cycleEndTime = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds waitDuration = TARGET_CYCLE_DURATION - (cycleEndTime - cycleStartTime);
        while (std::chrono::high_resolution_clock::now() < cycleEndTime + waitDuration){}
    }

    display.destroy();
}