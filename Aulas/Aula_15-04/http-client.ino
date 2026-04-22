#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

// LCD (endereço 0x27, 16 colunas, 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =========================
// CONFIGURACAO DO WIFI
// =========================
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

// =========================
// URL DA API (SÃO PAULO)
// =========================
const char* WEATHER_URL =
  "https://api.open-meteo.com/v1/forecast?latitude=-23.5505&longitude=-46.6333&current=temperature_2m,relative_humidity_2m,apparent_temperature";

// =========================
// VARIAVEIS
// =========================
unsigned long lastRequestTime = 0;
const unsigned long REQUEST_INTERVAL = 10000;

// =========================
// FUNÇÃO LCD (AGORA CORRETA)
// =========================
void exibeDadosNoLCD(float temperatureC, float humidityP, float appTemperatureC) {
  lcd.clear();

  // Linha 1
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatureC, 1);
  lcd.print("C ");

  lcd.print("U:");
  lcd.print(humidityP, 0);
  lcd.print("%");

  // Linha 2
  lcd.setCursor(0, 1);
  lcd.print("Sens:");
  lcd.print(appTemperatureC, 1);
  lcd.print("C");
}

// =========================
// CONECTA NO WIFI
// =========================
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void ensureWiFiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Reconectando...");
    connectWiFi();
  }
}

// =========================
// FAZ REQUISIÇÃO
// =========================
void makeGetRequest() {
  HTTPClient http;

  Serial.println("\n--- FAZENDO GET NA API OPEN-METEO ---");

  http.begin(WEATHER_URL);
  int httpCode = http.GET();

  Serial.print("Status HTTP: ");
  Serial.println(httpCode);

  if (httpCode <= 0) {
    Serial.println("Erro na requisicao");
    http.end();
    return;
  }

  String payload = http.getString();
  http.end();

  Serial.println("\nJSON recebido:");
  Serial.println(payload);

  // Parse JSON
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Erro ao interpretar JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Pegando dados
  float temperatureC = doc["current"]["temperature_2m"];
  float humidityP = doc["current"]["relative_humidity_2m"];
  float appTemperatureC = doc["current"]["apparent_temperature"];

  // Serial
  Serial.println("\n--- DADOS ---");
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println(" C");

  Serial.print("Umidade: ");
  Serial.print(humidityP);
  Serial.println(" %");

  Serial.print("Sensacao: ");
  Serial.print(appTemperatureC);
  Serial.println(" C");

  // LCD
  exibeDadosNoLCD(temperatureC, humidityP, appTemperatureC);
}

// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);

  // Inicializa LCD (ANTES de usar)
  lcd.init();
  lcd.backlight();

  connectWiFi();
  makeGetRequest();
}

// =========================
// LOOP
// =========================
void loop() {
  unsigned long now = millis();

  if (now - lastRequestTime >= REQUEST_INTERVAL) {
    lastRequestTime = now;

    ensureWiFiConnected();
    makeGetRequest();
  }
}