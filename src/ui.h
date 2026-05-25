#pragma once
#include <Arduino_GFX_Library.h>

struct Boton {
    // Boton btnPlay = {280, 180, 240, 100, "PLAY/STOP", GREEN, BLUE, &arp_activo};

    int x, y, w, h;
    const char* label;
    uint16_t colorActivo;
    uint16_t colorInactivo;
    bool* estadoAsociado; // Puntero a la variable que controla (ej: arp_activo)

    void dibujar(Arduino_RGB_Display* gfx) {
        uint16_t color = *estadoAsociado ? colorActivo : colorInactivo;
        gfx->fillRoundRect(x, y, w, h, 14, color);
        gfx->setTextColor(WHITE);
        gfx->setTextSize(3);
        gfx->setCursor(x + 20, y + (h/2) - 10);
        gfx->print(label);
    }

    bool checkToque(int tx, int ty) {
        if (tx >= x && tx <= x + w && ty >= y && ty <= y + h) {
            *estadoAsociado = !(*estadoAsociado); // Conmuta el estado
            return true;
        }
        return false;
    }
};