#include "DHTesp.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#define WIFI_SSID "FIAP-IOT"
#define WIFI_PASSWORD "F!@p25.IOT"
#define WIFI_CHANNEL 6

WebServer server(80);

const int DHT_PIN = 15;
DHTesp dhtSensor;

void sendJsonStatus(String temp, String humi) {
  String response = "{";
  response += "\"Temperatura\":" + temp + ",";
  response += "\"Umidade\":" + humi;
  response += "}";

  server.send(200, "application/json", response);
}

void setup() {
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.println(WiFi.localIP());

  server.on("/api/status", HTTP_GET, []() {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    String temp = String(data.temperature);
    String humi = String(data.humidity);

    sendJsonStatus(temp, humi);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}