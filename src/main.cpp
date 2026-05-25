#include <Arduino.h>
#include "hardware.h"
#include "ui.h"
#include "UsbMidi.h"
#include "M5UnitSynth.h"
#include "teclado.h"

// App State Variables
bool arp_activo = false;
bool nts1_listo = false;
bool toque_anterior = false;

// Secuencer variables
unsigned long t_ultimo_paso = 0;
unsigned long intervalo = 250; 
int idx = 0;

// Global objects
Arduino_RGB_Display* gfx;
TAMC_GT911 ts(8, 9, 4, -1, 800, 480);
UsbMidi midi;
M5UnitSynth synth;

Boton btnPlay = {280, 180, 240, 100, "PLAY/STOP", GREEN, BLUE, &arp_activo};

void setup() {
    Serial.begin(115200);
    
    digitalWrite(38, HIGH); 
    delay(200);

    init_wifi_ota();
    
    gfx = init_display();
    ts.begin();

    Serial.println("\n--- ESCANER I2C ---");

    // Forzamos los pines de habilitación
    pinMode(14, OUTPUT); digitalWrite(14, HIGH);
    pinMode(38, OUTPUT); digitalWrite(38, HIGH);
    delay(200);

    Wire.begin(8, 9);

    // Escaneo I2C para verificar que el CH422G responde
    // scan_i2c(gfx);

    debug_screen(gfx, "Configurando CH422G...", WHITE);

    // 1. Reset
    ch422g_write(CH422G_REG_MODE, 0x00); 
    delay(50);
    
    // 2. Modo 2: Habilitación Global (0x80) + Modo de Salida Alternativo (0x10)
    ch422g_write(CH422G_REG_MODE, 0x90); 
    delay(50);
    
    // 3. Probamos abrir con HIGH total
    ch422g_write(CH422G_REG_OUT, 0xFF);
    delay(500);

    debug_screen(gfx, "Outputs en HIGH. Verificar LED.", GREEN);

    // Inicializar MIDI
    midi.begin();
    debug_screen(gfx, "USB MIDI Inicializado", WHITE);
    
    // Inicializar Sintetizador
    synth.begin();
    debug_screen(gfx, "Sintetizador Inicializado", WHITE);

    if (WiFi.status() == WL_CONNECTED) {
        String ipStr = "WIFI Connected | IP: " + WiFi.localIP().toString();
        debug_screen(gfx, ipStr.c_str(), GREEN);
    }
    
    // Dibujar interfaz inicial
    //btnPlay.dibujar(gfx);
    dibujarTecladoRetro(gfx);
}

void loop() {
    ArduinoOTA.handle();
    ts.read();
    midi.update();
    
    if (ts.isTouched) {
        int tx = ts.points[0].x;
        int ty = ts.points[0].y;

        // Buscamos qué tecla de la octava se presionó
        int nota_detectada = obtenerNotaDesdeTouch(tx, ty);

        if (nota_detectada != -1) {
            uint8_t nota_midi = NOTAS_TECLADO[nota_detectada];

            // Si el dedo se movió a otra tecla distinta
            if (nota_detectada != ultima_nota_tocada) {
                
                // 1. Apagar nota anterior (si había una sonando)
                if (ultima_nota_tocada != -1) {
                    midi.noteOff(1, NOTAS_TECLADO[ultima_nota_tocada], 0);
                    synth.setNoteOff(1, NOTAS_TECLADO[ultima_nota_tocada], 0);
                }

                // 2. Encender nueva nota
                midi.noteOn(1, nota_midi, 127); // 127 = Volumen máximo (Estilo chiptune)
                synth.setNoteOn(1, nota_midi, 127);
                
                // Guardamos el estado para no repetir el disparo por ráfaga
                ultima_nota_tocada = nota_detectada;
                
                // Opcional: Podés pintar un pixel o cartelito indicando 
                // la nota activa para darle más feedback visual arcade.
            }
        }
    } else {
        // En el momento exacto en que se levanta el dedo de la pantalla
        if (ultima_nota_tocada != -1) {
            midi.noteOff(1, NOTAS_TECLADO[ultima_nota_tocada], 0);
            synth.setNoteOff(1, NOTAS_TECLADO[ultima_nota_tocada], 0);
            
            // Limpiamos rastro visual si es necesario y reseteamos el estado
            ultima_nota_tocada = -1;
        }
    }

    delay(10); // Estabilidad para los rebotes del touch
}