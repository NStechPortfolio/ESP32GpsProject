#pragma once

class TemperatureModule {
    private:
        // I2C設定
        const int ADT7410_ADDR = 0x48; // I2Cアドレス
        const int ADT7410_REG_CONF = 0x03; // 高精細モードの設定レジスタ
        const int CONF_16BIT_RESOLUTION = 0x80; // 高精細モードON/OFF切替
        const int TEMPERATURE_ADDR = 0x00; // 温度データの格納先アドレス
    public:
        TemperatureModule();
        void begin();
        double getTemperature();
};