#pragma once

#include <Arduino.h>

// Centinela para "ninguna nota": handleTouch devuelve uint8_t y no puede
// representar -1; las notas MIDI validas van de 0 a 127, asi que 255 esta libre.
static const uint8_t NO_NOTE = 255;

// Notas MIDI reales indexadas del 0 al 12 (Do4 a Do5)
static const uint8_t BASE_NOTES[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
static const char* const NOTE_NAMES[13] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C"};

inline uint8_t getMidiNote(uint8_t keyIndex, uint8_t currentOctave) {
    int note = BASE_NOTES[keyIndex] + (currentOctave * 12);
    if (note > 127) note = 127; // Limite del estándar MIDI
    return note;
}
