#include <Arduino.h>
#include "hardware.h"
#include "ui.h"
#include "UsbMidi.h"
#include "keyboard.h"
#include "intro.h"
#include "music.h"

// Global objects
Arduino_RGB_Display* gfx;
TAMC_GT911 ts(8, 9, 4, -1, 800, 480);
UsbMidi midi;

void setup() {
    Serial.begin(115200);
    init_wifi_ota();
    gfx = init_display();
    ts.begin();
    Wire.begin(8, 9);
    
    midi.begin();
    debug_screen(gfx, "USB MIDI Inicializado", WHITE);

    String ipStr = (WiFi.status() == WL_CONNECTED) ? "WIFI Connected | IP: " + WiFi.localIP().toString() : "WIFI not connected";
    
    debug_screen(gfx, ipStr.c_str(), WHITE);
    
    delay(5000);
    intro(midi, gfx);
    drawKeyboard(gfx);
}

void loop() {
    ArduinoOTA.handle();
    ts.read();
    midi.update();
    
    if (ts.isTouched) {
        int tx = ts.points[0].x;
        int ty = ts.points[0].y;

        int nota_detectada = getKeyboardNote(tx, ty);

        if (nota_detectada != -1) {
            uint8_t nota_midi = NOTES[nota_detectada];

            // Si el dedo se movió a otra tecla distinta
            if (nota_detectada != ultima_nota_tocada) {
                
                // 1. Apagar nota anterior 
                if (ultima_nota_tocada != -1) {
                    midi.noteOff(1, NOTES[ultima_nota_tocada], 0);
                }

                // 2. Encender nueva nota
                midi.noteOn(1, nota_midi, 127); // 127 = Volumen maximo (Estilo chiptune)
                
                // Guardamos el estado para no repetir el disparo por rafaga
                ultima_nota_tocada = nota_detectada;
                
            }
        }
    } else {
        // En el momento exacto en que se levanta el dedo de la pantalla
        if (ultima_nota_tocada != -1) {
            midi.noteOff(1, NOTES[ultima_nota_tocada], 0);
            
            // Limpiamos rastro visual si es necesario y reseteamos el estado
            ultima_nota_tocada = -1;
        }
    }

    delay(10); // Estabilidad para los rebotes del touch
}
