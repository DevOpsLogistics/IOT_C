#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <BH1750.h> // Thu vien cho cam bien anh sang BH1750 I2C
// ==== CHAN KET NOI (ESP32-C3 Super Mini 16 chan) ====
// ADC chi co tren GPIO 0-4 cua ESP32-C3
#define PIN_SOIL       0   // ADC - Cam bien do am dat
#define PIN_WATER      1   // ADC - Cam bien muc nuoc
// ==== KHAI BAO DHT11 ====
#define PIN_DHT        4   // Digital In - Cam bien nhiet do/do am (DHT11)
#define PIN_PUMP       8   // Digital Out - Relay may bom (Active Low)
#define PIN_LED_GROW   20  // Digital Out - Den LED quang hop
#define PIN_SERVO      10  // PWM - Servo quat
#define PIN_BUZZER     21  // Digital Out - Coi bao dong
#define PIN_BTN        9   // Digital In - Nut bam (chung voi nut BOOT)

// ==== THONG SO HE THONG ====
#define DHTTYPE DHT11
int threshold_soil_low   = 40;
int threshold_soil_high  = 70;
int threshold_water_low  = 10;
int threshold_light_low  = 30;
float threshold_temp_high  = 35.0;
float threshold_hum_high   = 80.0;
float threshold_hum_low    = 40.0;

// OLED 1.3 inch (SH1106) I2C
// SCK/SCL = GPIO6, MOSI/SDA = GPIO7
#define I2C_SDA 7
#define I2C_SCL 6
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
// DHT & LDR (Bay gio dung BH1750 I2C)
DHT dht(PIN_DHT, DHTTYPE);
BH1750 lightMeter;

