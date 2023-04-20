#include <Arduino.h>
#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define MINUTE 60000
#define FAST_PUBLISH

#ifdef FAST_PUBLISH
#define PUBLISH_DELAY (MINUTE / 6) // 10 seconds per publish
#else
#define PUBLISH_DELAY (MINUTE * 2) // 2 minutes per publish
#endif
long lastMessage = 0;

void connectIoT()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
}

void publishMessage()
{
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    StaticJsonDocument<200> doc;
    WiFiClient client;
    HTTPClient http;

    doc["time"] = millis();
    doc["sensor_a0"] = rand();
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    Serial.println("Posting to Node-Red");
    long long int timeStart = millis();
    http.begin(client, ENDPOINT);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = jsonBuffer;           
    int httpResponseCode = http.POST(httpRequestData);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);        
    http.end();
    long long int timeEnd = millis();
    Serial.print("Time taken for 1 message: ");
    Serial.println(timeEnd - timeStart);
}

void setup()
{
    Serial.begin(9600);
    connectIoT();
}

void loop()
{
    if ((lastMessage + PUBLISH_DELAY) < millis())
    {
        publishMessage();
        lastMessage = millis();
    }
}
