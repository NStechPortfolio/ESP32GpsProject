#include "WifiModule/WifiModule.h"
#include <ESPmDNS.h>
#include <Arduino.h>

WifiModule::WifiModule(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
}

WifiModule::~WifiModule() {}

bool WifiModule::connect() {
    Serial.println("\n=================================");
    Serial.printf("Start Wifi connection: %s\n", ssid);
    Serial.println("=================================");

    // 明示的にSTA(Station)モードに設定
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startTime = millis();

    // 接続処理
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        // ウォッチドッグ
        if (millis() - startTime > CONNECTION_TIMEOUT) {
            Serial.println("\n[error] Wifi connection timeout.");
            return false;
        }
    }

    Serial.println("\n[SUCCESS] Wifi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
}

bool WifiModule::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}
