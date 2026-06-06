#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
// ==== CHAN KET NOI (Cấu hình cho ESP32 Thường) ====
#define PIN_SOIL       32
#define PIN_WATER      33
#define PIN_LIGHT      34
#define PIN_DHT        14
#define PIN_PUMP       25
#define PIN_LED_GROW   26
#define PIN_SERVO      27
#define PIN_BUZZER     13
#define PIN_BTN        19

// Ghi chú: Chân I2C mặc định của ESP32 là SDA = 21, SCL = 22

// ==== THONG SO HE THONG ====
#define DHTTYPE DHT11
int threshold_soil_low   = 40;
int threshold_soil_high  = 70;
int threshold_water_low  = 10;
int threshold_light_low  = 30; // 30% ánh sáng
float threshold_temp_high  = 35.0;
float threshold_hum_high   = 80.0;
float threshold_hum_low    = 40.0;

// TFT ST7789 (240x240) SPI
#define TFT_CS    5
#define TFT_RST   4
#define TFT_DC    2
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
DHT dht(PIN_DHT, DHTTYPE);

// WiFi & MQTT
const char* ssid = "Thuy An";
const char* password = "thanh22888";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic_state = "smartgarden_v1/state";
const char* topic_cmd   = "smartgarden_v1/cmd";

WiFiClient espClient;
PubSubClient client(espClient);

bool isPumpOn = false;
bool isLedOn = false;
bool isAutoMode = true;
unsigned long lastReadTime = 0;
unsigned long lastScreenSwitch = 0;
int currentScreen = 0;

Servo fanServo;
int servoAngle = 0;
int servoStep = 5;
unsigned long lastServoUpdate = 0;
bool isFanOn = false;

// Button debounce variables
bool lastBtnState = HIGH;
bool btnState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
#define NUM_SCREENS    3
#define SCREEN_INTERVAL 2000 // Doi man hinh moi 2 giay

// Luu gia tri de hien thi
float g_temp = 0.0;
float g_hum = 0.0;
int g_soil = 0;
int g_water = 0;
int g_light = 0;
float temp_offset = 0.0; // Biến giả lập giảm nhiệt độ khi bật bơm
float hum_offset = 0.0; // Biến giả lập giảm độ ẩm khi quạt chạy

