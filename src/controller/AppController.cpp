// AppController.cpp
#include "AppController.h"
#include "../ui/screenPiano/UIConfig.h"
#include "../ui/screenPiano/keyboard/Notes.h"
#include "../ui/UIEvent.h"

AppController::AppController() {
}

void AppController::begin() {
    midiController.begin();
    uiController.begin();
    touchController.begin();
}

void AppController::update() {
    int tx, ty;

    touchController.update();
    touchController.getTouchCoordinates(tx, ty);
    
    UIEvent event = uiController.handleTouch(tx, ty); 
    
    // El AppController despacha segun el tipo de evento
    switch (event.type) {
        case EventType::PLAY_NOTE:
            midiController.noteOn(event.data1, 127);
            uiController.debugMsg(event.toString().c_str(), RED);
            break;

        case EventType::NOTE_OFF:
            midiController.noteOff(0);
            break;
            
        case EventType::CC_CHANGE:
            midiController.sendControlChange(event.data1, event.data2);
            break;
        
        default:
            break;
    }

    //midiController.update();
    //uiController.update();
}