#include <Arduino.h>

#define COLOR_FONDO     0x0000 
#define COLOR_BLANCA    0xFFFF 
#define COLOR_NEGRA     0x10A2 
#define COLOR_BORDE     0x0000 

const int TECLAS_BLANCAS = 8; 
const int ANCHO_PANTALLA = 800;
const int ALTO_PANTALLA  = 480;

const int ANCHO_W = ANCHO_PANTALLA / TECLAS_BLANCAS; // 100 px cada blanca exactos
const int ALTO_W  = ALTO_PANTALLA - 110;              
const int ANCHO_B = (ANCHO_W * 6) / 10;              // 60% del ancho de la blanca queda mejor
const int ALTO_B  = (ALTO_W * 6) / 10;               // 60% del alto

int ultima_nota_tocada = -1; // Para evitar disparos repetidos al mantener el dedo en la misma tecla

void drawKeyboard(Arduino_RGB_Display* gfx) {
    gfx->fillScreen(COLOR_FONDO);
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(2); 
    gfx->setCursor(10, 10);
    gfx->print("HOLA ALMENDRA - ESP-8BIT SYNTH // OCTAVE 1");

    // 1. Dibujar Teclas Blancas (De izquierda a derecha de forma natural)
    int idx_blanca = 0;
    for (int i = 0; i < 13; i++) {
        // Notas negras: Do#(1), Re#(3), Fa#(6), Sol#(8), La#(10)
        if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10) continue; 
        
        int x = idx_blanca * ANCHO_W;
        int y = 60;
        
        gfx->fillRect(x, y, ANCHO_W, ALTO_W, COLOR_BLANCA);
        gfx->drawRect(x, y, ANCHO_W, ALTO_W, COLOR_BORDE);
        gfx->drawRect(x + 1, y + 1, ANCHO_W - 2, ALTO_W - 2, COLOR_BORDE); 
        
        idx_blanca++;
    }

    // 2. Dibujar Teclas Negras (Centradas en los valles correctos)
    idx_blanca = 0;
    for (int i = 0; i < 12; i++) {
        // Si es blanca, sumamos al contador físico de posición y saltamos
        if (i != 1 && i != 3 && i != 6 && i != 8 && i != 10) {
            idx_blanca++;
            continue;
        }

        // x se posiciona en la intersección exacta de las blancas
        int x = (idx_blanca * ANCHO_W) - (ANCHO_B / 2);
        int y = 60;

        gfx->fillRect(x, y, ANCHO_B, ALTO_B, COLOR_NEGRA);
        gfx->drawRect(x, y, ANCHO_B, ALTO_B, COLOR_BORDE);
        gfx->drawRect(x + 1, y + 1, ANCHO_B - 2, ALTO_B - 2, COLOR_BORDE);
    }
}

int getKeyboardNote(int tx, int ty) {
    tx = ANCHO_PANTALLA - tx; 

    if (ty < 60 || ty > ALTO_PANTALLA) return -1; 

    // 1. Verificar si toco una tecla negra (Zona Superior)
    if (ty >= 60 && ty <= (60 + ALTO_B + 15)) { // 15 píxeles de tolerancia hacia abajo para facilitar el toque de las negras
        int idx_blanca = 0;
        for (int i = 0; i < 12; i++) {
            if (i != 1 && i != 3 && i != 6 && i != 8 && i != 10) {
                idx_blanca++;
                continue;
            }
            
            int x_negra = (idx_blanca * ANCHO_W) - (ANCHO_B / 2);
            if (tx >= x_negra && tx <= (x_negra + ANCHO_B)) {
                return i; // Devuelve el indice MIDI correcto (1, 3, 6, 8 o 10)
            }
        }
    }

    // 2. Si no fue negra, cae en una blanca de forma lineal estandar
    int columna_blanca = tx / ANCHO_W;
    if (columna_blanca >= TECLAS_BLANCAS) columna_blanca = TECLAS_BLANCAS - 1;

    // Mapeo directo y natural de izquierda a derecha: C, D, E, F, G, A, B, C
    int mapeo_blancas[] = {0, 2, 4, 5, 7, 9, 11, 12};
    return mapeo_blancas[columna_blanca];
}