#pragma once

#include "IController.h"
#include "UsbMidi.h"

class MidiController : IController{
    private:
        UsbMidi midi;
        int activeNote = -1;

    public:
        MidiController();
        void begin();
        void update();
        void noteOn(uint8_t note, uint8_t velocity);
        void noteOff(uint8_t velocity);
        void sendControlChange(uint8_t, uint8_t);
    };