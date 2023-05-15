#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <string.h>
#include <SFE_MicroOLED.h>

// DISPLAY
#define OLED_RESET 9
#define DC_JUMPER 1

MicroOLED oled(OLED_RESET, DC_JUMPER);
int cursorY = 0;

// I2C ADDRESSES
#define TEMPERATURE_ADDRESS 1
#define HUMIDITY_ADDRESS 2

// DEVICE ID
#define __HUMIDITY__

#ifdef __HUMIDITY__
#define MY_ADDRESS HUMIDITY_ADDRESS
#define OTHER_DEVICE_ADDRESS TEMPERATURE_ADDRESS
#define CURSOR_Y 16
#define GATHER_DATA (int)dht11.readHumidity()
#endif

#ifdef __TEMPERATURE__
#define MY_ADDRESS TEMPERATURE_ADDRESS
#define OTHER_DEVICE_ADDRESS HUMIDITY_ADDRESS
#define CURSOR_Y 0
#define GATHER_DATA (int)dht11.readTemperature()
#endif

// GENERAL

#define TITLE_TEMPERATURE "T: "
#define TITLE_HUMIDITY "H: "

// M2M COMMUNICATION
#define TOKEN 69

// DISPLAY COMMUNICATION

// REGISTER
bool hasToken = false;
int sensorData = 0; // Is only used for outbound communication
int otherSensorData = 0; // Is only used for inbound communication

// DHT11
DHT dht11(A0, DHT11);

int messageCounter = 0;


void onRecieve(int howMany)
{
  int inbound = 0;

  while(Wire.available() > 0)
  {
    inbound = Wire.read();
  }

  if(messageCounter == 0)
  {
    otherSensorData = inbound;
    messageCounter++;
  }
  if(messageCounter == 1 && inbound == TOKEN)
  {
    hasToken = true;
    messageCounter = 0;
  }
}

void setup()
{
  dht11.begin();
  Wire.onReceive(onRecieve);
  Wire.begin(MY_ADDRESS);

  oled.begin();
  oled.setFontType(1);
  oled.clear(ALL);
  oled.display();
  oled.clear(PAGE);
  Serial.begin(9600);
}

void PrintLine(String title, int data)
{
  oled.print(title);
  oled.print(data);
  oled.display();
  cursorY += 16;
  oled.setCursor(0,cursorY);
}

void ResetCursor()
{
  oled.setCursor(0, 0);
}

void loop()
{
  if(hasToken)
  {
    hasToken = false;
    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(sensorData);
    Wire.endTransmission();

    PrintLine(TITLE_TEMPERATURE, otherSensorData);
    PrintLine(TITLE_HUMIDITY, sensorData);
    ResetCursor();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN);
    Wire.endTransmission();
  }
    sensorData = GATHER_DATA;
}
