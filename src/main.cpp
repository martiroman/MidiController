#include <Arduino.h>
#include "hardware.h"
#include "ui.h"
#include "UsbMidi.h"
#include "M5UnitSynth.h"

// App State Variables
bool arp_activo = false;
bool nts1_listo = false;
bool toque_anterior = false;

// Secuencer variables
unsigned long t_ultimo_paso = 0;
unsigned long intervalo = 250; 
uint8_t notas[4] = {60, 63, 67, 70}; // Acorde de ejemplo
int idx = 0;

// Global objects
Arduino_RGB_Display* gfx;
TAMC_GT911 ts(8, 9, 4, -1, 800, 480);
UsbMidi midi;
M5UnitSynth synth;

Boton btnPlay = {280, 180, 240, 100, "PLAY/STOP", GREEN, BLUE, &arp_activo};

void setup() {
    Serial.begin(115200);
    Wire.begin(8, 9);
    delay(100);

    init_wifi_ota();
    
    // Encender el bus de 5V para el NTS-1
    ch422g_write(CH422G_REG_MODE, 0x01);
    ch422g_write(CH422G_REG_OUT, 0x1F);
    delay(500);

    gfx = init_display();
    ts.begin();

    // Inicializar MIDI
    midi.begin();

    debug_screen(gfx, "Starting CyM...", WHITE);
    
    if (WiFi.status() == WL_CONNECTED) {
        debug_screen(gfx, "WiFi Conectada!", GREEN);
        String ipStr = "IP: " + WiFi.localIP().toString();
        debug_screen(gfx, ipStr.c_str(), BLUE);
    }
    
    // Dibujar interfaz inicial
    btnPlay.dibujar(gfx);
}

void loop() {
    ArduinoOTA.handle();
    ts.read();
    
    // Procesar interacciones de Touch
    if (ts.isTouched && !toque_anterior) {
        int tx = ts.points[0].x;
        int ty = ts.points[0].y;
        
        if (btnPlay.checkToque(tx, ty)) {
            if (!arp_activo) {
                // Panico: Apagar ultima nota si se frena el arp
                synth.setNoteOff(1, notas[idx == 0 ? 3 : idx - 1], 0);
            }
            btnPlay.dibujar(gfx); // Redibuja con el nuevo color
        }
    }
    toque_anterior = ts.isTouched;

    // Lógica del secuenciador / Arpegiador
    if (arp_activo && (millis() - t_ultimo_paso >= intervalo)) {
        synth.setNoteOff(1, notas[idx == 0 ? 3 : idx - 1], 0);
        synth.setNoteOn(1, notas[idx], 100);
        
        idx = (idx + 1) % 4;
        t_ultimo_paso = millis();
    }
    
    delay(1);
}