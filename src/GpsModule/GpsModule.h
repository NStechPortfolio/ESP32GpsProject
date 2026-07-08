#pragma once
#include <TinyGPS++.h>
#include "RawData/Telemetry.h"

class GpsModule {
    private:
        TinyGPSPlus gps;
        HardwareSerial& gpsSerial;

    public:
        GpsModule(HardwareSerial& gpsSerial);
        ~GpsModule();
        void fetchGPSData(Telemetry& telemetry);
        String getTimestamp();
};