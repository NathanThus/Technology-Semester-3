#include <Arduino.h>
#include <Wire.h>

// I2C ADDRESSES

#define MY_ADDRESS 0x01
#define HUMIDITY_ADDRESS 0x02
#define DISPLAY_ADDRESS 0x03

// M2M COMMUNICATION

#define MARK_AS_SLAVE 0x0
#define MARK_AS_MASTER 0x1

// DISPLAY COMMUNICATION

#define TEMPERATURE_REGISTER 0x0

// REGISTER

volatile bool isSlave = 0;
volatile int sensorData = 0; // Is only used for outbound communication

void onRecieve(int howMany)
{
  while(Wire.available())
  {
    int inbound = Wire.read();
  }

  if(inbound == 0)
  {
    isSlave = false;
  }
  else if(inbound == 1)
  {
    isSlave = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(MY_ADDRESS);
  Wire.onReceive(onRecieve);
}

void loop() {
  if(!isSlave)
  {
    Wire.beginTransmission(HUMIDITY_ADDRESS);
    Wire.write(MARK_AS_SLAVE);
    Wire.endTransmission();

    Wire.beginTransmission(DISPLAY_ADDRESS);
    Wire.write();
    Wire.write(sensorData);
    Wire.endTransmission();

    Wire.beginTransmission(HUMIDITY_ADDRESS);
    Wire.write(MARK_AS_MASTER);
    Wire.endTransmission();
  }

  delay(50);
}