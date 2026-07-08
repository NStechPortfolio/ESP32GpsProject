#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include "RawData/Telemetry.h"

class HttpClientModule {
    private:
        const char* serverUrl;
        const char* satelliteId;

    public:
        HttpClientModule(const char* url, const char* satelliteId);
        ~HttpClientModule();

        // 位置情報と温度情報を外部サーバにPOSTする
        void postTelemetry(Telemetry telemetry);
};