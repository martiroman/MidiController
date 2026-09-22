#pragma once

#include "../ui/IUIScreen.h"
#include "../ui/intro/introScreen.h"
#include "../ui/screenPiano/screenPiano.h"
#include "IController.h"
#include "../ui/UIEvent.h"

class UIController : IController {
    private:
        IUIScreen* currentScreen;
        Arduino_RGB_Display* gfx;

        IntroScreen* introScreen;
        ScreenPiano* pianoScreen;

    public:
        UIController();
        ~UIController();

        void begin();
        void update();
        void drawCurrentScreen();

        IUIScreen* getCurrentScreen() { return currentScreen; }
        void debugMsg(const char* msg, uint16_t color = WHITE);
        UIEvent handleTouch(int, int);
};
