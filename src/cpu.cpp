#include "cpu.hpp"

#include <sstream>
#include <iostream>

#include "emulator.hpp"

std::string Cpu::opcodeErrorMsg() { 
    std::stringstream ss;
    ss << "Unknown opcode: " << std::hex << opcode;
    return ss.str();
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
                } default: {
                    throw std::runtime_error(opcodeErrorMsg());
                }
            }
            break;
        } case 0x1000: {
            // Jump to NNN
            pc = opcode & 0x0FFF;
            break;
        } case 0x6000: {
            // Set VX to NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        } case 0x7000: {
            // Add NN to VX
            V[(opcode & 0xF00) >> 8] += opcode & 0x00FF;
            break;
        } case 0xA000: {
            // set I to NNN
            I = opcode & 0x0FFF;
            break;
        } case 0xD000: {
            // Draw sprite at coord VX, VY
            // with width of 8 pixels and height of N pixels
            
            // Decode instruction
            unsigned char spriteX = V[(opcode & 0x0F00) >> 8] % DISPLAY_WIDTH;
            unsigned char spriteY = V[(opcode & 0x00F0) >> 4] % DISPLAY_HEIGHT;
            unsigned char height = opcode & 0x000F;
            
            // Execute instruction, drawing sprite
            V[0xF] = 0;
            for (unsigned char y = 0; y < height; y++) {
                //unsigned char spriteData = device->memory.read(I + (height - 1 - y));
                unsigned char spriteData = device->memory.read(I + y);
                for (unsigned char x = 0; x < 8; x++) {
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
        } default: {
            throw std::runtime_error(opcodeErrorMsg());
        }
    }

    std::cout << pc << "| executed opcode: " << std::hex << opcode << "\n";
}