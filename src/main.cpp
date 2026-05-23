#include <Arduino.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include "UsbMidi.h"
#include "M5UnitSynth.h"

// --- Instancias ---
UsbMidi midi;
M5UnitSynth synth;
uint8_t program_current = 0;
bool nts1_listo = false;

// --- CORRECCIÓN CH422G (Control de energía de la placa) ---
#define CH422G_I2C_ADDR 0x24  // direccion del chip
#define CH422G_REG_MODE 0x01  // registro para configurar modo
#define CH422G_REG_OUT  0x02  // registro para activar salidas

void ch422g_write(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CH422G_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// --- LCD (Pines Waveshare S3 4.3") ---
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
    14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(800, 480, bus);

// --- Touch ---
TAMC_GT911 ts(8, 9, 4, -1, 800, 480);

// --- Arpegio ---
const int BTN_X = 280, BTN_Y = 180, BTN_W = 240, BTN_H = 100;
bool arp_activo = false;
bool toque_anterior = false;
uint8_t notas[] = {60, 64, 67, 72}; // Do, Mi, Sol, Do
int idx = 0;
unsigned long t_ultimo_paso = 0;
const int intervalo = 125; 

void debug_screen(const char* msg, uint16_t color = WHITE) {
    static int line = 0;
    if (line > 15) { gfx->fillScreen(BLACK); line = 0; }
    gfx->setCursor(10, 10 + (line * 25));
    gfx->setTextColor(color);
    gfx->setTextSize(2);
    gfx->println(msg);
    line++;
    Serial.println(msg);
}

void dibujar_boton() {
    gfx->fillRoundRect(BTN_X, BTN_Y, BTN_W, BTN_H, 14, arp_activo ? GREEN : BLUE);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(4);
    gfx->setCursor(BTN_X + 65, BTN_Y + 35);
    gfx->print(arp_activo ? "STOP" : "PLAY");
}

// --- Callbacks MIDI ---
void onConnected() {
    debug_screen("NTS-1 CONECTADO!", GREEN);
    nts1_listo = true;
}

void onDisconnected() {
    debug_screen("NTS-1 DESCONECTADO", RED);
    nts1_listo = false;
}

void onMidiMessage(const uint8_t (&data)[4]) {
    // Aquí recibís MIDI desde el NTS-1 si tocaras sus teclas
}

void setup() {
    Serial.begin(115200);
    
    // 1. Inicializar I2C en los pines de la Waveshare
    Wire.begin(8, 9); 
    delay(100);

    // 2. ACTIVAR ENERGÍA USB (Crucial para que el NTS-1 prenda)
    ch422g_write(CH422G_REG_MODE, 0x01); // Modo salida
    ch422g_write(CH422G_REG_OUT, 0x1F);  // "Abrir canilla" de 5V
    delay(500); // Esperar que el NTS-1 reaccione

    // 3. Inicializar Pantalla
    gfx->begin();
    gfx->fillScreen(BLACK);
    ts.begin();

    // 4. Inicializar USB MIDI
    midi.onMidiMessage(onMidiMessage);
    midi.onDeviceConnected(onConnected);
    midi.onDeviceDisconnected(onDisconnected);
    midi.begin();

    debug_screen("Sistema Iniciado...", YELLOW);
    dibujar_boton();
}

void loop() {
    ts.read();
    
    if (ts.isTouched && !toque_anterior) {
        int x = ts.points[0].x;
        int y = ts.points[0].y;
        
        if (x >= BTN_X && x <= BTN_X + BTN_W && y >= BTN_Y && y <= BTN_Y + BTN_H) {
            arp_activo = !arp_activo;
            if (!arp_activo) {
                // Pánico: Apagar nota al detener
                synth.setNoteOff(1, notas[idx == 0 ? 3 : idx - 1], 0);
            }
            dibujar_boton();
        }
    }
    toque_anterior = ts.isTouched;

    if (arp_activo && (millis() - t_ultimo_paso >= intervalo)) {
        // El NTS-1 suele usar el Canal 1 por defecto
        synth.setNoteOff(1, notas[idx == 0 ? 3 : idx - 1], 0);
        synth.setNoteOn(1, notas[idx], 100);
        
        idx = (idx + 1) % 4;
        t_ultimo_paso = millis();
    }
    
    delay(1); 
}