// --- Arte ASCII (usando raw strings para facilitar) ---
// --- Arte ASCII Estilo Arcade de los 80 ---
const char* arc_almen[] = {
  "     _    _      __  __ _____ _   _ ",
  "    / \\  | |    |  \\/  | ____| \\ | |",
  "   / _ \\ | |    | |\\/| |  _| |  \\| |",
  "  / ___ \\| |___ | |  | | |___| |\\  |",
  " /_/   \\_\\_____||_|  |_|_____|_| \\_|"
};

const char* arc_synth[] = {
  "  ____ __     __ _   _ _____ _   _ ",
  " / ___|\\ \\   / /| \\ | |_   _| | | |",
  " \\___ \\ \\ \\ / / |  \\| | | | | |_| |",
  "  ___) | \\ V /  | |\\  | | | |  _  |",
  " |____/   \\_/   |_| \\_| |_| |_| |_|"
};

const char* arc_korg[] = {
  "  _  _____  ____   ____ ",
  " | |/ / _ \\|  _ \\ / ___|",
  " | ' / | | | |_) | |  _ ",
  " | . \\ |_| |  _ <| |_| |",
  " |_|\\_\\___/|_| \\_\\\\____|"
};

const char* arc_boder_top = "################################################################";
const char* arc_boder_bot = "################################################################";

// --- Melodía de Mario Bros ---
struct Note {
  int note;
  int duration;
};

Note marioMelody[] = {
  {76, 150}, {76, 150}, {0, 150}, {76, 150}, {0, 150}, {72, 150}, {76, 150}, {0, 150}, {79, 150}, {0, 450}, {67, 150}, {0, 450}, 
};

// --- Definiciones de Colores Retro (565 RGB) ---
#define RETRO_RED      0xF800
#define RETRO_GREEN    0x07E0
#define RETRO_BLUE     0x001F
#define RETRO_YELLOW   0xFFE0
#define RETRO_MAGENTA  0xF81F
#define RETRO_CYAN     0x07FF
#define RETRO_ORANGE   0xFD20
#define RETRO_PURPLE   0x780F



void drawAscii(Arduino_RGB_Display* gfx, const char* art[], int lines, int x, int y, uint16_t color) {
  gfx->setTextColor(color);
  for (int i = 0; i < lines; ++i) {
    gfx->setCursor(x, y + (i * 12));
    gfx->print(art[i]);
  }
}

void intro(UsbMidi& midi, Arduino_RGB_Display* gfx) {
  gfx->fillScreen(BLACK);
  gfx->setTextSize(2);
  int startY = 50;

  for(int i = 0; i < 4; i++) {
    gfx->fillScreen(RETRO_RED);
    delay(80);
    gfx->fillScreen(BLACK);
    delay(80);
  }
  delay(200);

  drawAscii(gfx, arc_almen, 5, 100, startY, RETRO_RED);
  startY += 70;
  delay(300);

  drawAscii(gfx, arc_synth, 5, 120, startY, RETRO_BLUE);
  startY += 70;
  delay(300);

  drawAscii(gfx, arc_korg, 5, 230, startY, RETRO_GREEN);
  startY += 80;
  delay(500);

  gfx->setTextColor(RETRO_CYAN);
  gfx->setCursor(0, startY);
  gfx->print(arc_boder_top);
  
  startY += 20;
  gfx->setTextSize(2);
  gfx->setTextColor(RETRO_YELLOW);
  gfx->setCursor(220, startY);
  gfx->println("PRESIONE START");
  
  gfx->setTextSize(2);
  gfx->setTextColor(WHITE);
  gfx->setCursor(280, startY + 25);
  gfx->println("< INSERT COIN >");

  startY += 50;
  gfx->setTextColor(RETRO_CYAN);
  gfx->setCursor(0, startY);
  gfx->print(arc_boder_bot);

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
  gfx->fillScreen(BLUE);
}