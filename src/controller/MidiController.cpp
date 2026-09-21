#include "MidiController.h"

MidiController::MidiController(){
    
}

void MidiController::begin(){
    midi.begin();
}

void MidiController::update(){
    midi.update();
}

void MidiController::noteOn(uint8_t note, uint8_t velocity) {
    midi.noteOn(1, note, velocity);
}

void MidiController::noteOff(uint8_t note, uint8_t velocity) {
    midi.noteOff(1, note, velocity);
}