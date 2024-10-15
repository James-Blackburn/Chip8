#include "speaker.hpp"

#include <stdexcept>

void Speaker::init() {
    device = alcOpenDevice(nullptr);
    if (!device) { 
        throw std::runtime_error("Failed to open a playback device");
    }

    context = alcCreateContext(device, nullptr);
    if (!context) {
        alcCloseDevice(device);
        throw std::runtime_error("Failed to create an OpenAL context");
    } 
    alcMakeContextCurrent(context);
    createSource();
}

void Speaker::createSource() {
    // Create square wave
    constexpr int SECONDS = 4;
    constexpr int SAMPLES = SECONDS * SAMPLE_RATE;
    constexpr int PERIOD = SAMPLE_RATE / FREQUENCY;
    
    ALshort *data = new ALshort[SAMPLES];
    for (int i = 0; i < SAMPLES; ++i) {
        data[i] = (i % PERIOD < PERIOD / 2) ? 32767 : -32767;
    }

    // Fill buffer with wave
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, data, SAMPLES * sizeof(ALshort), SAMPLE_RATE);
    delete[] data;

    // Create source
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
}

void Speaker::playBeep() {
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        alSourcePlay(source);
    } else {
        // Do not play sound
    }
}

void Speaker::stopBeep() {
    alSourceStop(source);
}

void Speaker::destroy() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

Speaker::~Speaker() {
    destroy();
}