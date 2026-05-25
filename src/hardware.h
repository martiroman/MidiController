#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <usb/usb_host.h>
#include <LittleFS.h>

// CH422G Energy Control
#define CH422G_I2C_ADDR 0x24
#define CH422G_REG_MODE 0x01
#define CH422G_REG_OUT  0x02

inline void ch422g_write(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CH422G_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Init panel RGB de la Waveshare 4.3"
inline Arduino_RGB_Display* init_display() {
    auto *bus = new Arduino_ESP32RGBPanel(
        5, 3, 46, 7, 1, 2, 42, 41, 40, 39, 0, 45, 48, 47, 21,
        14, 38, 18, 17, 10, 0, 8, 4, 8, 0, 8, 4, 8, 1, 14000000
    );
    auto *gfx = new Arduino_RGB_Display(800, 480, bus);
    gfx->begin();
    gfx->fillScreen(BLACK);
    return gfx;
}

inline void init_wifi_ota() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("thecooders", "apuki2018");
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    ArduinoOTA.setHostname("microkorg-esp32");
    ArduinoOTA.begin();
}

// --- Asegurate de incluir esto al final de hardware.h ---
inline void debug_screen(Arduino_RGB_Display* gfx, const char* msg, uint16_t color = WHITE) {
    static int line = 0;
    if (line > 15) { gfx->fillScreen(BLACK); line = 0; }
    gfx->setCursor(10, 10 + (line * 25));
    gfx->setTextColor(color);
    gfx->setTextSize(2);
    gfx->println(msg);
    line++;
    Serial.println(msg);
}

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

bool cargarNotasDesdeArchivo( Arduino_RGB_Display* gfx, const char* path) {
    uint8_t notas[4] = {0, 0, 0, 0};
    // Si pasamos 'true', si no puede montarlo, lo formatea automáticamente
    if (!LittleFS.begin(true)) {
        debug_screen(gfx, "Error crítico: No se pudo montar ni formatear LittleFS");
        return false;
    }

    // Comprobamos si el archivo existe. Si no existe (porque falló la subida OTA), lo creamos nosotros
    if (!LittleFS.exists(path)) {
        debug_screen(gfx, "Archivo no encontrado. Creando acorde por defecto en la memoria...");
        File archivoEscritura = LittleFS.open(path, "w");
        if (archivoEscritura) {
            archivoEscritura.println("60,63,67,70"); // Escribimos un acorde por defecto (Do menor 7)
            archivoEscritura.close();
            debug_screen(gfx, "Archivo creado con éxito.", GREEN);
        } else {
            debug_screen(gfx, "Error: No se pudo crear el archivo.", RED);
            return false;
        }
    }

    // Ahora que nos aseguramos de que el archivo existe, lo leemos de forma normal
    File file = LittleFS.open(path, "r");
    if (!file) {
        debug_screen(gfx, "Error al abrir el archivo para lectura", RED);
        return false;
    }

    String contenido = file.readStringUntil('\n');
    contenido.trim();
    file.close();

    // Parsear las notas separadas por comas
    int idx_nota = 0;
    int posicion_inicio = 0;
    while (idx_nota < 4) {
        int posicion_coma = contenido.indexOf(',', posicion_inicio);
        String nota_str;
        if (posicion_coma == -1) {
            nota_str = contenido.substring(posicion_inicio);
            notas[idx_nota] = nota_str.toInt();
            break;
        } else {
            nota_str = contenido.substring(posicion_inicio, posicion_coma);
            notas[idx_nota] = nota_str.toInt();
            posicion_inicio = posicion_coma + 1;
            idx_nota++;
        }
    }
    return true;
}