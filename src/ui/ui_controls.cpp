#include "ui_controls.h"

void ControlsUI::draw(Arduino_RGB_Display* gfx) {
    gfx->fillRect(0, 0, PianoConfig::SCREEN_WIDTH, BAR_HEIGHT, COLOR_BAR_BG);

    drawPixelButton(gfx, rectRec,
        isRecording ? "STOP" : "REC",
        isRecording ? COLOR_BTN_RED_FACE : COLOR_BTN_RED_FACE,
        COLOR_BTN_RED_SHADOW);

    drawPixelButton(gfx, rectPlay,
        isPlaying ? "STOP" : "PLAY",
        isPlaying ? 0xFFE0 : COLOR_BTN_GREEN_FACE,
        isPlaying ? 0xA080 : COLOR_BTN_GREEN_SHADOW);

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
    gfx->drawRect(r.x, r.y, r.w, r.h + BTN_SHADOW, COLOR_BTN_SHADOW);

    gfx->setTextColor(COLOR_BTN_TEXT);
    gfx->setTextSize(1);
    int textW = strlen(label) * 6;
    int textX = r.x + (r.w - textW) / 2;
    int textY = r.y + (r.h - 8) / 2;
    gfx->setCursor(textX, textY);
    gfx->print(label);
}

void ControlsUI::drawOctaveDisplay(Arduino_RGB_Display* gfx) {
    int x = rectOctDn.x + rectOctDn.w + 4;
    int w = rectOctUp.x - x - 4;

    gfx->fillRect(x, BAR_Y, w, BTN_H, COLOR_DISPLAY_BG);
    gfx->drawRect(x, BAR_Y, w, BTN_H, COLOR_DISPLAY_TEXT);
    gfx->drawRect(x + 1, BAR_Y + 1, w - 2, BTN_H - 2, COLOR_DISPLAY_TEXT);

    gfx->setTextColor(COLOR_DISPLAY_TEXT);
    gfx->setTextSize(1);
    char buf[8];
    snprintf(buf, sizeof(buf), "OCT %d", currentOctave);
    int textW = strlen(buf) * 6;
    gfx->setCursor(x + (w - textW) / 2, BAR_Y + (BTN_H - 8) / 2);
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
