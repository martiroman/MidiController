#pragma once

namespace Config {
    constexpr const int SCREEN_WIDTH    = 800;
    constexpr const int SCREEN_HEIGHT   = 480;
    constexpr const int TOP_BAR_HEIGHT  = 110;

// Controls
    constexpr const int BAR_HEIGHT = 110;  // = KEY_Y_OFFSET
    constexpr const int BTN_H = 62; // Altura de los botones
    constexpr const int BTN_SHADOW = 8; // Sombra de los botones
    constexpr const int BAR_Y = 16;    // Padding vertical en la barra

// PianoKeyboard
    constexpr const int KEY_Y_OFFSET    = TOP_BAR_HEIGHT;
    constexpr const int WHITE_KEY_COUNT = 8;

    constexpr const int WHITE_KEY_WIDTH  = SCREEN_WIDTH / WHITE_KEY_COUNT; // 100 px each
    constexpr const int WHITE_KEY_HEIGHT = SCREEN_HEIGHT - KEY_Y_OFFSET; // 360 px
    constexpr const int BLACK_KEY_WIDTH  = (WHITE_KEY_WIDTH  * 6) / 10;   // 60% of white width
    constexpr const int BLACK_KEY_HEIGHT = (WHITE_KEY_HEIGHT * 6) / 10;   // 60% of white height

    // Black key positions within a 13-note chromatic scale (0–12):
    // C#(1), D#(3), F#(6), G#(8), A#(10)
    constexpr static const int BLACK_KEY_COUNT  = 5;

    static constexpr int BLACK_KEY_INDICES[BLACK_KEY_COUNT] = {1, 3, 6, 8, 10};

}