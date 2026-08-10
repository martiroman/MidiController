#include "ui_controls.h"

void ControlsUI::draw(Arduino_RGB_Display* gfx) {
    gfx->fillRect(0, 0, PianoConfig::SCREEN_WIDTH, BAR_HEIGHT, COLOR_BAR_BG);

    drawPixelButton(gfx, rectRec,
        isRecording ? "STOP" : "REC",
        isRecording ? COLOR_BTN_RED_FACE : COLOR_BTN_RED_SHADOW,
        isRecording ? COLOR_BTN_RED_SHADOW : COLOR_BTN_RED_FACE);

    drawPixelButton(gfx, rectPlay,
        isPlaying ? "STOP" : "PLAY",
        isPlaying ? COLOR_BTN_GREEN_FACE : COLOR_BTN_GREEN_SHADOW,
        isPlaying ? COLOR_BTN_GREEN_SHADOW : COLOR_BTN_GREEN_FACE);

    drawPixelButton(gfx, rectOctDn, "<", COLOR_BTN_BLACK_FACE, COLOR_BTN_BLACK_SHADOW);
    drawPixelButton(gfx, rectOctUp, ">", COLOR_BTN_BLACK_FACE, COLOR_BTN_BLACK_SHADOW);

    drawOctaveDisplay(gfx);
}

bool ControlsUI::handleTouch(int tx, int ty) {
    if (contains(rectRec, tx, ty)) { toggleRec(); return true; }
    if (contains(rectPlay, tx, ty)) { togglePlay(); return true; }
    if (contains(rectOctDn, tx, ty)) { changeOctave(-1); return true; }
    if (contains(rectOctUp, tx, ty)) { changeOctave(+1); return true; }
    return false;
}

bool ControlsUI::contains(const Rect& r, int x, int y) {
    return x >= r.x && x <= r.x + r.w &&
           y >= r.y && y <= r.y + r.h;
}

void ControlsUI::drawPixelButton(Arduino_RGB_Display* gfx,
                                 const Rect& r,
                                 const char* label,
                                 uint16_t faceColor,
                                 uint16_t shadowColor)
{
    gfx->fillRect(r.x, r.y + r.h, r.w, BTN_SHADOW, shadowColor);
    gfx->fillRect(r.x, r.y, r.w, r.h, faceColor);
    //gfx->drawRect(r.x, r.y, r.w, r.h + BTN_SHADOW, COLOR_BTN_SHADOW);

    gfx->setTextColor(COLOR_BTN_TEXT);
    gfx->setTextSize(4);
    int textW = strlen(label) * 6 * 4; // 6 pixels per character at size 4
    int textX = r.x + (r.w - textW) / 2;
    int textH = 8 * 4; // 32 px den alto a tamao 4
    int textY = r.y + (r.h - textH) / 2;
    gfx->setCursor(textX, textY);
    gfx->print(label);
}

void ControlsUI::drawOctaveDisplay(Arduino_RGB_Display* gfx) {
    int x = rectOctDn.x + rectOctDn.w + 4;
    int w = rectOctUp.x - x - 4;

    gfx->fillRect(x, BAR_Y, w, BTN_H, COLOR_DISPLAY_BG);

    gfx->setTextColor(COLOR_DISPLAY_TEXT);
    gfx->setTextSize(4);

    char buf[8];
    snprintf(buf, sizeof(buf), "OCT %d", currentOctave);

    // Dimensiones reales del texto con textSize(4)
    int textW = strlen(buf) * 24; // 6 px * 4
    int textH = 32;               // 8 px * 4

    // Centrado en X e Y
    int textX = x + (w - textW) / 2;
    int textY = BAR_Y + (BTN_H - textH) / 2;

    gfx->setCursor(textX, textY);
    gfx->print(buf);
}

void ControlsUI::toggleRec() {
    isRecording = !isRecording;
}

void ControlsUI::togglePlay() {
    isPlaying = !isPlaying;
}

void ControlsUI::changeOctave(int dir) {
    currentOctave = constrain(currentOctave + dir, 1, 8);
}