// WiFi & MQTT
const char* ssid = "CFVGHCM";
const char* password = "12345@bc";

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
  
  Serial.print("[MQTT CMD] Nhan lenh: ");
  Serial.println(message);

  if (String(topic) == topic_cmd) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    if (!error) {
      String cmd = doc["cmd"].as<String>();
      if (cmd == "auto") {
        isAutoMode = doc["value"].as<bool>();
        Serial.printf(">>> Chuyen che do: %s\n", isAutoMode ? "TU DONG" : "THU CONG");
      } else if (cmd == "pump" && !isAutoMode) {
        isPumpOn = doc["value"].as<bool>();
        Serial.printf(">>> Pump: %s\n", isPumpOn ? "BAT" : "TAT");
      } else if (cmd == "pump" && isAutoMode) {
        Serial.println(">>> Pump: BI KHOA vi dang o che do TU DONG!");
      } else if (cmd == "led" && !isAutoMode) {
        isLedOn = doc["value"].as<bool>();
        Serial.printf(">>> LED: %s\n", isLedOn ? "BAT" : "TAT");
      } else if (cmd == "fan" && !isAutoMode) {
        isFanOn = doc["value"].as<bool>();
        Serial.printf(">>> Fan: %s\n", isFanOn ? "BAT" : "TAT");
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  switch (screen) {
    case 0: // Man hinh 1: Do am dat & Muc nuoc
      display.setCursor(18, 0);
      display.print("VUON THONG MINH");

      display.setCursor(0, 15);
      display.print("Dat: ");
      display.print(g_soil);
      display.print("%");

      display.setCursor(0, 25);
      display.print("Nuoc: ");
      display.print(g_water);
      display.print("%");

      display.setCursor(0, 35);
      display.print("Bom: ");
      display.print(isPumpOn ? "BAT" : "TAT");

      display.setCursor(0, 45);
      display.print("Den: ");
      display.print(isLedOn ? "BAT" : "TAT");

      display.setCursor(0, 55);
      display.print(isAutoMode ? "[ TU DONG ]" : "[THU CONG]");
      break;

    case 1: // Man hinh 2: Nhiet do & Do am & Anh sang
      display.setCursor(15, 0);
      display.print("=== THOI TIET ===");

      display.setCursor(0, 20);
      display.print("Nhiet do: ");
      display.print(g_temp, 1);
      display.print(" C");

      display.setCursor(0, 35);
      display.print("Do am: ");
      display.print(g_hum, 1);
      display.print(" %");

      display.setCursor(0, 50);
      display.print("Anh sang: ");
      display.print(g_light);
      display.print(" %");
      break;

    case 2: // Man hinh 3: Canh bao
      display.setCursor(15, 0);
      display.print("=== CANH BAO ===");

      display.setCursor(0, 25);
      if (currentAlarm == ALARM_WATER) {
        display.print(" ! HET NUOC !");
      } else if (currentAlarm == ALARM_TEMP) {
        display.print(" ! NHIET DO CAO !");
      } else if (currentAlarm == ALARM_SOIL) {
        display.print(" ! DAT KHO !");
      } else if (currentAlarm == ALARM_HUM) {
        display.print(" ! DO AM THAP !");
      } else {
        display.print("  He thong OK");
      }
      break;
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== SMART GARDEN IoT ===");
  Serial.println("System starting...");

  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_LED_GROW, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
  
  digitalWrite(PIN_PUMP, HIGH);      // Active Low Relay: HIGH = TAT bom
  digitalWrite(PIN_LED_GROW, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // === TEST RELAY: Keu 'Cach' 2 lan khi khoi dong ===
  Serial.println("[TEST] Bat bom thu...");
  digitalWrite(PIN_PUMP, LOW);   // Active Low: LOW = BAT relay
  delay(300);
  digitalWrite(PIN_PUMP, HIGH);  // Active Low: HIGH = TAT relay
  delay(300);
  digitalWrite(PIN_PUMP, LOW);   // BAT
  delay(300);
  digitalWrite(PIN_PUMP, HIGH);  // TAT (trang thai cuoi: bom TAT)
  Serial.println("[TEST] Xong test bom.");

  // Setup cho ESP32Servo (ESP32-C3 co 4 timer, 6 kenh LEDC)
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  fanServo.setPeriodHertz(50);
  fanServo.attach(PIN_SERVO, 500, 2400);

  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_WATER, INPUT);

  dht.begin();
  
  // Khoi tao OLED 1.3" (SH1106)
  Wire.begin(I2C_SDA, I2C_SCL);
  display.begin(0x3C, true); // Dia chi I2C cua OLED la 0x3C
  
  // Khoi tao BH1750 tren cung bus I2C
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 da khoi tao thanh cong");
  } else {
    Serial.println("Loi: Khong tim thay cam bien BH1750!");
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(35, 10);
  display.print("SMART");
  display.setCursor(30, 35);
  display.print("GARDEN");
  display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 30);
  display.print("Connecting WiFi...");
  display.display();

  // === TANG CUONG DO ON DINH WIFI ===
  WiFi.setSleep(false); // Tat che do tiet kiem nang luong WiFi (giup song manh va on dinh lien tuc)
  WiFi.setAutoReconnect(true); // Tu dong ket noi lai neu rot mang
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  display.clearDisplay();
  display.setCursor(20, 30);
  display.print("WiFi Connected!");
  display.display();
  delay(1000);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  unsigned long currentMillis = millis();

  // Kiem tra WiFi truoc, co WiFi thi moi kiem tra MQTT
  if (WiFi.status() != WL_CONNECTED) {
    // Doi WiFi tu dong ket noi lai (vi da bat AutoReconnect)
  } else {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }

  // Doc cam bien moi giay
  if (currentMillis - lastReadTime >= 1000) {
    lastReadTime = currentMillis;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Doc ADC
    int rawSoil = analogRead(PIN_SOIL);
    int rawWater = analogRead(PIN_WATER);
    
    // Doc cam bien anh sang BH1750 (don vi Lux)
    float lux = lightMeter.readLightLevel();

    // Cảm biến thực tế hoạt động khác trên web mô phỏng:
    // 1. Cảm biến độ ẩm đất: Khô = giá trị ADC cao (~4095), Ướt = giá trị ADC thấp (~1000)
    g_soil = map(rawSoil, 4095, 1000, 0, 100);
    g_soil = constrain(g_soil, 0, 100); // Đảm bảo không vượt quá 0-100%
    
    // 2. Cảm biến mực nước: Khô (không có nước) = 0, Ngập nước = ADC cao (~3000)
    g_water = map(rawWater, 0, 3000, 0, 100);
    g_water = constrain(g_water, 0, 100);
    
    // 3. Cảm biến ánh sáng BH1750 (lux): trong phong thuong ~0-1000 lux
    // Map tu 0-1000 lux sang 0-100%
    g_light = map(lux, 0, 1000, 0, 100);
    g_light = constrain(g_light, 0, 100);

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

    digitalWrite(PIN_PUMP, isPumpOn ? LOW : HIGH); // Active Low Relay: LOW = BAT, HIGH = TAT
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
