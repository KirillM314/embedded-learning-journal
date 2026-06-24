#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

#define net_name "UKYV" // Your net_name
#define password "igor2616" // Your password

WebSocketsServer webSocket(81);

bool WiFi_flag = 0;

void onWebSocketEvent(
    uint8_t num,
    WStype_t type,
    uint8_t * payload,
    size_t length
){
    switch(type) {

        case WStype_CONNECTED:
            Serial.println("Client connected");
            webSocket.sendTXT(num, "connected");
            break;

        case WStype_DISCONNECTED:
            Serial.println("Client disconnected");
            break;

        case WStype_TEXT: {
            String msg = String((char*)payload).substring(0, length);
            String json;
            JsonDocument doc;
            doc["input"] = msg;
            doc["length"] = length;
            serializeJson(doc, json);
            webSocket.sendTXT(num, json);
            break;
        }
    }
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(net_name, password);
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);

}

void loop() {
    if (WiFi.status() == WL_CONNECTED && WiFi_flag == 0) {
        WiFi_flag = 1;
        Serial.println("successful connection");
        Serial.println(WiFi.localIP());
    }

    webSocket.loop();
}