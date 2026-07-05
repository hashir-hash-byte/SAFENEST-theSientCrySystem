<div align="center">

<img src="https://img.shields.io/badge/SafeNest-Baby%20Monitor-FF6B9D?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZmlsbD0id2hpdGUiIGQ9Ik0xMiAyQzYuNDggMiAyIDYuNDggMiAxMnM0LjQ4IDEwIDEwIDEwIDEwLTQuNDggMTAtMTBTMTcuNTIgMiAxMiAyek0xMiA2YzEuNjYgMCAzIDEuMzQgMyAzcy0xLjM0IDMtMyAzLTMtMS4zNC0zLTMgMS4zNC0zIDMtM3ptMCAxNGMtMi41IDAtNC43MS0xLjI4LTYtMy4yMi4wMy0xLjk5IDQtMy4wOCA2LTMuMDggMS45OSAwIDUuOTcgMS4wOSA2IDMuMDhDMTYuNzEgMTguNzIgMTQuNSAyMCAxMiAyMHoiLz48L3N2Zz4=&logoColor=white" alt="SafeNest" />

# 🍼 SafeNest
### *Smart Baby Monitoring System for Deaf Mothers*

**Real-time cry detection · Roll-over alerts · Bluetooth watch · WiFi dashboard**

> 💡 **Because every mother deserves to keep her baby safe — sound or not.**

<br/>

