#include "emulator.hpp"

#include <random>
#include <ctime>

Emulator::Emulator() {
    cpu.setDevice(this);
    std::srand(static_cast<unsigned int>(std::time(0)));
}

void Emulator::init() { 
    display.init();
    //memory.loadProgram("res/rom/ibm.ch8");
    //memory.loadProgram("res/rom/Particle Demo [zeroZshadow, 2008].ch8");
    //memory.loadProgram("res/rom/Maze [David Winter, 199x].ch8");
    //memory.loadProgram("res/rom/Zero Demo [zeroZshadow, 2007].ch8");
    //memory.loadProgram("res/rom/Space Invaders [David Winter].ch8");
    //memory.loadProgram("res/rom/danm8ku.ch8");
    memory.loadProgram("res/rom/octopeg.ch8");
    cpu.setProgramCounter(0x200);
}

void Emulator::run() { 
    while (display.isShowing()) { 
        // Simulate CPU FDE cycle
        cpu.fetchOpcode();
        cpu.executeOpcode();

        display.update();
    }
}