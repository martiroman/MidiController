#pragma once

#include "IController.h"
#include "UsbMidi.h"

class MidiController : IController{
    private:
        UsbMidi midi;

    public:
        MidiController();
        void begin();
        void update();
};