#include "cpu.hpp"
#include "emulator.hpp"

#include <sstream>
#include <iostream>
#include <random>

std::string Cpu::opcodeErrorMsg() { 
    std::stringstream ss;
    ss << "Unknown opcode: " << std::hex << opcode;
    return ss.str();
}

void Cpu::countdownTimers() {
    if (delayTimer > 0) {
        delayTimer--;
    } else {
        // No action needed
    }
    if (soundTimer > 0) {
        device->speaker.playBeep();
        soundTimer--;
    } else {
        // Stop sound
        device->speaker.stopBeep();
    }
}

void Cpu::reset() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;
    
    for (int i = 0; i < 16; i++) {
        V[i] = 0;
        stack[i] = 0;
    }
}

void Cpu::fetchOpcode() { 
    // Construct opcode
    opcode = static_cast<unsigned short>(device->memory.read(pc)) << 8 | device->memory.read(pc + 1);
    pc += 2;
}

void Cpu::executeOpcode() { 
    switch (opcode & 0xF000) { 
        case 0x0000: {
            switch (opcode & 0x00FF) {
                case 0x00E0: {
                    // Clear display
                    device->display.clear();
                    break;
                } case 0x00EE: {
                    // Return from subroutine
                    pc = stack[--sp];
                    break;
                } default: {
                    throw std::runtime_error(opcodeErrorMsg());
                }
            }
            break;
        } case 0x1000: {
            // Jump to NNN
            pc = opcode & 0x0FFF;
            break;
        } case 0x2000: {
            // Call subroutine at NNN
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;
        } case 0x3000: {
            // Skip next if VX == NN
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 2;
            }
            break;
        } case 0x4000: {
            // Skip next if VX != NN
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 2;
            }
            break;
        } case 0x5000: {
            // Skip next if VX == VY
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc += 2;
            }
            break;
        } case 0x6000: {
            // Set VX to NN
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            break;
        } case 0x7000: {
            // Add NN to VX
            V[(opcode & 0xF00) >> 8] += (opcode & 0x00FF);
            break;
        } case 0x8000: {
            switch (opcode & 0x000F) {
                case 0x0000: {
                    // Set VX to VY
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    break;
                } case 0x0001: {
                    // Set VX to VX | VY
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    break;
                } case 0x0002: {
                    // Set VX to VX & VY
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    break;
                } case 0x0003: {
                    // Set VX to VX ^ VY
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    break;
                } case 0x0004: {
                    // Add VY to VX
                    unsigned char VX = V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    // Set carry flag
                    if (V[(opcode & 0x0F00) >> 8] < VX) { 
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    break;
                } case 0x0005: {
                    // Subtract VY from VX
                    unsigned char VX = V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    // Set carry flag
                    if (V[(opcode & 0x0F00) >> 8] > VX) { 
                        V[0xF] = 0;
                    } else {
                        V[0xF] = 1;
                    }
                    break;
                } case 0x0006: {
                    // Set VX to VX >> 1
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0b00000001;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    break;
                } case 0x0007: {
                    // Subtract VX = VY - VX
                    unsigned char VX = V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    // Set carry flag
                    if (V[(opcode & 0x00F0) >> 4] >= VX) { 
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    break;
                } case 0x000E: {
                    // Set VX to VX << 1
                    V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0b10000000) >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    break;
                } default: {
                    throw std::runtime_error(opcodeErrorMsg());
                }
            }
            break;
        } case 0x9000: {
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0X00F0) >> 4]) {
                pc += 2;
            }
            break;
        } case 0xA000: {
            // set I to NNN
            I = opcode & 0x0FFF;
            break;
        } case 0xB000: {
            // Jump to NNN + V0
            pc = V[0x0] + (opcode & 0x0FFF);
            break;
        } case 0xC000: {
            // Set VX = rand() & NN
            V[(opcode & 0x0F00) >> 8] = (std::rand() % 255) & (opcode & 0x00FF);
            break;
        } case 0xD000: {
            // Draw sprite at coord VX, VY
            // with width of 8 pixels and height of N pixels
            
            // Decode instruction
            unsigned char spriteX = V[(opcode & 0x0F00) >> 8] % DISPLAY_WIDTH;
            unsigned char spriteY = V[(opcode & 0x00F0) >> 4] % DISPLAY_HEIGHT;
            unsigned char width = 8;
            unsigned char height = opcode & 0x000F;

            // Cull off-screen portions of sprite
            if (spriteX + width > DISPLAY_WIDTH - 1) {
                width = DISPLAY_WIDTH - spriteX;
            }
            if (spriteY + height > DISPLAY_HEIGHT - 1) {
                height = DISPLAY_HEIGHT - spriteY;
            }
            
            // Execute instruction, drawing sprite
            V[0xF] = 0;
            for (unsigned char y = 0; y < height; y++) {
                unsigned char spriteData = device->memory.read(I + y);
                for (unsigned char x = 0; x < width; x++) {
                    unsigned char oldPixelValue = device->display.readPixel(x + spriteX, y + spriteY);
                    unsigned char newPixelValue = oldPixelValue ^ ((spriteData >> (7 - x)) & 0b00000001);
                    device->display.setPixel(x + spriteX, y + spriteY, newPixelValue);

                    // Set collision flag
                    if (oldPixelValue == 1 && newPixelValue == 0) { 
                        V[0xF] = 1;
                    }
                }
            }
            break;
        } case 0xE000: {
            switch(opcode & 0x00FF) {
                case 0x009E: {
                    // Skip next if VX pressed
                    if (device->keyboard.getKeyPressed(V[(opcode & 0x0F00) >> 8])) {
                        pc += 2;
                    }
                    break;
                } case 0x00A1: {
                    // Skip next if VX not pressed
                    if (!device->keyboard.getKeyPressed(V[(opcode & 0x0F00) >> 8])) {
                        pc += 2;
                    }
                    break;
                } default: {
                    throw std::runtime_error(opcodeErrorMsg());
                }
            }
            break;
        } case 0xF000: {
            switch(opcode & 0x00FF) {
                case 0x0007: {
                    // Set VX to delay
                    V[(opcode & 0x0F00) >> 8] = delayTimer;
                    break;
                } case 0x000A: {
                    // Await and set VX to keypress
                    pc -= 2;
                    for (unsigned char key = 0; key <= 0xF; key++) {
                        if (device->keyboard.getKeyPressed(key)) {
                            V[(opcode & 0x0F00) >> 8] = key;
                            pc += 2;
                            break;
                        }
                    }
                    break;
                } case 0x0015: {
                    // Set delay to VX
                    delayTimer = V[(opcode & 0x0F00) >> 8];
                    break;
                } case 0x0018: {
                    // Set sound to VX
                    soundTimer = V[(opcode & 0x0F00) >> 8];
                    break;
                } case 0x001E: {
                    // Add VX to I
                    I += V[(opcode & 0x0F00) >> 8];
                    break;
                } case 0x0029: {
                    // Set I to location of character VX
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    break;
                } case 0x0033: {
                    // Store decimal representation of VX at I
                    unsigned char VX = V[(opcode & 0x0F00) >> 8];
                    device->memory.write(I + 2, VX % 10);
                    VX /= 10;
                    device->memory.write(I + 1, VX % 10);
                    VX /= 10;
                    device->memory.write(I, VX % 10);
                    break;
                } case 0x0055: {
                    // Dump registers V0-VX in memory starting at I
                    const unsigned char VX = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= VX; i++) {
                        device->memory.write(I + i, V[i]);
                    }
                    break;
                } case 0x0065: {
                    // Load registers V0-VX from memory starting at I
                    const unsigned char VX = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= VX; i++) {
                        V[i] = device->memory.read(I + i);
                    }
                    break;
                } default: {
                    throw std::runtime_error(opcodeErrorMsg());
                }
            }
            break;
        } default: {
            throw std::runtime_error(opcodeErrorMsg());
        }
    }
}