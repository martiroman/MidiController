# MidiController

Proyecto de controlador MIDI basado en ESP32-S3 pensado para interactuar con sintetizadores (por ejemplo: Korg NTS-1), pantallas RGB y control táctil. Este README describe el propósito del proyecto, la arquitectura software, el hardware requerido, las conexiones recomendadas y cómo compilar/subir el firmware.

**Resumen**
- **Plataforma**: ESP32-S3 (configurado en PlatformIO como `esp32-s3-43-waveshare`).
- **Pantalla**: Waveshare 4.3" RGB (panel RGB) con controlador táctil GT911.
- **Entrada MIDI**: USB Host MIDI (soporta dispositivos USB-MIDI) y opciones para salida a sintetizadores externos como NTS-1 (USB o DIN/TRS-MIDI según tu configuración).

**Características**
- Lectura de teclado/piano virtual y control táctil.
- Soporte de USB Host MIDI para recibir eventos desde teclados MIDI USB.
- Visualización en pantalla RGB y depuración por Serial/OTA.
- Módulos organizados: controlador táctil, UI (teclado/piano), configuración global.

**Requisitos de hardware (ejemplo mínimo)**
- ESP32-S3 (dev board o módulo compatible). Recomendado: placa objetivo `esp32-s3-43-waveshare`.
- Pantalla Waveshare 4.3" RGB compatible con `Arduino_ESP32RGBPanel`.
- Controlador táctil GT911 (I2C).
- Cable USB OTG (para conectar dispositivos USB-MIDI al ESP32 si usas USB Host) o adaptador USB Host compatible.
- Korg NTS-1 (opcional) — puede conectarse por USB (como dispositivo USB-MIDI) o por salida MIDI (TRS/DIN) si usas interfaz MIDI física.
- Fuente de alimentación 5V con suficiente corriente para la pantalla y el ESP.
- Cables dupont, protoboard o PCB para conexiones.

**Lista de materiales (BOM) sugerida**
- 1x ESP32-S3 Dev Board (o equivalente).
- 1x Waveshare 4.3" RGB display.
- 1x Módulo táctil GT911 (si la pantalla no lo integra).
- 1x Cable USB OTG / Host.
- 1x Korg NTS-1 (opcional).
- 1x Convertidor MIDI DIN optoaislado (si vas a usar MIDI DIN tradicional).

**Conexiones / Wiring (guía)**
Nota: el proyecto está pensado para usarse con la configuración por defecto en `src/shared/hardware.h`. Revisa ese archivo para los detalles del inicializador de pantalla y los pines utilizados.

- Pantalla RGB (Waveshare 4.3"):
  - Conecta la pantalla según las señales requeridas por el controlador RGB. La inicialización del panel se realiza en [src/shared/hardware.h](src/shared/hardware.h#L1).

- Táctil (GT911):
  - El GT911 usa I2C. Conecta SDA/SCL a los pines I2C del ESP32-S3 (configuración por defecto en el proyecto). Wire/I2C se inicializa en el firmware.

- USB Host / USB-MIDI:
  - Conecta un cable USB OTG desde el puerto USB Host del ESP32 (o el adaptador USB Host) al dispositivo USB-MIDI (teclado USB, NTS-1 en modo dispositivo USB, etc.). El soporte USB Host está referenciado en el proyecto (`usb/usb_host.h`).

- MIDI DIN/TRS (opcional, para NTS-1 por DIN):
  - Si prefieres usar MIDI DIN/optical isolation, coloca un circuito optoacoplador MIDI para la entrada MIDI del ESP32 y conecta la salida MIDI del controlador al optoacoplador. Nunca conectes directamente señales MIDI (sin aislamiento) a los pines del microcontrolador.

Ejemplo de topologías posibles:
- Conexión USB-MIDI (recomendada si tienes cable/adaptador):
  - Teclado USB -> USB OTG -> ESP32 USB Host -> firmware procesa eventos MIDI.
  - ESP32 -> (USB host) -> enviar MIDI a síntes por USB (si el síntetizador soporta USB Device).

- Conexión hardware MIDI (DIN/TRS):
  - Teclado/Módulo MIDI -> MIDI DIN -> circuito optoaislador -> UART RX del ESP32 (con el protocolo MIDI en UART a 31250 bps) o usa una interfaz dedicada.

**Mapeo de pines y referencias**
- La inicialización del panel y pines relevantes está en [src/shared/hardware.h](src/shared/hardware.h#L1).
- El controlador táctil y la lógica de interacción se encuentran en [src/controller/TouchController.cpp](src/controller/TouchController.cpp#L1) y [src/controller/TouchController.h](src/controller/TouchController.h#L1).
- La UI del teclado/piano está en [src/ui/keyboard/PianoKeyboard.cpp](src/ui/keyboard/PianoKeyboard.cpp#L1) y [src/ui/keyboard/PianoKeyboard.h](src/ui/keyboard/PianoKeyboard.h#L1).
- El punto de entrada del firmware es [src/main.cpp](src/main.cpp#L1).

**Software: compilar y subir (PlatformIO)**
1. Instala PlatformIO (VSCode PlatformIO extension) o usa la CLI `pio`.
2. Compilar y subir al target (ejemplo usando el entorno definido en `platformio.ini`):

```bash
pio run -e esp32-s3-43-waveshare -t upload
```

3. Monitor serial:

```bash
pio device monitor -b 115200
```

4. OTA: el firmware inicializa OTA si hay WiFi configurado. Revisa la función `init_wifi_ota()` en [src/shared/hardware.h](src/shared/hardware.h#L1) para ajustar SSID/password.


**Consejos de seguridad y buenas prácticas**
- Usa siempre aislamiento para conexiones MIDI DIN (optoacoplador).
- Alimenta la pantalla y el ESP con una fuente estable y con suficiente corriente.
- Verifica las tensiones antes de conectar módulos externos.

**Contribuciones**
Si quieres colaborar, abre un issue o un pull request con una descripción clara del cambio. Para grandes cambios, abre un issue primero para discutir el diseño.

**Licencia**
Por defecto no se incluye una licencia en este repo. Añade un archivo `LICENSE` con la licencia que prefieras (MIT, Apache-2.0, etc.) si quieres permitir que otros usen/forqueen el proyecto.

---

Si quieres, he añadido diagramas de conexiones y un esquema de optoaislador MIDI en la carpeta `docs`.

**Diagramas**
- Diagrama de wiring USB-MIDI y conexiones principales: [docs/wiring_usb_midi.svg](docs/wiring_usb_midi.svg)
- Esquema de optoaislador MIDI (MIDI IN): [docs/midi_opto.svg](docs/midi_opto.svg)
