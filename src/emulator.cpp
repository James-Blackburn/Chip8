#include "emulator.hpp"

void Emulator::init() { 
    display.init();
    memory.loadProgram("res/rom/ibm.ch8");
    cpu.setProgramCounter(0x200);
    cpu.setDevice(this);
}

void Emulator::run() { 
    while (display.isShowing()) { 
        // Simulate CPU FDE cycle
        cpu.fetchOpcode();
        cpu.executeOpcode();

        display.update();
    }
}