// Cấu trúc phân loại báo động
enum AlarmType {
  ALARM_NONE = 0,
  ALARM_WATER,
  ALARM_TEMP,
  ALARM_SOIL,
  ALARM_HUM
};
AlarmType currentAlarm = ALARM_NONE;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  if (String(topic) == topic_cmd) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    if (!error) {
      String cmd = doc["cmd"].as<String>();
      if (cmd == "auto") {
        isAutoMode = doc["value"].as<bool>();
      } else if (cmd == "pump" && !isAutoMode) {
        isPumpOn = doc["value"].as<bool>();
      } else if (cmd == "led" && !isAutoMode) {
        isLedOn = doc["value"].as<bool>();
      } else if (cmd == "fan" && !isAutoMode) {
        isFanOn = doc["value"].as<bool>();
      } else if (cmd == "set_threshold_soil") {
        threshold_soil_low = doc["value"].as<int>();
      } else if (cmd == "set_threshold_temp") {
        threshold_temp_high = doc["value"].as<float>();
      }
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic_cmd);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void displayScreen(int screen) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);

  switch (screen) {
    case 0: // Man hinh 1: Do am dat & Muc nuoc
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(10, 10);
      tft.print("VUON THONG MINH");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 50);
      tft.print("Dat: ");
      if(g_soil < threshold_soil_low || g_soil > threshold_soil_high) tft.setTextColor(ST77XX_RED);
      else tft.setTextColor(ST77XX_CYAN);
      tft.print(g_soil);
      tft.print("%");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 90);
      tft.print("Nuoc:");
      if(g_water <= threshold_water_low) tft.setTextColor(ST77XX_RED);
      else tft.setTextColor(ST77XX_CYAN);
      tft.print(g_water);
      tft.print("%");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 130);
      tft.print("Bom: ");
      if(isPumpOn) { tft.setTextColor(ST77XX_GREEN); tft.print("BAT"); }
      else { tft.setTextColor(ST77XX_RED); tft.print("TAT"); }

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 170);
      tft.print("Den: ");
      if(isLedOn) { tft.setTextColor(ST77XX_GREEN); tft.print("BAT"); }
      else { tft.setTextColor(ST77XX_RED); tft.print("TAT"); }

      tft.setCursor(10, 210);
      tft.setTextColor(ST77XX_YELLOW);
      tft.print(isAutoMode ? "[ TU DONG ]" : "[THU CONG]");
      break;

    case 1: // Man hinh 2: Nhiet do & Do am & Anh sang
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(30, 10);
      tft.print("=== THOI TIET ===");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 60);
      tft.print("Nhiet do: ");
      tft.setTextColor(ST77XX_ORANGE);
      tft.print(g_temp, 1);
      tft.print(" C");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 110);
      tft.print("Do am: ");
      tft.setTextColor(ST77XX_CYAN);
      tft.print(g_hum, 1);
      tft.print(" %");

      tft.setTextColor(ST77XX_WHITE);
      tft.setCursor(10, 160);
      tft.print("Anh sang: ");
      tft.setTextColor(ST77XX_YELLOW);
      tft.print(g_light);
      tft.print(" %");
      break;

    case 2: // Man hinh 3: Canh bao
      tft.setTextColor(ST77XX_RED);
      tft.setCursor(30, 10);
      tft.print("=== CANH BAO ===");

      tft.setCursor(10, 100);
      if (currentAlarm == ALARM_WATER) {
        tft.print(" ! HET NUOC !");
      } else if (currentAlarm == ALARM_TEMP) {
        tft.print(" ! NHIET DO CAO !");
      } else if (currentAlarm == ALARM_SOIL) {
        tft.print(" ! DAT KHO !");
      } else if (currentAlarm == ALARM_HUM) {
        tft.print(" ! DO AM THAP !");
      } else {
        tft.setTextColor(ST77XX_GREEN);
        tft.print("  He thong OK");
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== SMART GARDEN IoT ===");
  Serial.println("System starting...");

  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_LED_GROW, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
  
  digitalWrite(PIN_PUMP, LOW);
  digitalWrite(PIN_LED_GROW, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Setup cho ESP32Servo (cần thiết cho core mới)
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  fanServo.setPeriodHertz(50);
  fanServo.attach(PIN_SERVO, 500, 2400);

  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_WATER, INPUT);
  pinMode(PIN_LIGHT, INPUT);

  dht.begin();

  // Khoi tao TFT (ST7789)
  tft.init(240, 240); // Do phan giai 240x240
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(10, 80);
  tft.print("SMART");
  tft.setCursor(10, 120);
  tft.print("GARDEN");

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 180);
  tft.print("Connecting...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(10, 110);
  tft.print("WiFi Connected!");
  delay(1000);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  unsigned long currentMillis = millis();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Doc cam bien moi giay
  if (currentMillis - lastReadTime >= 1000) {
    lastReadTime = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    int rawSoil = analogRead(PIN_SOIL);
    int rawWater = analogRead(PIN_WATER);
    int rawLight = analogRead(PIN_LIGHT);

    // Đảo ngược mapping: 0 -> 100, 4095 -> 0 
    // Vì mặc định của Wokwi: vặn hết sang phải (xuôi kim đồng hồ) điện áp tiến về 0
    g_soil = map(rawSoil, 0, 4095, 100, 0);
    g_water = map(rawWater, 0, 4095, 100, 0);
    
    // Cảm biến ánh sáng LDR: giá trị raw càng lớn (kéo về bên trái) tức là càng tối
    // Nên ta map lại thành phần trăm ánh sáng: 0 (tối) -> 100% (sáng)
    g_light = map(rawLight, 0, 4095, 100, 0);

    if (isnan(h) || isnan(t)) {
      h = 0;
      t = 0;
    }

    // Gán trực tiếp giá trị cảm biến thật
    g_temp = t;
    g_hum = h;

    bool hasWarning = false;

    // Logic dieu khien
    if (isAutoMode) {
      // Logic may bom
      if (g_soil < threshold_soil_low) {
        if (g_water > threshold_water_low) {
          isPumpOn = true;
        } else {
          isPumpOn = false;
        }
      } else if (g_soil > threshold_soil_high) {
        isPumpOn = false;
      } else {
        if (isPumpOn && g_water <= threshold_water_low) {
          isPumpOn = false;
        }
      }

      // Logic den LED
      if (g_light < threshold_light_low) {
        isLedOn = true;
      } else {
        isLedOn = false;
      }

      // Logic quạt thông gió
      isFanOn = (g_hum > threshold_hum_high);
    } else {
      // Che do thu cong: kiem tra an toan
      if (isPumpOn && g_water <= threshold_water_low) {
        isPumpOn = false;
      }
    }

    // --- TỔNG HỢP CÁC LỖI CẦN BÁO ĐỘNG ---
    currentAlarm = ALARM_NONE; // Reset trạng thái
    
    // Đánh giá lỗi theo mức độ ưu tiên (Cái nào nguy hiểm hơn đặt lên trước)
    if (g_water <= 20) {
      currentAlarm = ALARM_WATER; // Nước hết hoặc gần hết (Nguy hiểm nhất)
    } 
    else if (g_temp > threshold_temp_high + 2.0 && !isPumpOn) {
      currentAlarm = ALARM_TEMP;  // Nhiệt độ quá cao (nóng hơn mức an toàn 2 độ)
    } 
    else if (g_temp < 10.0) {
      currentAlarm = ALARM_TEMP;  // Nhiệt độ quá thấp (lạnh dưới 10 độ)
    } 
    else if (g_soil < (threshold_soil_low - 15) || g_soil > (threshold_soil_high + 15)) {
      currentAlarm = ALARM_SOIL;  // Đất quá khô (<25) hoặc quá ẩm (>85) - Tránh kêu khi chỉ vừa lố 1 xíu
    } 
    else if (g_hum > threshold_hum_high + 5.0 || g_hum < threshold_hum_low - 5.0) {
      currentAlarm = ALARM_HUM;   // Độ ẩm không khí quá cao (>85) hoặc quá thấp (<35)
    }

    digitalWrite(PIN_PUMP, isPumpOn ? HIGH : LOW);
    digitalWrite(PIN_LED_GROW, isLedOn ? HIGH : LOW);

    // Gui du lieu qua MQTT
    JsonDocument doc;
    doc["type"] = "state";
    doc["soil"] = g_soil;
    doc["water"] = g_water;
    doc["temp"] = g_temp;
    doc["hum"] = g_hum;
    doc["light"] = g_light;
    doc["pump"] = isPumpOn;
    doc["led"] = isLedOn;
    doc["fan"] = isFanOn;
    doc["auto"] = isAutoMode;
    
    String jsonString;
    serializeJson(doc, jsonString);
    client.publish(topic_state, jsonString.c_str());

    // In ra Serial de theo doi
    Serial.printf("Soil:%d%% Water:%d%% Temp:%.1fC Hum:%.1f%% Light:%d | Pump:%s LED:%s Fan:%s Mode:%s\n",
      g_soil, g_water, g_temp, g_hum, g_light,
      isPumpOn ? "ON" : "OFF", isLedOn ? "ON" : "OFF", isFanOn ? "ON" : "OFF", isAutoMode ? "AUTO" : "MANUAL");
  }

  // Quét góc cho Servo (Quạt) không dùng delay
  if (isFanOn) {
    if (currentMillis - lastServoUpdate > 15) { // Quét nhanh hơn một chút
      lastServoUpdate = currentMillis;
      servoAngle += servoStep;
      
      // Phát tiếng "tạch tạch" mô phỏng cơ cấu cánh quạt
      if (servoAngle % 20 == 0 && currentAlarm == ALARM_NONE) {
        tone(PIN_BUZZER, 1500, 15); // Tiếng tạch nhẹ
      }

      if (servoAngle >= 180 || servoAngle <= 0) {
        servoStep = -servoStep; // Đảo chiều quay (Quạt tuốc năng)
        if (currentAlarm == ALARM_NONE) {
          tone(PIN_BUZZER, 2000, 30); // Tiếng cạch mạnh khi đảo chiều
        }
      }
      fanServo.write(servoAngle);
    }
  } else {
    if (servoAngle != 0) {
      servoAngle = 0;
      fanServo.write(0);
    }
  }

  // Xu ly Nut nhan voi chong doi
  bool reading = digitalRead(PIN_BTN);
  if (reading != lastBtnState) {
    lastDebounceTime = currentMillis;
  }
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading != btnState) {
      btnState = reading;
      if (btnState == LOW) {
        // Chuyen che do sang thu cong va toggle bom
        isAutoMode = false;
        isPumpOn = !isPumpOn;
        Serial.println("Nut duoc nhan: Chuyen sang MANUAL, Toggle Pump");
        
        // Gui trang thai moi ngay lap tuc qua MQTT
        JsonDocument doc;
        doc["type"] = "state";
        doc["soil"] = g_soil;
        doc["water"] = g_water;
        doc["temp"] = g_temp;
        doc["hum"] = g_hum;
        doc["light"] = g_light;
        doc["pump"] = isPumpOn;
        doc["led"] = isLedOn;
        doc["fan"] = isFanOn;
        doc["auto"] = isAutoMode;
        
        String jsonString;
        serializeJson(doc, jsonString);
        client.publish(topic_state, jsonString.c_str());
      }
    }
  }
  lastBtnState = reading;

  // Xử lý còi hú liên tục đa âm sắc không dùng delay
  static int lastTone = -1;
  int currentTone = -1; // -1 nghĩa là tắt còi

  if (currentAlarm == ALARM_WATER) {
    // Tiếng bíp dồn dập, réo rắt báo hiệu khẩn cấp (Hết nước): Tít tít tít...
    currentTone = (currentMillis % 400 < 100) ? 2500 : -1;
  } else if (currentAlarm == ALARM_TEMP) {
    // Tiếng còi xe cứu thương (Nhiệt độ bất thường): Ò... e... ò... e...
    currentTone = (currentMillis % 1000 < 500) ? 800 : 1000;
  } else if (currentAlarm == ALARM_SOIL) {
    // Tiếng lỗi hệ thống trầm đục (Độ ẩm đất): Bíp -------- Bíp --------
    currentTone = (currentMillis % 2000 < 500) ? 300 : -1;
  } else if (currentAlarm == ALARM_HUM) {
    // Tiếng bíp đều đặn (Độ ẩm không khí): Tít ... Tít ...
    currentTone = (currentMillis % 1000 < 200) ? 1200 : -1;
  }

  // Cập nhật âm thanh nếu có sự thay đổi
  if (currentTone != lastTone) {
    if (currentTone == -1) {
      noTone(PIN_BUZZER);
      digitalWrite(PIN_BUZZER, LOW); // Đảm bảo còi tắt hoàn toàn
    } else {
      tone(PIN_BUZZER, currentTone);
    }
    lastTone = currentTone;
  }

  // Chuyen man hinh LCD moi 2 giay
  if (currentMillis - lastScreenSwitch >= SCREEN_INTERVAL) {
    lastScreenSwitch = currentMillis;
    displayScreen(currentScreen);
    currentScreen = (currentScreen + 1) % NUM_SCREENS;
  }
}
