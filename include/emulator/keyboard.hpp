#pragma once

#include <atomic>

class Keyboard {
    private:
        std::atomic<bool> keys[16]{false};
    public:
        void reset();
        bool getKeyPressed(unsigned char key) const;
        void setKeyPressed(unsigned char key, bool value);
};