#include <gtest/gtest.h>

#include "cpu.hpp"
#include "emulator.hpp"

#include <random>

// CPU testing is carried out through the Emulator class
// which allows the CPU to interface with the Emulator
// display and memory.

// Test opcode 0x00E0, clearing display
TEST(TestCpu, Opcode_0x00E0) {
    Emulator emulator;

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

// Test opcode 0x00EE, return from subroutine
TEST(TestCpu, Opcode_0x00EE) {
    Emulator emulator;

    // Populate stack with test address
    emulator.cpu.stack[0] = 0x0050;
    emulator.cpu.sp = 1;

    emulator.cpu.opcode = 0x00EE;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0050);
    ASSERT_EQ(emulator.cpu.sp, 0);
}

// Test opcode 0x1NNN, jump to address NNN
TEST(TestCpu, Opcode_0x1NNN) {
    Emulator emulator;
    emulator.cpu.opcode = 0x1000;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0);
}

// Test opcode 0x2NNN, call subroutine at NNN
TEST(TestCpu, Opcode_0x2NNN) { 
    Emulator emulator;
    emulator.cpu.opcode = 0x2202;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0202);
    ASSERT_EQ(emulator.cpu.stack[0], 0x0200);
    ASSERT_EQ(emulator.cpu.sp, 1);
}

// Test opcode 0x3XNN, skip next if VX == NN
TEST(TestCpu, Opcode_0x3XNN) { 
    Emulator emulator;
    emulator.cpu.V[0x0] = 0xFF;
    emulator.cpu.opcode = 0x30FF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0202);
}

// Test opcode 0x4XNN, skip next if VX != NN
TEST(TestCpu, Opcode_0x4XNN) { 
    Emulator emulator;
    emulator.cpu.V[0x0] = 0x0;
    emulator.cpu.opcode = 0x40FF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0202);
}

// Test opcode 0x5XY0, skip next if VX == VY
TEST(TestCpu, Opcode_0x5XY0) { 
    Emulator emulator;
    emulator.cpu.V[0x0] = 0xFF;
    emulator.cpu.V[0x1] = 0xFF;
    emulator.cpu.opcode = 0x5010;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0202);
}

// Test opcode 0x6XNN, set register VX to NN
TEST(TestCpu, Opcode_0x6XNN) { 
    Emulator emulator;
    emulator.cpu.opcode = 0x6AFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 0xFF);
}

// Test opcode 0x7XNN, add NN to register VX
TEST(TestCpu, Opcode_0x7XNN) { 
    Emulator emulator;

    emulator.cpu.V[0xA] = 0;
    
    emulator.cpu.opcode = 0x7A06;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 6);

    emulator.cpu.opcode = 0x7A09;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0xA], 15);
}

// Test opcode 0x8XY0, set VX to VY
TEST(TestCpu, Opcode_0x8XY0) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8010;
    emulator.cpu.V[0x1] = 0xFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0], 0xFF);
}

// Test opcode 0x8XY1, set VX to VX | VY
TEST(TestCpu, Opcode_0x8XY1) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8011;
    emulator.cpu.V[0x0] = 0x01;
    emulator.cpu.V[0x1] = 0x02;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0], 0x01 | 0x02);
}

// Test opcode 0x8XY2, set VX to VY & VY
TEST(TestCpu, Opcode_0x8XY2) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8012;
    emulator.cpu.V[0x0] = 0x01;
    emulator.cpu.V[0x1] = 0x02;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0], 0x01 & 0x02);
}

// Test opcode 0x8XY3, set VX to VX ^ VY
TEST(TestCpu, Opcode_0x8XY3) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8013;
    emulator.cpu.V[0x0] = 0x01;
    emulator.cpu.V[0x1] = 0x02;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0], 0x01 ^ 0x02);
}

// Test opcode 0x8XY4, add VY to VX
TEST(TestCpu, Opcode_0x8XY4) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8014;

    // Check addition without overflow
    emulator.cpu.V[0x0] = 0x01;
    emulator.cpu.V[0x1] = 0x02;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0], 0x03) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 0) << "Incorrect carry flag";

    // Check addition with overflow
    emulator.cpu.V[0x0] = 0xFF;
    emulator.cpu.V[0x1] = 0xFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0xFE) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 1) << "Incorrect carry flag";
}

// Test opcode 0x8XY5, subtract VY from VX
TEST(TestCpu, Opcode_0x8XY5) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8015;

    // Check subtraction without underflow
    emulator.cpu.V[0x0] = 0x07;
    emulator.cpu.V[0x1] = 0x03;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0], 0x04) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 1) << "Incorrect no borrow flag";
    
    // Check subtraction with underflow
    emulator.cpu.V[0x0] = 0x03;
    emulator.cpu.V[0x1] = 0x07;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0xFC) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 0) << "Incorrect no borrow flag";
}

// Test opcode 0x8XY6, shift VX right by 1
TEST(TestCpu, Opcode_0x8XY6) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8016;
    emulator.cpu.V[0x0] = 0b10101010;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0b01010101) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 0) << "Incorrect LSB stored";
}

// Test opcode 0x8XY7, set VX to VY - VX
TEST(TestCpu, Opcode_0x8XY7) {
    Emulator emulator;
    emulator.cpu.opcode = 0x8017;

    // Test subtraction with no underflow
    emulator.cpu.V[0x0] = 0x07;
    emulator.cpu.V[0x1] = 0x09;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0x02) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 1) << "Incorrect no borrow flag";

    // Test subtraction with underflow
    emulator.cpu.V[0x0] = 0x09;
    emulator.cpu.V[0x1] = 0x07;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0xFE) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 0) << "Incorrect no borrow flag";
}

