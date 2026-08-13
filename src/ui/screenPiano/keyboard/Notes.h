#include <Arduino.h>

// Notas MIDI reales indexadas del 0 al 12 (Do4 a Do5)
const uint8_t BASE_NOTES[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const char* NOTE_NAMES[13] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C"};

uint8_t getMidiNote(uint8_t keyIndex, uint8_t currentOctave) {
    int note = BASE_NOTES[keyIndex] + (currentOctave * 12);
    if (note > 127) note = 127; // Limite del estándar MIDI
    return note;
}
