#include <Arduino.h>
#include <BLEDevice.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define VIB_MOTOR_PIN 2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static boolean doConnect = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

String currentState = "SAFE";
unsigned long lastAnimFrame = 0;
int animFrame = 0;

void triggerVibration(int count, int durationOn, int durationOff) {
 for (int i = 0; i < count; i++) {
   digitalWrite(VIB_MOTOR_PIN, HIGH);
   delay(durationOn);
   digitalWrite(VIB_MOTOR_PIN, LOW);
   delay(durationOff);
 }
}

// ── BATTERY ICON ──
void drawBattery() {
 display.drawRect(0, 0, 20, 9, SSD1306_WHITE);
 display.fillRect(20, 2, 2, 5, SSD1306_WHITE);
 display.fillRect(2, 2, 5, 5, SSD1306_WHITE);
 display.fillRect(8, 2, 5, 5, SSD1306_WHITE);
 display.fillRect(14, 2, 4, 5, SSD1306_WHITE);
}

// ── HEADER (shared across all screens) ──
void drawHeader() {
 drawBattery();
 display.setTextSize(1);
 display.setTextColor(SSD1306_WHITE);
 display.setCursor(28, 1);
 display.print("SafeNest Watch");
 display.drawLine(0, 11, 128, 11, SSD1306_WHITE);
}

// ── HEART SHAPE ──
void drawHeart(int x, int y) {
 display.fillCircle(x + 4,  y + 4, 4, SSD1306_WHITE);
 display.fillCircle(x + 10, y + 4, 4, SSD1306_WHITE);
 display.fillTriangle(x, y + 6, x + 14, y + 6, x + 7, y + 14, SSD1306_WHITE);
}

// ── SAFE ANIMATION ──
void showSafeAnimation() {
 int offsets[] = {0, -2, -4, -2, 0, 2, 4, 2};
 int yOff = offsets[animFrame % 8];

 display.clearDisplay();
 drawHeader();

 drawHeart(57, 26 + yOff);

 display.setTextSize(1);
 display.setCursor(46, 48);
 display.print("ALL SAFE");

 for (int d = 0; d < (animFrame % 4); d++) {
   display.fillCircle(50 + d * 10, 57, 2, SSD1306_WHITE);
 }

 display.display();
}

// ── CRYING ANIMATION ──
void showCryingAnimation() {
 display.clearDisplay();
 drawHeader();

 if (animFrame % 2 == 0) {
   display.drawRect(1, 13, 126, 50, SSD1306_WHITE);
   display.setTextSize(2);
   display.setCursor(4, 18);
   display.print("!");
 }

 display.setTextSize(1);
 display.setCursor(20, 17);
 display.print("ALERT DETECTED");

 display.setTextSize(2);
 display.setCursor(14, 32);
 display.print("BABY CRY!");

 // Sound wave arcs
 int waveX = 108, waveY = 38;
 if (animFrame % 3 >= 1) display.drawCircle(waveX, waveY, 5,  SSD1306_WHITE);
 if (animFrame % 3 >= 2) display.drawCircle(waveX, waveY, 9,  SSD1306_WHITE);

 display.display();
}

// ── ROLLED ANIMATION ──
void showRolledAnimation() {
 display.clearDisplay();
 drawHeader();

 if (animFrame % 2 == 0) {
   display.drawRect(1, 13, 126, 50, SSD1306_WHITE);
   display.setTextSize(2);
   display.setCursor(4, 18);
   display.print("!");
 }

 display.setTextSize(1);
 display.setCursor(20, 17);
 display.print("ALERT DETECTED");

 display.setTextSize(2);
 display.setCursor(8, 32);
 display.print("ROLLED!");

 // Rotating dot around circle
 int cx = 108, cy = 38, r = 9;
 display.drawCircle(cx, cy, r, SSD1306_WHITE);
 float rad = ((animFrame % 4) * 90 * 3.14159) / 180.0;
 int tipX = cx + r * cos(rad);
 int tipY = cy + r * sin(rad);
 display.fillCircle(tipX, tipY, 2, SSD1306_WHITE);

 display.display();
}

// ── NOTIFY CALLBACK — DO NOT TOUCH ──
static void notifyCallback(BLERemoteCharacteristic* pChar, uint8_t* pData, size_t len, bool isNotify) {
 String msg = "";
 for (int i = 0; i < len; i++) msg += (char)pData[i];

 if (msg == "ALARM_MOVE") {
   currentState = "ROLLED";
   triggerVibration(10, 100, 100);
 } else if (msg == "ALARM_SOUND") {
   currentState = "CRYING";
   triggerVibration(20, 200, 150);
 } else if (msg == "SAFE") {
   currentState = "SAFE";
 }
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
 void onResult(BLEAdvertisedDevice advertisedDevice) {
   if (advertisedDevice.getName() == "CRIB_SENSOR") {
     BLEDevice::getScan()->stop();
     myDevice = new BLEAdvertisedDevice(advertisedDevice);
     doConnect = true;
   }
 }
};

void setup() {
 Serial.begin(115200);
 pinMode(VIB_MOTOR_PIN, OUTPUT);

 if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
   Serial.println(F("SSD1306 allocation failed"));
 }

 // Boot screen
 display.clearDisplay();
 drawHeader();
 display.setTextSize(2);
 display.setCursor(20, 28);
 display.print("BOOTING..");
 display.display();

 BLEDevice::init("WATCH_UNIT");
 BLEScan* pBLEScan = BLEDevice::getScan();
 pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
 pBLEScan->setActiveScan(true);
 pBLEScan->start(5, false);

 Serial.println("Watch Unit Online - BLE & OLED");
}

void loop() {
 // ── BLE CONNECT — DO NOT TOUCH ──
 if (doConnect) {
   BLEClient* pClient = BLEDevice::createClient();
   if (pClient->connect(myDevice)) {
     BLERemoteService* pSvc = pClient->getService(serviceUUID);
     if (pSvc) {
       pRemoteCharacteristic = pSvc->getCharacteristic(charUUID);
       if (pRemoteCharacteristic && pRemoteCharacteristic->canNotify())
         pRemoteCharacteristic->registerForNotify(notifyCallback);
     }
   }
   doConnect = false;
 }

 // ── ANIMATION TICK every 300ms ──
 if (millis() - lastAnimFrame > 300) {
   lastAnimFrame = millis();
   animFrame++;

   if      (currentState == "SAFE")   showSafeAnimation();
   else if (currentState == "CRYING") showCryingAnimation();
   else if (currentState == "ROLLED") showRolledAnimation();
 }
}
