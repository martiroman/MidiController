#include "keyboard/PianoKeyboard.h"
#include "controls/Controls.h"
#include "../IUIScreen.h"

class ScreenPiano : public IUIScreen {
    private:
        ControlsUI* controlsUi;
        PianoKeyboard* keyboard;
    
    public:
        ScreenPiano();
        ~ScreenPiano();
        uint8_t handleTouch(int, int);
        void draw(Arduino_RGB_Display*);
};