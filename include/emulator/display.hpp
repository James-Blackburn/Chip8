#pragma once

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

class Display {
    private:
        unsigned char pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT]{0};
        float textureData[DISPLAY_WIDTH * DISPLAY_HEIGHT * 3]{0.0f};
        bool textureUpdated = false;

        friend class Keyboard;
    public:
        void setPixel(int px, int py, unsigned char value);
        unsigned char readPixel(int px, int py) const;
        void clear();
        inline float* getTextureData();
        inline bool getTextureUpdated() const;
        inline void setTextureUpdated(bool status);
};

float* Display::getTextureData() { 
    return textureData;
}

bool Display::getTextureUpdated() const {
    return textureUpdated;
}

void Display::setTextureUpdated(bool status) {
    textureUpdated = status;
}