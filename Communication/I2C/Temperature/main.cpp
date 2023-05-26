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

// DEVICE SPECIFIC
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

#define TITLE_TEMPERATURE "C1: "
#define TITLE_HUMIDITY "C2: "

// M2M COMMUNICATION
#define TOKEN 0
#define TOKEN_INDICATOR 'T'
#define DATA_INDICATOR 'D'

// REGISTER
bool hasToken = false;
byte sensorData = 1; // Is only used for outbound communication
byte otherSensorData = 0; // Is only used for inbound communication

// DHT11
DHT dht11(A0, DHT11);

int messageCounter = 0;
char index = ' ';

void onRecieve(int howMany)
{
  int inbound = 0;

  if(messageCounter == 0)
  {
    index = (char)Wire.read();
    messageCounter++;
  }
  else
  {
    if(index == TOKEN_INDICATOR)
    {
      inbound = Wire.read();
      if(inbound == TOKEN)
      {
        hasToken = true;
      }
    }
    else if(index == DATA_INDICATOR)
    {
      inbound = Wire.read();
      otherSensorData = inbound;
    }
    index = ' ';
    messageCounter = 0;
  }
}

void setup()
{
  // dht11.begin();
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
  String outputData;
  if(data < 100)
  {
    outputData = "0";
    outputData += data;
  }
  else
  {
    outputData = String(data);
  }
  oled.print(outputData);
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
  digitalWrite(13, hasToken);
  if(hasToken)
  {
    hasToken = false;

    sensorData += 2;

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(DATA_INDICATOR);
    Wire.endTransmission();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(sensorData);
    Wire.endTransmission();

    PrintLine(TITLE_TEMPERATURE, otherSensorData);
    PrintLine(TITLE_HUMIDITY, sensorData);
    ResetCursor();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN_INDICATOR);
    Wire.endTransmission();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN);
    Wire.endTransmission();

  }
    // sensorData = GATHER_DATA;
}
