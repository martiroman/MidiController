#pragma once
#include <stdint.h>

// tipos de acciones posibles en la UI
enum class EventType {
    START_PRESSED, // Intro screen
    NONE,
    PLAY_NOTE,     // Para el sintetizador / piano
    CHANGE_SCREEN, // Para navegar entre pantallas
    CC_CHANGE      // Perillas o faders MIDI
};

// estructura que viaja con la informacion del evento
struct UIEvent {
    EventType type = EventType::NONE;
    uint8_t data1 = 0; // Usado para la nota MIDI, ID de boton
    uint8_t data2 = 0; // Usado para velocity, valor de fader

    UIEvent() : type(EventType::NONE), data1(0), data2(0) {}

    UIEvent(EventType t, uint8_t d1 = 0, uint8_t d2 = 0) 
        : type(t), data1(d1), data2(d2) {}
};