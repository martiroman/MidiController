#include <LittleFS.h>

// Modificá tu array global si ya lo tenías definido como const
uint8_t notas[4] = {0, 0, 0, 0}; 

bool cargarNotasDesdeArchivo( Arduino_RGB_Display* gfx, const char* path) {
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