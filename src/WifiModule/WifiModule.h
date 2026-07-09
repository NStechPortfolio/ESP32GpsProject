#pragma once
#include <WiFi.h>

class WifiModule {
    private:
        const char* ssid;
        const char* password;
        const unsigned long CONNECTION_TIMEOUT = 15000; // 15秒でタイムアウト

    public:
        WifiModule(const char* ssid, const char* password);
        ~WifiModule();

        bool connect();
        bool isConnected();
};