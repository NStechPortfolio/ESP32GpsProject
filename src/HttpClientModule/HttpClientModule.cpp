#include "HttpClientModule/HttpClientModule.h"
#include <ArduinoJson.h>
#include <Arduino.h>

HttpClientModule::HttpClientModule(const char* url, const char* satelliteId) {
    this->serverUrl = url;
    this->satelliteId = satelliteId;
}

HttpClientModule::~HttpClientModule() {}

void HttpClientModule::postTelemetry(Telemetry& telemetry) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] Skip to post request. (Wifi connection failed.)");
        return;
    }

    HTTPClient http;

    // 宛先URLを設定
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // POSTリクエストの作成
    JsonDocument doc;
    doc["satelliteId"] = satelliteId;
    doc["latitude"]    = telemetry.latitude;
    doc["longitude"]   = telemetry.longitude;
    doc["temperature"] = telemetry.temperature;
    doc["timestamp"]   = telemetry.timestamp;

    // JSONオブジェクトを文字列にシリアライズ
    String jsonString;
    serializeJson(doc, jsonString);

    Serial.println("\n[HTTP] sending a POST request ...");
    Serial.print("[Payload] ");
    Serial.println(jsonString);

    // POSTリクエストを送信
    int httpResponseCode = http.POST(jsonString);

    // レスポンスの評価
    if (httpResponseCode > 0) {
        Serial.print("[HTTP] Response: ");
        Serial.println(httpResponseCode);
        
        // サーバーからの返却テキスト
        String response = http.getString();
        Serial.print("[Server Response] ");
        Serial.println(response);
    } else {
        Serial.print("[HTTP] An error has been occurred. ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}