// Test opcode 0x8XYE, shift VX left by 1
TEST(TestCpu, Opcode_0x8XYE) {
    Emulator emulator;
    emulator.cpu.opcode = 0x801E;
    emulator.cpu.V[0x0] = 0b10101010;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.cpu.V[0x0], 0b01010100) << "Incorrect result";
    EXPECT_EQ(emulator.cpu.V[0xF], 1) << "Incorrect MSB stored";
}

// Test opcode 0x9XY0, skip next if VX != VY
TEST(TestCpu, Opcode_0x9XY0) {
    Emulator emulator;
    emulator.cpu.opcode = 0x9010;

    // Check no skip when equal
    emulator.cpu.pc = 0x0200;
    emulator.cpu.V[0x0] = 0;
    emulator.cpu.V[0x1] = 0;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0200) << "Incorrect, PC skipped instruction";

    // Check skip when unequal
    emulator.cpu.pc = 0x0200;
    emulator.cpu.V[0x0] = 0;
    emulator.cpu.V[0x1] = 1;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0202) << "Incorrect, PC did not skip instruction";
}

// Test opcode 0xANNN, set I to NNN
TEST(TestCpu, Opcode_0xANNN) { 
    Emulator emulator;
    emulator.cpu.opcode = 0xACCC;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.I, 0x0CCC) << "I set to wrong value";
}

// Test opcode 0xBNNN, jump to NNN + V0
TEST(TestCpu, Opcode_0xBNNN) {
    Emulator emulator;
    emulator.cpu.opcode = 0xB220;
    emulator.cpu.V[0x0] = 0x20;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.pc, 0x0240) << "Jumped to wrong address";
}

// Test opcode 0xCXNN, Set VX to rand() & NN
TEST(TestCpu, Opcode_0xCXNN) {
    Emulator emulator;
    emulator.cpu.opcode = 0xC007;
    
    // Calculate result
    std::srand(0);
    int validResult = (std::rand() % 255) & 0x07;
    
    // Reset random sequence before testing
    std::srand(0);
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0], validResult);
}

// Test opcode 0xDXYN, sprite at coords VX, VY
TEST(TestCpu, Opcode_0xDXYN) { 
    Emulator emulator;

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

// Test opcode 0xFX07, VX = delay
TEST(TestCpu, Opcode_0xFX07) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF007;
    emulator.cpu.delayTimer = 0xFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.V[0x0], 0xFF) << "VX set to wrong value";
}

// Test opcode 0xFX15, set delay = VX
TEST(TestCpu, Opcode_0xFX15) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF015;
    emulator.cpu.V[0x0] = 0xFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.delayTimer, 0xFF) << "Delay set to wrong value";
}

// Test opcode 0xFX18, set sound = VX
TEST(TestCpu, Opcode_0xFX18) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF018;
    emulator.cpu.V[0x0] = 0xFF;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.soundTimer, 0xFF) << "Sound set to wrong value";
}

// Test opcode 0xFX1E, add VX to I
TEST(TestCpu, Opcode_0xFX1E) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF01E;
    emulator.cpu.I = 0x03;
    emulator.cpu.V[0x0] = 0x04;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.I, 0x07) << "I set to wrong value"; 
}

// Test opcode 0xFX29, set I to location of character VX
TEST(TestCpu, Opcode_0xFX29) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF029;
    emulator.cpu.V[0x0] = 0x5;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    ASSERT_EQ(emulator.cpu.I, 25) << "I at wrong character address";
    ASSERT_EQ(emulator.memory.read(emulator.cpu.I), 0xF0) << "Incorrect font at I";
}

// Test opcode 0xFX33, store decimal representation of VX at I
TEST(TestCpu, Opcode_0xFX33) {
    Emulator emulator;
    emulator.cpu.opcode = 0xF033;
    emulator.cpu.V[0x0] = 128;
    emulator.cpu.I = RESERVED_MEMORY;
    ASSERT_NO_THROW(emulator.cpu.executeOpcode());
    EXPECT_EQ(emulator.memory.read(emulator.cpu.I), 1) << "Incorrect hundreds digit at I";
    EXPECT_EQ(emulator.memory.read(emulator.cpu.I + 1), 2) << "Incorrect tens digit at I+1";
    EXPECT_EQ(emulator.memory.read(emulator.cpu.I + 2), 8) << "Incorrect ones digit at I+2";
}

// Test opcode 0xFX55, dump registers V0-VX in memory starting at I
TEST(TestCpu, Opcode_0xFX55) {
    Emulator emulator;
    emulator.cpu.opcode = 0xFF55;
    emulator.cpu.I = RESERVED_MEMORY;
    
    // Fill registers
    for (int i = 0; i < 15; i++) {
        emulator.cpu.V[i] = i;
    }

    ASSERT_NO_THROW(emulator.cpu.executeOpcode());

    // Check memory
    for (int i = 0; i < 15; i++) { 
        ASSERT_EQ(emulator.memory.read(emulator.cpu.I + i), i);
    }
}

// Test opcode 0xFX65, load registers V0-VX from memory starting at I
TEST(TestCpu, Opcode_0xFX65) {
    Emulator emulator;
    emulator.cpu.opcode = 0xFF65;
    emulator.cpu.I = RESERVED_MEMORY;

    // Fill memory
    for (int i = 0; i <= 15; i++) {
        emulator.memory.write(emulator.cpu.I + i, i);
    }

    ASSERT_NO_THROW(emulator.cpu.executeOpcode());

    // Check registers
    for (int i = 0; i <= 15; i++) { 
        ASSERT_EQ(emulator.cpu.V[i], i);
    }
}