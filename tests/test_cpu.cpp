#include <gtest/gtest.h>

#include "cpu.hpp"
#include "emulator.hpp"

// CPU testing is carried out through the Emulator class
// which allows the CPU to interface with the Emulator
// display and memory.

// Test opcode 0x00E0, clearing display
TEST(TestCpu, Opcode_0x00E0) {
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);

    // Fill screen with test data
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            emulator.display.setPixel(x, y, 1);
        }
    }

    // Execute opcode
    emulator.cpu.opcode = 0x00E0;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    
    // Check opcode executed successfully
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            ASSERT_EQ(emulator.display.readPixel(x, y), 0);
        }
    }
}

// Test opcode 0x1NNN, jump to address NNN
TEST(TestCpu, Opcode_0x1NNN) {
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);
    
    emulator.cpu.opcode = 0x1000;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0);
}

// Test opcode 0x6XNN, set register VX to NN
TEST(TestCpu, Opcode_0x6XNN) { 
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);
    
    emulator.cpu.opcode = 0x6AFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 0xFF);
}

// Test opcode 0x7XNN, add NN to register VX
TEST(TestCpu, Opcode_0x7XNN) { 
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);
    
    emulator.cpu.opcode = 0x7A06;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 6);

    emulator.cpu.opcode = 0x7A09;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 15);
}

// Test opcode 0xANNN, set I to NNN
TEST(TestCpu, Opcode_0xANNN) { 
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);

    emulator.cpu.opcode = 0xACCC;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.I, 0x0CCC);
}

// Test opcode DXYN, sprite at coords VX, VY
TEST(TestCpu, Opcode_0xDXYN) { 
    Emulator emulator;
    emulator.cpu.setDevice(&emulator);

    unsigned char validPatch1[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    unsigned char validPatch2[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 1, 1, 1, 1, 0,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 0
    };

    // Set coords in registers
    emulator.cpu.V[0x0] = 0;
    emulator.cpu.V[0x1] = 1;

    // Define sprite data
    emulator.cpu.I = RESERVED_MEMORY;
    emulator.memory.write(emulator.cpu.I,    0b11111111);
    emulator.memory.write(emulator.cpu.I+1,  0b11000001);
    emulator.memory.write(emulator.cpu.I+2,  0b10100001);
    emulator.memory.write(emulator.cpu.I+3,  0b10010001);
    emulator.memory.write(emulator.cpu.I+4,  0b10001001);
    emulator.memory.write(emulator.cpu.I+5,  0b10000101);
    emulator.memory.write(emulator.cpu.I+6,  0b10000011);
    emulator.memory.write(emulator.cpu.I+7,  0b11111111);

    // Draw over blank area
    emulator.cpu.opcode = 0xD008;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xF], 0);

    // Check drawn correctly
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) { 
            std::cout << x << " , " << y << std::endl;
            ASSERT_EQ(emulator.display.readPixel(x, y), validPatch1[y * 8 + x]);
        }
    }

    // Draw over occupied area
    emulator.cpu.opcode = 0xD118;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xF], 1);

    // Check drawn correctly
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) { 
            ASSERT_EQ(emulator.display.readPixel(x, y), validPatch2[y * 8 + x]);
        }
    }
}