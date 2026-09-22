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
    
    if (touchController.isTouchActive()) {
        touchController.getTouchCoordinates(tx, ty);
        
        UIEvent event = uiController.handleTouch(tx, ty); 
        
        //Mensaje para debuggear eventos en la pantalla
        uiController.debugMsg(event.toString().c_str(), RED);

        // El AppController despacha segun el tipo de evento
        switch (event.type) {
            case EventType::PLAY_NOTE:
                midiController.noteOn(event.data1, 127);
                break;
                
            case EventType::CC_CHANGE:
                midiController.sendControlChange(event.data1, event.data2);
                break;
                
            default:
                break;
        }

    } else {
        //TODO: Mejorar mediante evento de UIevent:: Noteoff
        midiController.noteOff(0);
    }

    midiController.update();
    uiController.update();
}