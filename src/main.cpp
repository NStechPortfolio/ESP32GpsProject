#include <Arduino.h>
#include <Wire.h>
#include "WifiModule/WifiModule.h"
#include "GpsModule/GpsModule.h"
#include "HttpClientModule/HttpClientModule.h"
#include "TemperatureModule/TemperatureModule.h"
#include "RawData/Telemetry.h"


// Wi-Fi 設定(2.4GHz のみ対応)
const char* WIFI_SSID = "elecom-854d80";
const char* WIFI_PASS = "ckdvr9hm24en";

// 定周期処理用のタイマー
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 10000; // 10秒

// バックエンドサーバ設定
const char* SATELLITE_ID = "ESP32-STATION-01";
String query = String("http://") + SERVER_IPADDRESS + ":" + SERVER_PORT + API_URI; // この変数値はplatformio.ini で定義されているので実装で定義する必要はない
HttpClientModule httpClientModule = HttpClientModule(query.c_str(), SATELLITE_ID);

// 各モジュール設定初期化
HardwareSerial gpsSerial(2); 
GpsModule gpsModule = GpsModule(gpsSerial);
TemperatureModule temperatureModule;


void setup() {
  // シリアル初期化
  Serial.begin(115200); 
  gpsSerial.begin(9600, SERIAL_8N1, GpsModule::RDX_BUS, GpsModule::TDX_BUS); 

  // 温度センサ初期化
  temperatureModule.begin();

  // Wifi 設定
  WifiModule wifiModule(WIFI_SSID, WIFI_PASS);
  wifiModule.connect();

  Serial.println("=================================");
  Serial.println("Start GPS receival task");
  Serial.println("=================================");

}

void loop() {
  // 定周期でテレメトリを送信
  if (millis() - lastSendTime < SEND_INTERVAL) {
    return;
  }
  lastSendTime = millis();

  Telemetry telemetry;
  gpsModule.fetchGPSData(telemetry);
  telemetry.temperature = temperatureModule.getTemperature();

  Serial.println("\n💡 [Timer Trigger] Start sending telemetry process ...");

  // HTTP POSTを実行
  httpClientModule.postTelemetry(telemetry);
}
