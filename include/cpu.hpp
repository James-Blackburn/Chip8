#pragma once

class Emulator;

#include <string>

class Cpu { 
    private:
        Emulator* device;
        
        // Program counter
        int pc = 0x200;

        // Registers
        unsigned short opcode = 0;
        unsigned short I = 0;
        unsigned char V[16]{0};
        unsigned short stack[16]{0};
        unsigned char sp = 0;
        unsigned char delayTimer = 0;
        unsigned char soundTimer = 0;

        std::string opcodeErrorMsg();

        // friend classes for testing
        friend class TestCpu_Opcode_0x00E0_Test;
        friend class TestCpu_Opcode_0x00EE_Test;
        friend class TestCpu_Opcode_0x1NNN_Test;
        friend class TestCpu_Opcode_0x2NNN_Test;
        friend class TestCpu_Opcode_0x3XNN_Test;
        friend class TestCpu_Opcode_0x4XNN_Test;
        friend class TestCpu_Opcode_0x5XY0_Test;
        friend class TestCpu_Opcode_0x6XNN_Test;
        friend class TestCpu_Opcode_0x7XNN_Test;
        friend class TestCpu_Opcode_0x8XY0_Test;
        friend class TestCpu_Opcode_0x8XY1_Test;
        friend class TestCpu_Opcode_0x8XY2_Test;
        friend class TestCpu_Opcode_0x8XY3_Test;
        friend class TestCpu_Opcode_0x8XY4_Test;
        friend class TestCpu_Opcode_0x8XY5_Test;
        friend class TestCpu_Opcode_0x8XY6_Test;
        friend class TestCpu_Opcode_0x8XY7_Test;
        friend class TestCpu_Opcode_0x8XYE_Test;
        friend class TestCpu_Opcode_0xANNN_Test;
        friend class TestCpu_Opcode_0xBNNN_Test;
        friend class TestCpu_Opcode_0xCXNN_Test;
        friend class TestCpu_Opcode_0xDXYN_Test;
        friend class TestCpu_Opcode_0xEX9E_Test;
        friend class TestCpu_Opcode_0xEXA1_Test;
        friend class TestCpu_Opcode_0xFX07_Test;
        friend class TestCpu_Opcode_0xFX0A_Test;
        friend class TestCpu_Opcode_0xFX15_Test;
        friend class TestCpu_Opcode_0xFX18_Test;
        friend class TestCpu_Opcode_0xFX1E_Test;
        friend class TestCpu_Opcode_0xFX29_Test;
        friend class TestCpu_Opcode_0xFX33_Test;
        friend class TestCpu_Opcode_0xFX55_Test;
        friend class TestCpu_Opcode_0xFX65_Test;

    public:
        void fetchOpcode();
        void executeOpcode();
        void countdownTimers(int amount);

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