#pragma once

#include "../ui/IUIScreen.h"
#include "../ui/intro/introScreen.h"
#include "../ui/screenPiano/screenPiano.h"
#include "IController.h"

class UIController : IController {
    private:
        IUIScreen* currentScreen;
        Arduino_RGB_Display* gfx;

        IntroScreen* introScreen;
        ScreenPiano* pianoScreen;

        int lastDebugNote = -1;   // evita redibujar el recuadro en cada ciclo

    public:
        UIController();
        ~UIController();

        void begin();
        void update();
        void drawCurrentScreen();
        void showPianoScreen();
        bool isPianoScreen() const { return currentScreen == pianoScreen; }
        IUIScreen* getCurrentScreen() { return currentScreen; }
        void debugMsg(const char* msg, uint16_t color = WHITE);
        uint8_t handleTouch(int, int);
};
