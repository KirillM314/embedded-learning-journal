#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define LED_pin 2

#define net_name "UKYV" // Your net_name
#define password "igor2616" // Your password

WebServer server(80);

bool WiFi_flag = 0;
bool LED_status = false;

void handleLedOn() {
    LED_status = true;
    digitalWrite(LED_pin, HIGH);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}
void handleLedOff() {
    LED_status = false;
    digitalWrite(LED_pin, LOW);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

void handleStatus () {
    String json;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    JsonDocument doc_status;
    if (LED_status) {
        doc_status["led"] = "ON";
        doc_status["color"] = "green";
    }
    else {
        doc_status["led"] = "OFF";
        doc_status["color"] = "gray";
    }
    serializeJson(doc_status, json);
    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_pin, OUTPUT);

    WiFi.begin(net_name, password);
    server.begin();

    server.on("/ledOn", handleLedOn);
    server.on("/ledOff", handleLedOff);
    server.on("/status", handleStatus);

}

void loop() {
    if (WiFi.status() == WL_CONNECTED && WiFi_flag == 0) {
        WiFi_flag = 1;
        Serial.println("successful connection");
        Serial.println(WiFi.localIP());
    }
    server.handleClient();
}