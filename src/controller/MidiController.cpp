#include "MidiController.h"

MidiController::MidiController(){
    
}

void MidiController::begin(){
    midi.begin();
}

void MidiController::update(){
    midi.update();
}