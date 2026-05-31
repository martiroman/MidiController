#pragma once

namespace PianoConfig {
    constexpr int SCREEN_WIDTH    = 800;
    constexpr int SCREEN_HEIGHT   = 480;
    constexpr int KEY_Y_OFFSET    = 120;
    constexpr int WHITE_KEY_COUNT = 8;

    constexpr const int WHITE_KEY_WIDTH  = SCREEN_WIDTH / WHITE_KEY_COUNT; // 100 px each
    constexpr const int WHITE_KEY_HEIGHT = SCREEN_HEIGHT - KEY_Y_OFFSET; // 360 px
    constexpr const int BLACK_KEY_WIDTH  = (WHITE_KEY_WIDTH  * 6) / 10;   // 60% of white width
    constexpr const int BLACK_KEY_HEIGHT = (WHITE_KEY_HEIGHT * 6) / 10;   // 60% of white height

    // Black key positions within a 13-note chromatic scale (0–12):
    // C#(1), D#(3), F#(6), G#(8), A#(10)
    constexpr static const int BLACK_KEY_COUNT  = 5;
}