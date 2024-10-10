#include "emulator.hpp"

#include <iostream>
#include <random>
#include <ctime>
#include <thread>

Emulator::Emulator() {
    cpu.setDevice(this);
    std::srand(static_cast<unsigned int>(std::time(0)));
}

void Emulator::init() { 
    display.init();
    //memory.loadProgram("res/rom/ibm.ch8");
    //memory.loadProgram("res/rom/Particle Demo [zeroZshadow, 2008].ch8");
    memory.loadProgram("res/rom/Maze [David Winter, 199x].ch8");
    //memory.loadProgram("res/rom/Zero Demo [zeroZshadow, 2007].ch8");
    //memory.loadProgram("res/rom/Space Invaders [David Winter].ch8");
    //memory.loadProgram("res/rom/danm8ku.ch8");
    //memory.loadProgram("res/rom/octopeg.ch8");
    cpu.setProgramCounter(0x200);
}

void Emulator::run() { 
    int cycleCounter = 0;

    while (display.isShowing()) { 
        auto cycleStartTime = std::chrono::high_resolution_clock::now();

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
}