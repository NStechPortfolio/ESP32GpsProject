#include "TemperatureModule/TemperatureModule.h"
#include <Arduino.h>
#include <Wire.h>

TemperatureModule::TemperatureModule(){};

void TemperatureModule::begin() {
    Wire.begin(); // 標準ピン21, 22を自動的に利用
    
    // ADT7410を16ビット高解像度モードに設定
    Wire.beginTransmission(ADT7410_ADDR);
    Wire.write(ADT7410_REG_CONF);
    Wire.write(CONF_16BIT_RESOLUTION);
    Wire.endTransmission();

    Serial.println("[Temperature Module] Initialized (ADT7410 via I2C).");
}

double TemperatureModule::getTemperature() {
    // 温度データが格納されている先頭レジスタ(0x00)を指定
    Wire.beginTransmission(ADT7410_ADDR);
    Wire.write(TEMPERATURE_ADDR); 
    if (Wire.endTransmission() != 0) {
        Serial.println("[Error] ADT7410 not found on I2C bus.");
        return -999.0;
    }

    // 上位・下位の2バイト（16ビット分）のデータを要求
    Wire.requestFrom(ADT7410_ADDR, 2);
    
    if (Wire.available() >= 2) {
        uint8_t msb = Wire.read(); // 上位バイト
        uint8_t lsb = Wire.read(); // 下位バイト

        // 2つのバイトを1つの16ビット整数に結合
        int16_t rawData = (msb << 8) | lsb;

        // 摂氏温度に変換
        float tempC = rawData / 128.0;
        
        return tempC;
    }

    return -999.0;
}
