#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 440

class Speaker {
    private:
        ALCdevice *device;
        ALCcontext *context;
        ALuint source;
        ALuint buffer;
        
        void createSource();
    public:
        ~Speaker();

        void init();
        void destroy();
        void playBeep();
        void stopBeep();
};