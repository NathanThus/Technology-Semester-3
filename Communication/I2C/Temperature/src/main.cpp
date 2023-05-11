#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
// I2C ADDRESSES

#define MY_ADDRESS 0x1
#define HUMIDITY_ADDRESS 0x2
#define DISPLAY_ADDRESS 0x3

// M2M COMMUNICATION

#define MARK_AS_SLAVE 0x0
#define MARK_AS_MASTER 0x1

// DISPLAY COMMUNICATION

#define TEMPERATURE_REGISTER 0x0

// REGISTER

volatile bool isSlave = 0;
volatile int sensorData = 0; // Is only used for outbound communication

// DHT11
DHT dht11(A0, DHT11);

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
  dht11.begin();
  Serial.begin(9600);
  Wire.onReceive(onRecieve);
  Wire.begin(MY_ADDRESS);
}

void loop() {
  if(isSlave == 0)
  {
    // Wire.beginTransmission(HUMIDITY_ADDRESS);
    // Wire.write(MARK_AS_SLAVE);
    // Wire.endTransmission();
    Wire.beginTransmission(DISPLAY_ADDRESS);
    // Wire.write(TEMPERATURE_REGISTER);
    Wire.write(sensorData);
    Wire.endTransmission();

    // Wire.beginTransmission(HUMIDITY_ADDRESS);
    // Wire.write(MARK_AS_MASTER);
    // Wire.endTransmission();
  }
  sensorData = dht11.readTemperature();
  Serial.println(sensorData);

  delay(1000);
}