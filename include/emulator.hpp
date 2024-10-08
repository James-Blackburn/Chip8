#pragma once

#include "cpu.hpp"
#include "memory.hpp"
#include "display.hpp"

class Emulator { 
    private:
    public:
        Cpu cpu;
        Memory memory;
        Display display;

        void init();
        void run();
};