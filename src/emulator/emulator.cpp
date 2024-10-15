#include "emulator.hpp"

#include <iostream>
#include <random>
#include <ctime>
#include <thread>

Emulator::Emulator() {
    cpu.setDevice(this);
    speaker.init();
}

void Emulator::load(std::string romPath) { 
    std::srand(static_cast<unsigned int>(std::time(0)));
    memory.loadProgram(romPath);
}

void Emulator::reset() {
    cpu.reset();
    memory.reset();
    keyboard.reset();
    display.clear();
    speaker.stopBeep();
}

void Emulator::run() { 
    running = true;
    int cycleCounter = 0;

    while (running) { 
        auto cycleStartTime = std::chrono::high_resolution_clock::now();

        // Simulate CPU FDE cycle
        cpu.fetchOpcode();
        cpu.executeOpcode();

        // Countdown timers every 60hz
        if (cycleCounter % (cpuFrequency / 60) == 0) {
            cpu.countdownTimers();
            cycleCounter = 0;
        }
        cycleCounter++;

        // Limit FDE cycle to 540hz
        auto cycleEndTime = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds waitDuration = std::chrono::microseconds(1000000 / cpuFrequency) - (cycleEndTime - cycleStartTime);
        while (std::chrono::high_resolution_clock::now() < cycleEndTime + waitDuration){}
    }
}