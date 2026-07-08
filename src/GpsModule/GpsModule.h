#pragma once
#include <TinyGPS++.h>
#include "RawData/Telemetry.h"

class GpsModule {
    private:
        TinyGPSPlus gps;
        HardwareSerial& gpsSerial;
        unsigned long lastDisplayTime = 0;
        const unsigned long displayInterval = 5000; // 5秒ごとにシリアルに状況を表示

    public:
        GpsModule(HardwareSerial& gpsSerial);
        ~GpsModule();
        Telemetry fetchGPSData();
};