#pragma once

#include "keyboard/PianoKeyboard.h"
#include "controls/Controls.h"
#include "../IUIScreen.h"
#include "../UIEvent.h"

class ScreenPiano : public IUIScreen {
    private:
        ControlsUI* controlsUi;
        PianoKeyboard* keyboard;
        Arduino_RGB_Display* gfx = nullptr;
    
    public:
        ScreenPiano(Arduino_RGB_Display*);
        ~ScreenPiano();
        UIEvent handleTouch(int, int);
        void draw();
};