#include <Arduino.h>
#include <Wire.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define MPU_ADDR 0x68
#define LED_PIN 2

#define I2S_WS 15
#define I2S_SD 32
#define I2S_SCK 14
#define I2S_PORT I2S_NUM_0

const char* ssid     = "Afree";
const char* password = "00000000";

AsyncWebServer server(80);

int16_t AccX, AccY;

int loudCount = 0;
int cryScore = 0;
unsigned long lastTrigger = 0;

int totalCryCount = 0;
int totalMoveCount = 0;
String lastEvent = "SAFE";

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // MPU INIT — DO NOT TOUCH
  Wire.begin(18, 19);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // I2S CONFIG — DO NOT TOUCH
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);

  // WiFi CONNECT
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  // WEB SERVER ENDPOINTS
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req) {
    StaticJsonDocument<200> doc;
    doc["status"] = lastEvent;
    doc["cry"] = (lastEvent == "ALARM_SOUND");
    doc["move"] = (lastEvent == "ALARM_MOVE");
    doc["cryCount"] = totalCryCount;
    doc["moveCount"] = totalMoveCount;
    String out;
    serializeJson(doc, out);
    req->send(200, "application/json", out);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *req) {
    StaticJsonDocument<300> doc;
    doc["totalCryCount"] = totalCryCount;
    doc["totalMoveCount"] = totalMoveCount;
    doc["lastEvent"] = lastEvent;
    doc["uptime"] = millis() / 1000;
    String out;
    serializeJson(doc, out);
    req->send(200, "application/json", out);
  });

  server.begin();
  Serial.println("Web server started!");
  Serial.println("Crib Unit Online - WiFi Mode");
}

void loop() {
  bool isTilted = false;
  bool isCry = false;

  // ---------------- MPU — DO NOT TOUCH ----------------
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(MPU_ADDR, 4, true) == 4) {
    AccX = Wire.read() << 8 | Wire.read();
    AccY = Wire.read() << 8 | Wire.read();
    if (abs(AccX) > 12000 || abs(AccY) > 12000) isTilted = true;
  }

  // ---------------- SOUND ANALYSIS — DO NOT TOUCH ----------------
  int32_t sample = 0;
  size_t bytes_read;
  int32_t peak = 0;
  int64_t sum = 0;

  for (int i = 0; i < 80; i++) {
    i2s_read(I2S_PORT, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
    if (bytes_read > 0) {
      int val = abs(sample);
      sum += val;
      if (val > peak) peak = val;
    }
  }

  int avg = sum / 80;
  Serial.print("Peak: "); Serial.print(peak);
  Serial.print(" Avg: "); Serial.println(avg);

  bool isClap = (peak > 50000000 && avg < 10000000);
  bool isFan = (avg < 3000000);
  bool isCryLike = (avg > 10000000);

  if (!isClap && !isFan && isCryLike) {
    loudCount++;
  } else {
    loudCount = 0;
    if (cryScore > 0) cryScore--;
  }
  if (loudCount > 5) cryScore++;
  if (cryScore > 3) isCry = true;

  // ---------------- OUTPUT ----------------
  if (millis() - lastTrigger > 4000) {
    if (isTilted) {
      digitalWrite(LED_PIN, HIGH);
      lastEvent = "ALARM_MOVE";
      totalMoveCount++;
      lastTrigger = millis();
      Serial.println(">> ALARM_MOVE");
    } else if (isCry) {
      digitalWrite(LED_PIN, HIGH);
      lastEvent = "ALARM_SOUND";
      totalCryCount++;
      lastTrigger = millis();
      Serial.println(">> ALARM_SOUND");
    } else {
      digitalWrite(LED_PIN, LOW);
      lastEvent = "SAFE";
    }
  }

  delay(50);
}