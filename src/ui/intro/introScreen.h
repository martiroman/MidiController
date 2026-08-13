#include "../IUIScreen.h"

#define RETRO_RED      0xF800
#define RETRO_GREEN    0x07E0
#define RETRO_BLUE     0x001F
#define RETRO_YELLOW   0xFFE0
#define RETRO_MAGENTA  0xF81F
#define RETRO_CYAN     0x07FF
#define RETRO_ORANGE   0xFD20
#define RETRO_PURPLE   0x780F


class IntroScreen : public IUIScreen {
public:
    IntroScreen();
    void draw(Arduino_RGB_Display*);
    uint8_t handleTouch(int tx, int ty) override {
        // Handle touch events on the intro screen here
        return 0; // Return a value indicating the result of the touch event
    }
};

    /*
// --- Melodia Mario Bros ---
struct Note {
  int note;
  int duration;
};

Note marioMelody[] = {
  {76, 150}, {76, 150}, {0, 150}, {76, 150}, {0, 150}, {72, 150}, {76, 150}, {0, 150}, {79, 150}, {0, 450}, {67, 150}, {0, 450}, 
};

void intro(UsbMidi& midi, Arduino_RGB_Display* gfx) {
  // 1. Efecto Flasheo Rojo / Negro inicial
  for(int i = 0; i < 4; i++) {
    gfx->fillScreen(RETRO_RED);
    delay(80);
    gfx->fillScreen(BLACK);
    delay(80);
  }
  delay(200);

  // 2. Limpiar a fondo negro para dibujar la interfaz
  gfx->fillScreen(BLACK);
  
  // Título principal (ALMEN / SYNTH / KORG)
  gfx->setTextSize(5); 
  gfx->setCursor(20, 30);
  gfx->setTextColor(0x07E0); // Verde
  gfx->println("ALMEN");

  gfx->setCursor(20, 70);
  gfx->setTextColor(0x07FF); // Cyan
  gfx->println("SYNTH");

  gfx->setCursor(20, 110);
  gfx->setTextColor(0xFFE0); // Amarillo
  gfx->println("KORG");

  // Subtítulos Arcade
  gfx->setTextSize(3);
  gfx->setTextColor(RETRO_YELLOW);
  gfx->setCursor(20, 160);
  gfx->println("PRESIONE START");
  
  gfx->setTextColor(WHITE);
  gfx->setCursor(20, 190);
  gfx->println("< INSERT COIN >");

  // 3. Bucle de la melodía Mario MIDI
  for (int i = 0; i < sizeof(marioMelody) / sizeof(marioMelody[0]); i++) {
    if (marioMelody[i].note > 0) {
      midi.noteOn(1, marioMelody[i].note, 127);
    }
    
    unsigned long startMs = millis();
    while (millis() - startMs < marioMelody[i].duration) {
      midi.update();
    }

    if (marioMelody[i].note > 0) {
      midi.noteOff(1, marioMelody[i].note, 127);
    }
    midi.update();
  }

  delay(2000);
}
  */