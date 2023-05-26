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
#endif

#ifdef __TEMPERATURE__
#define MY_ADDRESS TEMPERATURE_ADDRESS
#define OTHER_DEVICE_ADDRESS HUMIDITY_ADDRESS
#define CURSOR_Y 0
#endif

// GENERAL

#define TITLE_COUNTER_ONE "C1: "
#define TITLE_COUNTER_TWO "C2: "

// M2M COMMUNICATION
#define TOKEN 0
#define TOKEN_INDICATOR 'T'
#define DATA_INDICATOR 'D'

// REGISTER
bool hasToken = false;
byte sensorData = 1; // Is only used for outbound communication
byte otherSensorData = 0; // Is only used for inbound communication

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
  Wire.onReceive(onRecieve);
  Wire.begin(MY_ADDRESS);

  delay(1000);
  oled.begin();
  oled.setFontType(0);
  oled.clear(CMD_CLEAR);
  oled.display();
}

void PrintData(int counterOneData, int counterTwoData)
{
  oled.clear(CMD_CLEAR);

  oled.print(TITLE_COUNTER_ONE);
  oled.println(counterOneData);

  oled.print(TITLE_COUNTER_TWO);
  oled.println(counterTwoData);
  
  oled.display();
  oled.setCursor(0,0);
}

void loop()
{
  digitalWrite(13, hasToken);
  if(hasToken)
  {
    hasToken = false;

    sensorData += 5;

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(DATA_INDICATOR);
    Wire.endTransmission();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(sensorData);
    Wire.endTransmission();

    PrintData(sensorData, otherSensorData);

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN_INDICATOR);
    Wire.endTransmission();

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN);
    Wire.endTransmission();
  }
}
