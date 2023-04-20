#include <Arduino.h>
#include "secrets.h"
#include <WiFiClient.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define TEMP_PUBLISH_TOPIC "TEMP"
#define LIGHT_SUBSCRIBE_TOPIC "LIGHT"

WiFiClient net = WiFiClient();
MQTTClient client = MQTTClient(256);

#define MINUTE 60000
#define FAST_PUBLISH

#ifdef FAST_PUBLISH
#define PUBLISH_DELAY (MINUTE / 12) // 5 seconds per publish
#else
#define PUBLISH_DELAY (MINUTE * 2) // 2 minutes per publish
#endif
long lastMessage = 0;

void messageHandler(String &topic, String &payload);

void connectIoT()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    client.begin(ENDPOINT, PORT, net);

    client.onMessage(messageHandler);

    Serial.print("Connecting to Node-Red");

    while (!client.connect(DEVICE_NAME))
    {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected())
    {
        Serial.println("Client Timeout!");
        return;
    }

    client.subscribe(LIGHT_SUBSCRIBE_TOPIC);

    Serial.println("MQTT Connected!");
}

void publishMessage()
{
    StaticJsonDocument<200> doc;
    doc["time"] = millis();
    doc["sensor_a0"] = rand();
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    long long int timeStart = millis();
    client.publish(TEMP_PUBLISH_TOPIC, jsonBuffer, false, 0);
    long long int timeEnd = millis();
    Serial.print("Publish took: ");
    Serial.println(timeEnd - timeStart);
}

void messageHandler(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);
}

void setup()
{
    Serial.begin(9600);
    connectIoT();
}

void loop()
{
    client.loop();

    if ((lastMessage + PUBLISH_DELAY) < millis())
    {
        publishMessage();
        lastMessage = millis();
    }
}