[![ESP32](https://img.shields.io/badge/ESP32-IoT_Device-E7352C?style=flat-square&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Firmware-orange?style=flat-square&logo=platformio&logoColor=white)](https://platformio.org/)
[![Netlify](https://img.shields.io/badge/Netlify-Web_Dashboard-00C7B7?style=flat-square&logo=netlify&logoColor=white)](https://netlify.app/)
[![C++](https://img.shields.io/badge/C++-Firmware-00599C?style=flat-square&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Bluetooth](https://img.shields.io/badge/Bluetooth-Classic-0082FC?style=flat-square&logo=bluetooth&logoColor=white)]()
[![Status](https://img.shields.io/badge/Status-Working_Prototype-brightgreen?style=flat-square)]()

<br/>

```
┌──────────────────────────────────────────────────────────┐
│  Crib Unit  →  Bluetooth  →  Watch Unit  →  Vibrate      │
│  Detects cry    or WiFi      OLED shows     + Display    │
│  & roll-over    switch       "Baby Crying"  alert        │
└──────────────────────────────────────────────────────────┘
```

</div>

---

## 📖 Table of Contents

- [Problem Statement](#-problem-statement)
- [Solution Overview](#-solution-overview)
- [System Architecture](#-system-architecture)
- [Live Demo](#-live-demo)
- [Features](#-features)
- [Hardware Setup](#-hardware-setup)
- [Pin Wiring](#-pin-wiring)
- [Alert Behavior](#-alert-behavior)
- [Dual Mode System](#-dual-mode-system)
- [How To Flash Firmware](#-how-to-flash-firmware)
- [Web Dashboard Setup](#-web-dashboard-setup)
- [Folder Structure](#-folder-structure)
- [Limitations](#-limitations)
- [Roadmap](#-roadmap)
- [Author](#-author)

---

## ❗ Problem Statement

Traditional baby monitors are broken for deaf caregivers:

| Problem | Impact |
|---|---|
| All alerts are audio-only | Deaf mothers hear nothing |
| No tactile feedback | Can't feel alerts while busy |
| No rollover detection | Safety risk — baby can suffocate |
| No remote monitoring | Useless when mother is bathing / away |
| Expensive dedicated devices | Not affordable for most families |

> **A deaf mother puts her baby to sleep. Baby rolls over. Audio alarm screams. She hears nothing. Baby is in danger.**
>
> SafeNest eliminates every one of these failure points.

---

## 💡 Solution Overview

A **dual-mode IoT baby monitoring system** built for deaf and hard-of-hearing mothers that:

- 🎙️ Uses **ESP32 + Microphone** to detect baby crying in real time
- 📐 Uses **MPU6050** to detect dangerous baby roll-overs
- ⌚ Sends **Bluetooth alerts to a wearable Watch Unit** — vibrates + shows message on OLED
- 🌐 Switches to **WiFi mode** for a live web dashboard when watch can't be worn
- 📊 Logs **timestamped alert history** — cry events, roll-over events, total alerts

---

## 🏗️ System Architecture

```
┌──────────────────────────────────────────┐
│              CRIB UNIT                   │
│                                          │
│  [Microphone] ──► cry detection          │
│  [MPU6050]    ──► roll-over detection    │
│  [Button]     ──► switch BT ↔ WiFi       │
│  [LED]        ──► mode indicator (red)   │
│                                          │
│           ESP32 (main controller)        │
└────────────┬─────────────────────────────┘
             │
     ┌───────┴────────┐
  BT MODE          WiFi MODE
  (default)        (button press)
     │                  │
     ▼                  ▼
┌─────────────┐   ┌──────────────────────────┐
│  WATCH UNIT │   │    WEB DASHBOARD         │
│             │   │  (hosted on Netlify)     │
│  ESP32      │   │                          │
│  Vibration  │   │  • Roll-Over Detected ⚠️  │
│  Motor      │   │  • Baby Crying 😢        │
│  OLED:      │   │  • Alert history log     │
│  "ALL SAFE" │   │  • Cry / Move counters   │
│  "CRYING!"  │   │  • Live ESP32 polling    │
│  "TILTED!"  │   │                          │
└─────────────┘   └──────────────────────────┘
```

---

## 📸 Live Demo

> **Real working prototype — built and tested July 2025**

### ⌚ Watch Unit — OLED showing "ALL SAFE"
> ESP32 mounted inside a smartwatch strap enclosure. OLED displays live baby status with battery indicator.

### 🛏️ Crib Unit — Prototype enclosure
> Cardboard enclosure with labelled mic hole, BT/WiFi mode button, MPU6050 motion sensor, and red LED mode indicator.

### 🌐 Web Dashboard — Roll-Over Alert LIVE
> Hosted on Netlify. Showing "Roll-Over Detected! Check on baby immediately" with ESP32 at `172.20.10.5`, polling every 3 seconds.

### 📊 Stats & Alert History
> 3 Cry Events · 3 Roll-Over Events · 6 Total Alerts — all timestamped (CRY / MOVE badges).

---

## ✨ Features

### 📡 Real-Time Detection
- Microphone samples every loop — cry detected via amplitude threshold + filter
- MPU6050 detects tilt angle beyond safe limit → roll-over alert
- Watch unit updates instantly over Bluetooth

### ⌚ Watch Unit Alerts
- **Baby Crying** → vibrates + OLED shows `Baby Crying!`
- **Baby Rolled** → vibrates + OLED shows `Baby Tilted!`
- **All Clear** → OLED shows `ALL SAFE` with green dot

### 🌐 Web Dashboard (WiFi Mode)
- Hosted on **Netlify** — open any phone browser, no app install
- Connect via ESP32 IP address
- Shows: Cry status · Roll-over status · ONLINE indicator
- Today's stats: Cry Events · Roll-Over Events · Total Alerts · Uptime
- Full timestamped **Alert History** (CRY / MOVE badges)

---

## 🔧 Hardware Setup

### Crib Unit

| Component | Model | Purpose |
|---|---|---|
| Microcontroller | ESP32 Dev Board | Main controller, BT + WiFi |
| Motion Sensor | MPU6050 (6-axis IMU) | Roll-over / tilt detection |
| Microphone | KY-038 or MAX4466 | Baby cry detection |
| Push Button | Momentary SPST | Switch BT ↔ WiFi mode |
| LED Indicator | Red LED | Mode status indicator |
| Power | 5V USB | Unit power |

### Watch Unit

| Component | Model | Purpose |
|---|---|---|
| Microcontroller | ESP32 Dev Board | Receiver controller |
| Vibration Motor | Coin ERM 3V DC | Tactile alert |
| Display | 0.96" OLED I2C | Visual status messages |
| Motor Driver | NPN Transistor 2N2222 | Drive motor safely |
| Power | USB (prototype) | Power supply |

---

## 📌 Pin Wiring

### Crib Unit (ESP32)

| Component | ESP32 Pin |
|---|---|
| MPU6050 SDA | GPIO 21 |
| MPU6050 SCL | GPIO 22 |
| MPU6050 VCC | 3.3V |
| MPU6050 GND | GND |
| Microphone OUT | GPIO 34 (ADC) |
| Microphone VCC | 3.3V |
| Mode Button | GPIO 0 |
| LED Indicator | GPIO 2 |

### Watch Unit (ESP32)

| Component | ESP32 Pin |
|---|---|
| Vibration Motor (via transistor) | GPIO 25 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC | 3.3V |
| OLED GND | GND |

---

## 🚨 Alert Behavior

| Event | Watch Vibration | OLED Message | Dashboard |
|---|---|---|---|
| 🍼 Baby Crying | Vibrates **8 sec** | `Baby Crying!` | CRY alert + log |
| ⚠️ Baby Rolled Over | Vibrates **10 sec** | `Baby Tilted!` | MOVE alert + log |
| ✅ All Clear | Silent | `ALL SAFE 🟢` | Normal status |

---

## 📡 Dual Mode System

### 🔵 Bluetooth Mode (Default)
- Crib Unit ↔ Watch Unit via **Bluetooth Classic (ESP-SPP)**
- Instant low-latency alerts
- Range: ~10m line-of-sight
- Best for: mother wearing watch nearby

### 🌐 WiFi Mode (Button Activated)
- Press button on Crib Unit → BT disconnects → ESP32 joins home WiFi
- Sends real-time data to Netlify-hosted web dashboard
- Mother checks any phone browser — zero app install
- Best for: bathing, swimming, doing chores without watch

---

## ⚡ How To Flash Firmware

### Prerequisites
- [VS Code](https://code.visualstudio.com/) + [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
- ESP32 board + USB data cable
- Libraries: `MPU6050`, `BluetoothSerial`, `Adafruit_SSD1306`, `ESPAsyncWebServer`, `ArduinoJson`

### Flash Crib Unit

```bash
git clone https://github.com/hashir-hash-byte/SAFENEST-theSientCrySystem.git
cd SAFENEST-theSientCrySystem/crib_unit
code .

# PlatformIO → click Upload (→) or:
pio run --target upload
pio device monitor --baud 115200
```

### Flash Watch Unit

```bash
cd SAFENEST-theSientCrySystem/watch_unit
code .
pio run --target upload
pio device monitor --baud 115200
```

> ⚠️ Flash **Crib Unit first**. Power both → Bluetooth pairs automatically.

---

## 🌐 Web Dashboard Setup

Dashboard live on **Netlify** — no server, no install.

```
1. Press button on Crib Unit → WiFi mode activated
2. Open Serial Monitor → note ESP32 IP address
   Example: 172.20.10.5
3. Open dashboard in browser
4. Enter IP → click Connect
5. Status goes ONLINE — polls every 3 seconds
```

### ESP32 API

```
GET http://{ESP32_IP}/status
```

```json
{
  "crying": false,
  "tilted": true,
  "cryCount": 3,
  "tiltCount": 3,
  "totalAlerts": 6,
  "uptime": 420
}
```

---

## 📁 Folder Structure

```
SAFENEST-theSientCrySystem/
│
├── crib_unit/               # Crib Unit firmware (PlatformIO)
│   ├── src/
│   │   └── main.cpp         # Cry + tilt detection, BT/WiFi switch
│   ├── include/
│   └── platformio.ini
│
├── watch_unit/              # Watch Unit firmware (PlatformIO)
│   ├── src/
│   │   └── watchUnit.cpp    # BT receiver, vibration, OLED
│   ├── include/
│   └── platformio.ini
│
├── web_dashboard/           # WiFi mode dashboard
│   └── index.html           # Hosted on Netlify
│
├── docs/
│   └── SafeNest_Phase1_Report.docx
│
├── .gitignore
└── README.md
```

---

## 🚧 Limitations

| Limitation | Detail |
|---|---|
| 🎙️ Cry detection | Threshold-based — loud noise can false-trigger |
| 📶 Bluetooth range | ~10m max, walls reduce it |
| 🌐 WiFi dependency | Web mode needs local WiFi |
| 🔋 No battery yet | Watch USB-powered in prototype |
| 🏭 Cardboard enclosure | Not waterproof or drop-proof |
| 🔄 Manual mode switch | Button press required to toggle BT ↔ WiFi |

---

## 🗺️ Roadmap

- [x] Crib Unit — cry detection (mic + threshold filter)
- [x] Crib Unit — roll-over detection (MPU6050)
- [x] Crib Unit — BT ↔ WiFi mode switch button
- [x] Crib Unit — LED mode indicator
- [x] Watch Unit — vibration alerts (8s cry / 10s roll)
- [x] Watch Unit — OLED display (ALL SAFE / Baby Crying / Baby Tilted)
- [x] Bluetooth communication (Crib ↔ Watch)
- [x] Web dashboard — live alerts on Netlify
- [x] Timestamped alert history (CRY / MOVE)
- [x] Today's stats (cry count, roll count, total, uptime)
- [ ] LiPo battery for wireless Watch Unit
- [ ] AI-based cry detection (CNN on ESP32)
- [ ] 3D printed enclosures
- [ ] Deep sleep + power optimization
- [ ] Mobile app (Flutter)
- [ ] Multi-caregiver support

---

## 👨‍💻 Author

<div align="center">

**Mohammed Hashir**
*Computer Science & Engineering*
*Bearys Institute of Technology, Mangaluru*
*USN: 4BP24CS035*

[![GitHub](https://img.shields.io/badge/GitHub-hashir--hash--byte-181717?style=for-the-badge&logo=github)](https://github.com/hashir-hash-byte)

<br/>

*Built with ❤️ for accessibility — because every mother deserves to keep her baby safe.*

</div>

---

<div align="center">

⭐ **Star this repo if SafeNest inspired you!** ⭐

</div>
