#include <Arduino.h>
#include "../config/CH422GCfg.h"
#include <Wire.h>

inline void ch422g_write(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CH422G_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

inline void initWire(){
    Wire.begin(8, 9);
}