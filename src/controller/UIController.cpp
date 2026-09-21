#include <Arduino.h>
#include "UIController.h"
#include "../ui/Colors.h"
#include "../ui/screenPiano/UIConfig.h"
#include "../ui/screenPiano/keyboard/Notes.h"

using namespace UIConfig;

UIController::UIController(){
        introScreen = new IntroScreen();
        pianoScreen = nullptr;   // se crea en begin(), con el display ya inicializado
        currentScreen = introScreen;
    }

UIController::~UIController() {
        delete introScreen;
        delete pianoScreen;
}

void UIController::drawCurrentScreen(){
    if (currentScreen) {
        currentScreen->draw(gfx);
    }
}

void UIController::showPianoScreen() {
    if (!pianoScreen) {
        Serial.println("showPianoScreen(): pianoScreen no inicializado");
        return;
    }
    currentScreen = pianoScreen;
    drawCurrentScreen();
}

uint8_t UIController::handleTouch(int tx, int ty) {
    if (!currentScreen) return 0;

    uint8_t result = currentScreen->handleTouch(tx, ty);

    // Desde el intro, cualquier toque avanza al teclado
    if (currentScreen == introScreen && result == IntroScreen::START_PRESSED) {
        showPianoScreen();
        return result;
    }

    // En el teclado, ty > BAR_HEIGHT es la zona de teclas: result es la nota MIDI
    if (currentScreen == pianoScreen && ty > BAR_HEIGHT &&
        result != NO_NOTE && result != lastDebugNote) {
        lastDebugNote = result;

        char buf[24];
        snprintf(buf, sizeof(buf), "%s (%u)", NOTE_NAMES[result % 12], result);
        debugMsg(buf, RETRO_GREEN);
    }

    return result;
}

// Escribe siempre en el mismo recuadro: borra el mensaje anterior y lo reemplaza,
// asi no avanza de linea ni necesita limpiar la pantalla entera.
void UIController::debugMsg(const char* msg, uint16_t color){
    Serial.println(msg);

    if (!gfx) return;

    gfx->fillRect(DEBUG_X, DEBUG_Y, DEBUG_W, DEBUG_H, COLOR_DISPLAY_BG);

    gfx->setTextColor(color);
    gfx->setTextSize(DEBUG_TEXT_SIZE);
    gfx->setCursor(DEBUG_X + 4, DEBUG_Y + (DEBUG_H - (8 * DEBUG_TEXT_SIZE)) / 2);
    gfx->print(msg);
}

void UIController::begin(){
    auto *bus = new Arduino_ESP32RGBPanel(
        5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
        14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
    );
    gfx = new Arduino_RGB_Display(800, 480, bus);
    gfx->begin();
    gfx->fillScreen(BLACK);

    pianoScreen = new ScreenPiano();
}

void UIController::update(){
    
}