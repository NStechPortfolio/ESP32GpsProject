#include "GpsModule/GpsModule.h"
#include <HardwareSerial.h>
#include <Arduino.h>
#include "RawData/Telemetry.h"

GpsModule::GpsModule(HardwareSerial& serial) : gpsSerial(serial){
  this->gpsSerial = serial;
}

GpsModule::~GpsModule(){
}

/** 定周期で現在の測位ステータスをシリアル画面出力 */
void GpsModule::fetchGPSData(Telemetry& telemetry){

  // GPSから流れてくるNMEAデータを常に読み込んでライブラリに流し込む
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

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
    telemetry.timestamp = this->getTimestamp();
  } else {
    // 測位がまだ完了していない場合
    telemetry.latitude = 0.0;
    telemetry.longitude = 0.0;
    telemetry.timestamp = this->getTimestamp();
    Serial.print("Status: now receiving ...\n");
    Serial.print("Satellites tracked: ");
    Serial.println(gps.satellites.value());
    Serial.println("Place antenna near window to get GPS fix.");
  }
}

String GpsModule::getTimestamp() {
  // 衛星を捕捉できない場合
  if (!gps.date.isValid() || !gps.time.isValid()) {
    return "0000/00/00 00:00:00.000";
  }

  int year = gps.date.year();
  int month = gps.date.month();
  int day = gps.date.day();
  int hour = gps.time.hour(); // UTC
  int minute = gps.time.minute();
  int second = gps.time.second();

  // ミリ秒に変換
  int ms = gps.time.centisecond() * 10;

  // yyyy/mm/dd hh/mm/ss.xxx 形式
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d %02d:%02d:%02d.%03d",
           year, month, day, hour, minute, second, ms);

  return String(buffer);
}