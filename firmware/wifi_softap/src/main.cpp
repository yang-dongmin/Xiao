#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

WiFiServer server(80);

#ifndef LED_BUILTIN
#define LED_BUILTIN 21
#endif

const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = "";
  unsigned long started = millis();
  while (client.connected() && millis() - started < 2000) {
    if (!client.available()) continue;
    char c = client.read();
    request += c;
    if (c == '\n' && request.indexOf("\r\n\r\n") >= 0) break;
  }

  if (request.startsWith("GET /H")) digitalWrite(ledPin, HIGH);
  if (request.startsWith("GET /L")) digitalWrite(ledPin, LOW);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html><body>");
  client.println("<h2>XIAO ESP32S3 LED Control</h2>");
  client.println("<p><a href='/H'>LED ON</a></p>");
  client.println("<p><a href='/L'>LED OFF</a></p>");
  client.println("</body></html>");
  client.stop();
}

