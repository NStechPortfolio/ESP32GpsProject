#pragma once
#include <TinyGPS++.h>
#include "RawData/Telemetry.h"

class GpsModule {
    private:
        TinyGPSPlus gps;
        HardwareSerial& gpsSerial;

    public:
        static constexpr unsigned long RDX_BUS = 16;
        static constexpr unsigned long TDX_BUS = 17;
        GpsModule(HardwareSerial& gpsSerial);
        ~GpsModule();
        void fetchGPSData(Telemetry& telemetry);
        String getTimestamp();
};