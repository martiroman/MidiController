

void scan_i2c(Arduino_RGB_Display* gfx) {
    debug_screen(gfx, "--- ESCANER I2C ---", WHITE);

    byte error, address;
    int nDevices = 0;
    char buffer[50];

    // 3. Barrido de direcciones
    for(address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            sprintf(buffer, "Encontrado: 0x%02X", address);
            
            // Pintamos de verde si es el chip de energía esperado
            uint16_t color = (address == 0x24) ? GREEN : YELLOW;
            debug_screen(gfx, buffer, color);
            nDevices++;
        }
    }

    if (nDevices == 0) {
        debug_screen(gfx, "Bus vacio. Nada detectado.", RED);
    } else {
        sprintf(buffer, "Escaneo fin. Total: %d", nDevices);
        debug_screen(gfx, buffer, WHITE);
    }
}
