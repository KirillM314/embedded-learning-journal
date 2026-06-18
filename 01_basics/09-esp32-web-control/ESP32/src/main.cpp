#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED_pin 2

#define net_name "Your net_name"
#define password "Your password"

WebServer server(80);

bool WiFi_flag = 0;

void handleLedOn() {
    digitalWrite(LED_pin, HIGH);
}
void handleLedOff() {
    digitalWrite(LED_pin, LOW);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(net_name, password);
    pinMode(LED_pin, OUTPUT);
    server.begin();
    server.on("/ledOn", handleLedOn);
    server.on("/ledOff", handleLedOff);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED && WiFi_flag == 0) {
        WiFi_flag = 1;
        Serial.println("successful connection");
        Serial.println(WiFi.localIP());
    }
    server.handleClient();
}