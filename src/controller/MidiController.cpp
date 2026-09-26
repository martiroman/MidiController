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
    
    // Misma nota no hace nada
    if (activeNote == note) {
        return;
    }

    // Si hay nota activa, primero la apaga
    if (activeNote != -1) {
        if (!midi.noteOff(1, activeNote, 0)) {
            return;
        }
        activeNote = -1;
    }

    // Nueva nota
    if (midi.noteOn(1, note, velocity)) {
        activeNote = note;
    }
}

void MidiController::noteOff(uint8_t velocity) {
    if (activeNote != -1 && midi.noteOff(1, activeNote, velocity)) {
        activeNote = -1;
    }
}

void MidiController::sendControlChange(uint8_t control, uint8_t value) {
    midi.controlChange(1, control, value);
}