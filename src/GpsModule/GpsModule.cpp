#include "GpsModule/GpsModule.h"
#include <HardwareSerial.h>
#include <Arduino.h>
#include "RawData/Telemetry.h"

GpsModule::GpsModule(HardwareSerial& serial) : gpsSerial(serial){
  this->gpsSerial = serial;
}

GpsModule::~GpsModule(){
}

Telemetry GpsModule::fetchGPSData(){
  Telemetry telemetry;

  // GPSから流れてくるNMEAデータを常に読み込んでライブラリに流し込む
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // 定周期で現在の測位ステータスをシリアル画面出力
  if (millis() - lastDisplayTime >= displayInterval) {
    lastDisplayTime = millis();

    Serial.println("\n--- [GPS Status Report] ---");

    // 衛星の電波を捉え、位置情報が有効かどうか
    if (gps.location.isValid()) {

      // for debug
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6); // 小数点6桁まで表示
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
      Serial.print("Satellites tracked: ");
      Serial.println(gps.satellites.value());

      // 位置情報を格納する
      telemetry.latitude = gps.location.lat();
      telemetry.longitude = gps.location.lng();
      telemetry.timestamp = lastDisplayTime;
      
    } else {
      // 測位がまだ完了していない場合
      Serial.print("Status: now receiving ...\n");
      Serial.print("Satellites tracked: ");
      Serial.println(gps.satellites.value());
      Serial.println("Place antenna near window to get GPS fix.");
    }
  }

  return telemetry;
}