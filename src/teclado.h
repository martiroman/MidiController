#include <Arduino.h>

// Notas MIDI: Do4 (60) hasta Do5 (72)
const uint8_t NOTAS_TECLADO[13] = {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72};

// Estética 8-bit: Paleta de colores puros y contrastantes
#define COLOR_FONDO     0x0000 // Negro Absoluto
#define COLOR_BLANCA    0xFFFF // Blanco Puro
#define COLOR_NEGRA     0x10A2 // Azul Oscuro / Negro Arcade
#define COLOR_PRESION   0xF800 // Rojo Retro (para feedback visual)
#define COLOR_BORDE     0x0000 // Bordes negros y gruesos

// Configuración de geometría (Ajustable según tu pantalla)
const int TECLAS_BLANCAS = 8; // Do, Re, Mi, Fa, Sol, La, Si, Do
const int ANCHO_PANTALLA = 480;
const int ALTO_PANTALLA  = 320;

const int ANCHO_W = ANCHO_PANTALLA / TECLAS_BLANCAS; // ~60 px cada blanca
const int ALTO_W  = ALTO_PANTALLA - 40;              // Deja espacio arriba para texto
const int ANCHO_B = (ANCHO_W * 7) / 10;              // Negra es un 70% del ancho de la blanca
const int ALTO_B  = (ALTO_W * 6) / 10;               // Negra ocupa el 60% del alto

// Mapeo físico de las 13 teclas (Índices)
// Blancas: 0=C, 2=D, 4=E, 5=F, 7=G, 9=A, 11=B, 12=C
// Negras:  1=C#, 3=D#, 6=F#, 8=G#, 10=A#
int ultima_nota_tocada = -1;


void dibujarTecladoRetro(Arduino_RGB_Display* gfx) {
    gfx->fillScreen(COLOR_FONDO);
    
    // Encabezado con estética 8-bit
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(2); // Texto pixelado grande
    gfx->setCursor(10, 10);
    gfx->print("ESP-8BIT SYNTH // OCTAVE 1");

    // 1. Dibujar Teclas Blancas
    int idx_blanca = 0;
    for (int i = 0; i < 13; i++) {
        if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10) continue; // Saltear negras
        
        int x = idx_blanca * ANCHO_W;
        int y = 40;
        
        // Rectángulo relleno + borde grueso pixelado
        gfx->fillRect(x, y, ANCHO_W, ALTO_W, COLOR_BLANCA);
        gfx->drawRect(x, y, ANCHO_W, ALTO_W, COLOR_BORDE);
        gfx->drawRect(x + 1, y + 1, ANCHO_W - 2, ALTO_W - 2, COLOR_BORDE); // Doble borde
        
        idx_blanca++;
    }

    // 2. Dibujar Teclas Negras (Encima de los valles de las blancas)
    idx_blanca = 0;
    for (int i = 0; i < 12; i++) {
        // Si la nota actual es blanca, avanzamos el contador físico pero no dibujamos nada
        if (i != 1 && i != 3 && i != 6 && i != 8 && i != 10) {
            idx_blanca++;
            continue;
        }

        // La negra se posiciona justo en la intersección de dos blancas
        int x = (idx_blanca * ANCHO_W) - (ANCHO_B / 2);
        int y = 40;

        gfx->fillRect(x, y, ANCHO_B, ALTO_B, COLOR_NEGRA);
        gfx->drawRect(x, y, ANCHO_B, ALTO_B, COLOR_BORDE);
        gfx->drawRect(x + 1, y + 1, ANCHO_B - 2, ALTO_B - 2, COLOR_BORDE);
    }
}

int obtenerNotaDesdeTouch(int tx, int ty) {
    if (ty < 40 || ty > ALTO_PANTALLA) return -1; // Fuera del teclado

    // --- PASO 1: Verificar si tocó una tecla Negra (Zona Superior) ---
    if (ty <= (40 + ALTO_B)) {
        int idx_blanca = 0;
        for (int i = 0; i < 12; i++) {
            if (i != 1 && i != 3 && i != 6 && i != 8 && i != 10) {
                idx_blanca++;
                continue;
            }
            
            int x_negra = (idx_blanca * ANCHO_W) - (ANCHO_B / 2);
            if (tx >= x_negra && tx <= (x_negra + ANCHO_B)) {
                return i; // Encontró negra tocada
            }
        }
    }

    // --- PASO 2: Si no fue negra, cae en una blanca obligatoriamente ---
    int columna_blanca = tx / ANCHO_W;
    if (columna_blanca >= TECLAS_BLANCAS) columna_blanca = TECLAS_BLANCAS - 1;

    // Mapeo inverso de columna blanca a índice de nota real
    int mapeo_blancas[] = {0, 2, 4, 5, 7, 9, 11, 12};
    return mapeo_blancas[columna_blanca];
}

