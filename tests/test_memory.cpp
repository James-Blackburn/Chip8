#include <gtest/gtest.h>
#include <filesystem>

#include "memory.hpp"

TEST(MemoryTest, HandlesValidLocations) { 
    Memory memory;
    
    // Test memory writes
    EXPECT_NO_THROW(memory.write(RESERVED_MEMORY, 128));
    EXPECT_NO_THROW(memory.write(ADDRESSABLE_MEMORY - 1, 128));

    // Test memory reads
    EXPECT_EQ(memory.read(RESERVED_MEMORY), 128);
    EXPECT_EQ(memory.read(ADDRESSABLE_MEMORY - 1), 128);
}

TEST(MemoryTest, HandlesInvalidLocations) { 
    Memory memory;

    // Test memory writes
    EXPECT_THROW(memory.write(-1, 128), std::out_of_range);
    EXPECT_THROW(memory.write(RESERVED_MEMORY - 1, 128), std::out_of_range);
    EXPECT_THROW(memory.write(ADDRESSABLE_MEMORY, 128), std::out_of_range);

    // Test memory reads
    EXPECT_THROW(memory.read(-1), std::out_of_range);
    EXPECT_THROW(memory.read(ADDRESSABLE_MEMORY), std::out_of_range);
}

TEST(MemoryTest, CheckFontIntegrity) { 
    Memory memory;

    const unsigned char validFontData[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    for (int i = FONT_START; i < FONT_END; i++) { 
        ASSERT_EQ(validFontData[i - FONT_START], memory.read(i));
    }
}

TEST(MemoryTest, HandlesLoadingPrograms) { 
    Memory memory;
    
    const int validTestProgramSize = 133;
    const unsigned char validTestProgram[validTestProgramSize] = {
        0x00, 0xE0, 0xA2, 0x2A, 0x60, 0x0C, 0x61, 0x08, 0xD0, 0x1F, 0x70, 0x09,
        0xA2, 0x39, 0xD0, 0x1F, 0xA2, 0x48, 0x70, 0x08, 0xD0, 0x1F, 0x70, 0x04,
        0xA2, 0x57, 0xD0, 0x1F, 0x70, 0x08, 0xA2, 0x66, 0xD0, 0x1F, 0x70, 0x08,
        0xA2, 0x75, 0xD0, 0x1F, 0x12, 0x28, 0xFF, 0x00, 0xFF, 0x00, 0x3C, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
        0x00, 0x38, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x38, 0x00, 0xFF, 0x00, 0xFF,
        0x80, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00,
        0xE0, 0x00, 0x80, 0xF8, 0x00, 0xFC, 0x00, 0x3E, 0x00, 0x3F, 0x00, 0x3B,
        0x00, 0x39, 0x00, 0xF8, 0x00, 0xF8, 0x03, 0x00, 0x07, 0x00, 0x0F, 0x00,
        0xBF, 0x00, 0xFB, 0x00, 0xF3, 0x00, 0xE3, 0x00, 0x43, 0xE0, 0x00, 0xE0,
        0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00, 0xE0,
        0x0A
    };

    
    // Test loading program
    std::cout << std::filesystem::current_path() << std::endl;
    ASSERT_NO_THROW(memory.loadProgram("./test_data/ibm.ch8")) << "Failed to load program into memory";

    // Validate data
    for (int i = 0; i < validTestProgramSize; i++) { 
        ASSERT_EQ(validTestProgram[i], memory.read(i + RESERVED_MEMORY)) << "Memory integrity check failed";
    }
}