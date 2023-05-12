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
#define DISPLAY_ADDRESS 3

// DEVICE ID
#define __TEMPERATURE__

#ifdef __HUMIDITY__
#define MY_ADDRESS HUMIDITY_ADDRESS
#define OTHER_DEVICE_ADDRESS TEMPERATURE_ADDRESS
#define CURSOR_Y 16
#define TITLE "H: "
#define GATHER_DATA random(0,100)
#endif

#ifdef __TEMPERATURE__
#define MY_ADDRESS TEMPERATURE_ADDRESS
#define OTHER_DEVICE_ADDRESS HUMIDITY_ADDRESS
#define CURSOR_Y 0
#define TITLE "T: "
#define GATHER_DATA random(100,200)
#endif

// M2M COMMUNICATION

#define TOKEN 69

// DISPLAY COMMUNICATION

// REGISTER
bool hasToken = true;
int sensorData = 0; // Is only used for outbound communication

// DHT11
DHT dht11(A0, DHT11);

// ONLY USED FOR M2M COMMUNICATION
void onRecieve(int howMany)
{
  int inbound = -1;
  while (Wire.available())
  {
    inbound = Wire.read();
  }

  if(inbound == TOKEN)
  {
    hasToken = true;
  }
}

void setup()
{
  dht11.begin();
  Wire.onReceive(onRecieve);
  Wire.begin(MY_ADDRESS);

  // Add display code
  oled.begin();
  oled.setFontType(1);
  oled.clear(ALL);
  oled.display();
  oled.clear(PAGE);
}

void PrintLine(String title, int data)
{
  oled.print(title);
  oled.print(data);
  oled.display();
  cursorY += 16;
  oled.setCursor(0, cursorY);
}

void ResetCursor()
{
  oled.setCursor(0, CURSOR_Y);
}

void loop()
{
  if(hasToken)
  {
    hasToken = false;

    PrintLine(TITLE, sensorData);
    ResetCursor();
    oled.clear(PAGE);

    Wire.beginTransmission(OTHER_DEVICE_ADDRESS);
    Wire.write(TOKEN);
    Wire.endTransmission();
  }
  else
  {
    sensorData = GATHER_DATA;
  }
}