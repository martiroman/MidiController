#pragma once

#include "MidiController.h"
#include "UIController.h"
#include "TouchController.h"

class UIController;   // evita dependencia circular
class MidiController;
class TouchController;

class AppController : IController{
    public: 
        AppController();
        void begin();
        void update();

    private:
        TouchController touchController;
        UIController uiController;
        MidiController midiController;
};