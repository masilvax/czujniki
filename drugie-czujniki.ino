#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <HTTPClient.h>
#include "Config.h"
#include "WiFiConfig.h"
#include "AlarmTypes.h"
#include "AlarmMelodies.h"
#include "MelodyPlayer.h"

#define DEBUG 1 // 1 - logowanie na konsole, 0 - produkcja

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire);

// Czas pomiędzy wysyłkami danych
// unsigned long previousMillis = 0;
const long interval = 60000;  // 1 minuta
unsigned long previousMillis = millis() - interval; // Jednorazowe ustawienie podczas inicjalizacji zeby nie czekal z wyswietleniem przez caly pierwszy interwał

void setup() {
#if DEBUG
  Serial.begin(115200); // Inicjalizacja portu szeregowego tylko w trybie debugowania
  delay(100);           // Opcjonalny delay dla stabilizacji
#endif

  initializeDisplay();
  initializeWiFi();  
  dht.begin();
  displayStatusAndPlayStartSound();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Odczyt danych z czujników
    float temp = readTemperature();
    float humidity = readHumidity();
    int gasLevel = readGasLevel();
    int waterLevel = readWaterLevel();

    displayData(temp, humidity, gasLevel, waterLevel);
    updateMelodyQueue(temp > 35, humidity > 80, gasLevel > 500, waterLevel > 50);
  }
  playMelodyQueue();
}

void initializeDisplay() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    debugPrint(F("OLED nie jest podłączony"));
    while (true); // program sie tu powiesi, w sensie bedzie w nieskonczonej petli jak nie bedzie ekranu, ale na serialu bedzie widac powyzszy komunikat
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void initializeWiFi() {
  const int maxRetryCount = 5;
  int retryCount = 0;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && retryCount < maxRetryCount) {
      delay(1000);
      retryCount++;
      debugPrint("Łączenie z WiFi, próba: " + String(retryCount));
  }

  if (WiFi.status() == WL_CONNECTED) {
      debugPrint("Połączono z WiFi");
  } else {
      debugPrint("Nie udało się połączyć z WiFi po " + String(maxRetryCount) + " próbach.");
  }
}

float readTemperature() {
  return dht.readTemperature();
}

float readHumidity() {
  return dht.readHumidity();
}

int readWaterLevel() {
  return analogRead(WATER_SENSOR_PIN);
}

int readGasLevel() {
  return analogRead(GAS_SENSOR_PIN);
}

void displayData(float temp, float humidity, int gas, int water) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("T: %.1f C\n", temp);
  display.printf("W: %.1f %%\n", humidity);
  display.printf("G: %d\n", gas);
  display.printf("Z: %d\n", water);
  display.display();
}

void sendDataToServer(float temp, float humidity, int gas, int water) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonData = String("{\"akcja\": \"saveCzujniki\", \"czujniki\": {") +
                  "\"id\": 1," +
                  "\"temperature\": " + temp + "," +
                  "\"humidity\": " + humidity + "," +
                  "\"gas\": " + gas + "," +
                  "\"water\": " + water +
                  "}}";

    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      // debugPrint(String("POST: ") + httpResponseCode);
    } else {
      debugPrint(String("Błąd POST: ") + http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    debugPrint("Brak połączenia WiFi");
  }
}

void displayStatusAndPlayStartSound() {
  String mac = WiFi.macAddress();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,25);
  display.println("MAC adress:");
  display.println(mac);
  display.println("WiFi:");
  display.println(WiFi.status());
  display.display();
  delay(2000);

  playNote(200, 100);
  playNote(300, 100);
  playNote(400, 75);
  playNote(500, 75);
  playNote(600, 50);
  playNote(700, 50);
  playNote(800, 50);
  playNote(900, 50);
  playNote(1000, 50);
  playNote(1500, 50);
  playNote(2000, 50);
  playNote(2500, 50);
  playNote(3000, 50);

  noTone(BUZZER_PIN);
}

void playNote(int freq, int duration) {
  tone(BUZZER_PIN, freq);
  delay(duration);
  noTone(BUZZER_PIN);
  delay(10);
}

#if DEBUG
// Funkcja do obsługi tekstów typu const char* (zwykłe ciągi znaków w stylu C)
void debugPrint(const char *message) {
  Serial.println(message);
}

// Funkcja do obsługi tekstów przechowywanych w pamięci Flash (F())
void debugPrint(const __FlashStringHelper *message) {
  Serial.println(message);
}

// Funkcja do obsługi obiektów typu String
void debugPrint(const String &message) {
  Serial.println(message);
}
#else
// Puste wersje funkcji, gdy DEBUG jest wyłączony

void debugPrint(const char *message) {}
void debugPrint(const __FlashStringHelper *message) {}
void debugPrint(const String &message) {}
#endif