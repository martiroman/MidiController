#include "introScreen.h"

IntroScreen::IntroScreen(){

}

void IntroScreen::draw(Arduino_RGB_Display* gfx) {
// Draw the intro screen graphics here
    gfx->fillScreen(BLACK);
    gfx->setTextSize(5); 
    gfx->setCursor(20, 30);
    gfx->setTextColor(RETRO_GREEN); 
    gfx->println("ALMEN");

    gfx->setCursor(20, 70);
    gfx->setTextColor(RETRO_CYAN); 
    gfx->println("SYNTH");

    gfx->setCursor(20, 110);
    gfx->setTextColor(RETRO_YELLOW); 
    gfx->println("KORG");

    // Subtitles Arcade
    gfx->setTextSize(3);
    gfx->setTextColor(RETRO_YELLOW);
    gfx->setCursor(20, 160);
    gfx->println("PRESIONE START");
    
    gfx->setTextColor(WHITE);
    gfx->setCursor(20, 190);
    gfx->println("< INSERT COIN >");
}

