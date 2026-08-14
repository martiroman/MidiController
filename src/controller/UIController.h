#pragma once

#include "../ui/IUIScreen.h"
#include "../ui/intro/introScreen.h"
#include "IController.h"

class UIController : IController {
    private:
        IUIScreen* currentScreen;
        Arduino_RGB_Display* gfx;
        
        IntroScreen* introScreen;

    public:
        UIController();
        ~UIController();

        void begin();
        void update();
        void drawCurrentScreen();
        void debugMsg(const char* msg, uint16_t color = WHITE);
        uint8_t handleTouch(int, int);
};
