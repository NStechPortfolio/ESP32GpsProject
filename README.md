# ESP32 疑似人工衛星ファームウェア (Firmware Core)
本リポジトリは、疑似人工衛星機体(ESP32)に搭載された各種センサを制御し、地上局システムへテレメトリデータを送信するファームウェアソースコードである。

---

## 使用コンポーネント (Hardware Parts)
| パーツ名 | 概要 / 特徴 | メーカー |
| :--- | :--- | :--- |
| **ESP32-WROOM-32E** | メインマイコンボード (Wi-Fi / Bluetooth搭載) | Espressif Systems |
| **GT-502MGG-N** | GPS/GNSSモジュール(「みちびき」2機(194/195)対応、高感度) | YIC |
| **ADT7410** | 高精度・高分解能 I2C温度センサ(±0.5°C精度) | Analog Devices |

![全体図](img/overview.png)

---

## 🔌 回路配線図 (Pin Mapping)

```
    +-----------------------------------+
    |          ESP32-WROOM-32E          |
    |                                   |
    |   [3.3V] [GND] [GPIO21] [GPIO22]  |  [GPIO16] [GPIO17]  [5V]  [GND]
    +-----+------+------+------+--------+-----+-----+-----+-----+-----+-----+
          |      |      |      |              |     |           |      |      
          | VCC  | GND  | SDA  | SCL          | RX2 | TX2       | VCC  | GND  
          ▼      ▼      ▼      ▼              ▼     ▼           ▼      ▼      
    +-----------------------------------+   +-----------------+
    |         ADT7410 (I2C)             |   | GT-502MGG (UART)|
    |        高精度温度センサ           |   |   GPSモジュール |
    +-----------------------------------+   +-----------------+
```

### 詳細ピンマッピング表

| センサ側ピン | ESP32側ピン | 通信方式 / 役割 | 備考 |
| :--- | :--- | :--- | :--- |
| **共通 VCC** | **3.3V/5V** | 電源 | 各モジュールの定格電圧(3.3V)に準拠 |
| **共通 GND** | **GND** | グランド| 基準電位の共通化 |
| **GPS TX** | **GPIO 16 (RX2)** | UART (Serial2) | ESP32のハードウェアシリアル2番の標準RXピン |
| **GPS RX** | **GPIO 17 (TX2)** | UART (Serial2) | ハードウェアシリアル2番の標準TXピン |
| **ADT7410 SDA** | **GPIO 21** | I2C (Wire) | ESP32標準のハードウェアI2C(SDA)ピン |
| **ADT7410 SCL** | **GPIO 22** | I2C (Wire) | ESP32標準のハードウェアI2C(SCL)ピン |

---

## ファームウェア開発

### 使用ライブラリ
* **TinyGPS++**
* **ArduinoJson**
* **LiquidCrystal_I2C**

### 動作確認
モニタ出力例は以下の通り:
```text
[Temperature Module] Initialized (ADT7410 via I2C).
[GPS Module] UART Connection Opened at 9600bps.

--- [GPS Status Report] ---
Status: now receiving ...
Satellites tracked: 2
Place antenna near window to get GPS fix.

--- [GPS Status Report] ---
Latitude: 35.681234
Longitude: 139.767123
Satellites tracked: 8
GPS UTC Time: 2026/07/09 11:20:05.120
Current Temp: 24.58°C
```
