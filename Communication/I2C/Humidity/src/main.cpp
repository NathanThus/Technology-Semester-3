#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
// I2C ADDRESSES

#define MY_ADDRESS 0x02
#define TEMPERATURE_ADDRESS 0x01
#define DISPLAY_ADDRESS 0x03

// M2M COMMUNICATION

#define MARK_AS_SLAVE 0x0
#define MARK_AS_MASTER 0x1

// DISPLAY COMMUNICATION

#define HUMIDITY_REGISTER 0x1

// REGISTER

volatile bool isSlave = 0;
volatile int sensorData = 0; // Is only used for outbound communication

// DHT11
DHT dht11(A2, DHT11);

void onRecieve(int howMany)
{
  int inbound = -1;
  while(Wire.available())
  {
    inbound = Wire.read();
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
    Wire.beginTransmission(TEMPERATURE_ADDRESS);
    Wire.write(MARK_AS_SLAVE);
    Wire.endTransmission();

    Wire.beginTransmission(DISPLAY_ADDRESS);
    Wire.write(HUMIDITY_REGISTER);
    Wire.write(sensorData);
    Wire.endTransmission();

    Wire.beginTransmission(TEMPERATURE_ADDRESS);
    Wire.write(MARK_AS_MASTER);
    Wire.endTransmission();
  }
  else
  {
    sensorData = (int)dht11.readHumidity();
  }

  delay(50);
}