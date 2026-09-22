#pragma once

#include "keyboard/PianoKeyboard.h"
#include "controls/Controls.h"
#include "../IUIScreen.h"
#include "../UIEvent.h"

class ScreenPiano : public IUIScreen {
    private:
        ControlsUI* controlsUi;
        PianoKeyboard* keyboard;
    
    public:
        ScreenPiano();
        ~ScreenPiano();
        UIEvent handleTouch(int, int);
        void draw(Arduino_RGB_Display*);
};