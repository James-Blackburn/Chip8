#pragma once

class Emulator;

class Keyboard {
    private:
        Emulator* device;        
        int mapKey(unsigned char hexKey);
    public:
        void setDevice(Emulator* emulator);
        bool getKeyPressed(unsigned char key);
};