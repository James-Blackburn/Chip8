#pragma once

class Emulator;

#include <string>

class Cpu { 
    private:
        Emulator* device;
        
        // Program counter
        int pc = 0x200;

        // Registers
        unsigned short opcode;
        unsigned short I;
        unsigned char V[16]{0};

        std::string opcodeErrorMsg();

        // friend classes for testing
        friend class TestCpu_Opcode_0x00E0_Test;
        friend class TestCpu_Opcode_0x1NNN_Test;
        friend class TestCpu_Opcode_0x6XNN_Test;
        friend class TestCpu_Opcode_0x7XNN_Test;
        friend class TestCpu_Opcode_0xANNN_Test;
        friend class TestCpu_Opcode_0xDXYN_Test;

    public:
        void fetchOpcode();
        void executeOpcode();

        inline void setDevice(Emulator* emulator);
        inline void setProgramCounter(int value);
        inline int getProgramCounter();
};

void Cpu::setDevice(Emulator* emulator) {
    device = emulator;
}

void Cpu::setProgramCounter(int value) { 
    pc = value;
}

int Cpu::getProgramCounter() { 
    return pc;
}