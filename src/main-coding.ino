#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <time.h>

/******************** WIFI & TELEGRAM ********************/
#define WIFI_SSID     "WIFI_NAME"
#define WIFI_PASSWORD "PASSWORD_HERE"
#define BOT_TOKEN     "BOT_TOKEN_HERE"

/******************** PIN ********************/
#define LED1 25
#define LED2 26
#define LED3 27

#define LDR_PIN 34
#define SERVO_PIN 13

#define DHT_PIN 14
#define DHTTYPE DHT11

#define CLK 18
#define DIO 19

/******************** OBJECT ********************/
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
Servo servo;
DHT dht(DHT_PIN, DHTTYPE);
TM1637Display display(CLK, DIO);

/******************** VARIABLE ********************/
bool automaticMode = false;
bool modeSelected = false;

long lastUpdateId = 0;
unsigned long lastTelegramCheck = 0;

/******************** SETUP ********************/
void setup() {
  Serial.begin(115200);
  Serial.println("SMART HOME ESP32 START");

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  servo.attach(SERVO_PIN);
  dht.begin();
  display.setBrightness(7);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setInsecure();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  configTime(7 * 3600, 0, "pool.ntp.org");
}

/******************** LOOP ********************/
void loop() {
  if (millis() - lastTelegramCheck > 1500) {
    checkTelegram();
    lastTelegramCheck = millis();
  }

  displayClock();

  if (modeSelected && automaticMode) {
    automaticModeFunction();
  }
}

/******************** AUTOMATIC MODE ********************/
void automaticModeFunction() {
  int ldrValue = analogRead(LDR_PIN);

  Serial.print("AUTOMATIC MODE | LDR = ");
  Serial.print(ldrValue);

  if (ldrValue < 2000) {   // DARK
    digitalWrite(LED1, HIGH);
    servo.write(0);
    Serial.println(" | DARK | LED1 ON | WINDOW CLOSED");
  } else {                // BRIGHT
    digitalWrite(LED1, LOW);
    servo.write(90);
    Serial.println(" | BRIGHT | LED1 OFF | WINDOW OPEN");
  }
}

/******************** DIGITAL CLOCK ********************/
void displayClock() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int timeValue = timeinfo.tm_hour * 100 + timeinfo.tm_min;
  display.showNumberDecEx(timeValue, 0b11100000, true);
}

/******************** MENU ********************/
void sendManualMenu(String chat_id) {
  bot.sendMessage(
    chat_id,
    "MANUAL MODE\n\n"
    "Light Control:\n"
    "/LED1_ON  /LED1_OFF\n"
    "/LED2_ON  /LED2_OFF\n"
    "/LED3_ON  /LED3_OFF\n\n"
    "Window Control:\n"
    "/open_window\n"
    "/close_window\n\n"
    "Sensors:\n"
    "/check_temperature\n"
    "/check_humidity",
    ""
  );
}

void sendAutomaticMenu(String chat_id) {
  bot.sendMessage(
    chat_id,
    "AUTOMATIC MODE\n\n"
    "Porch Light and Window are controlled automatically based on LDR\n\n"
    "Room Lights:\n"
    "/LED2_ON  /LED2_OFF\n"
    "/LED3_ON  /LED3_OFF\n\n"
    "Sensors:\n"
    "/check_temperature\n"
    "/check_humidity",
    ""
  );
}

/******************** TELEGRAM ********************/
void checkTelegram() {
  int messageCount = bot.getUpdates(lastUpdateId + 1);

  while (messageCount) {
    for (int i = 0; i < messageCount; i++) {
      lastUpdateId = bot.messages[i].update_id;

      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      Serial.print("Telegram Command: ");
      Serial.println(text);

      if (text == "/start") {
        modeSelected = false;
        Serial.println("Waiting for mode selection");
        bot.sendMessage(
          chat_id,
          "SMART HOME ESP32\n\n"
          "Please select operation mode:\n"
          "Manual\n"
          "Automatic",
          ""
        );
      }

      if (text == "Manual") {
        automaticMode = false;
        modeSelected = true;
        Serial.println("MANUAL MODE ACTIVE");
        bot.sendMessage(chat_id, "MANUAL mode activated", "");
        sendManualMenu(chat_id);
      }

      if (text == "Automatic") {
        automaticMode = true;
        modeSelected = true;
        Serial.println("AUTOMATIC MODE ACTIVE");
        bot.sendMessage(chat_id, "AUTOMATIC mode activated", "");
        sendAutomaticMenu(chat_id);
      }

      if (modeSelected && !automaticMode) {
        if (text == "/LED1_ON") digitalWrite(LED1, HIGH);
        if (text == "/LED1_OFF") digitalWrite(LED1, LOW);
        if (text == "/LED2_ON") digitalWrite(LED2, HIGH);
        if (text == "/LED2_OFF") digitalWrite(LED2, LOW);
        if (text == "/LED3_ON") digitalWrite(LED3, HIGH);
        if (text == "/LED3_OFF") digitalWrite(LED3, LOW);

        if (text == "/open_window") servo.write(90);
        if (text == "/close_window") servo.write(0);
      }

      if (modeSelected && automaticMode) {
        if (text == "/LED2_ON") digitalWrite(LED2, HIGH);
        if (text == "/LED2_OFF") digitalWrite(LED2, LOW);
        if (text == "/LED3_ON") digitalWrite(LED3, HIGH);
        if (text == "/LED3_OFF") digitalWrite(LED3, LOW);
      }

      if (text == "/check_temperature") {
        float temperature = dht.readTemperature();
        Serial.print("Temperature: ");
        Serial.println(temperature);
        bot.sendMessage(chat_id, "Temperature: " + String(temperature) + " C", "");
      }

      if (text == "/check_humidity") {
        float humidity = dht.readHumidity();
        Serial.print("Humidity: ");
        Serial.println(humidity);
        bot.sendMessage(chat_id, "Humidity: " + String(humidity) + " %", "");
      }
    }
    messageCount = bot.getUpdates(lastUpdateId + 1);
  }
}
