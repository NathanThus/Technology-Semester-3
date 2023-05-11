#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
// I2C ADDRESSES

#define MY_ADDRESS 60
#define TEMPERATURE_ADDRESS 50
#define DISPLAY_ADDRESS 3

// M2M COMMUNICATION

#define MARK_AS_SLAVE 10
#define MARK_AS_MASTER 10

// DISPLAY COMMUNICATION

#define HUMIDITY_REGISTER 1

// REGISTER

int isSlave = MARK_AS_MASTER;
int sensorData = 0; // Is only used for outbound communication

// DHT11
DHT dht11(A0, DHT11);

void onData(int howMany)
{
  int inbound = Wire.read();

  if(inbound != -1)
  {
    isSlave = inbound;
  }

  Wire.flush();
}

void setup() {
  // put your setup code here, to run once:
  dht11.begin();
  Serial.begin(9600);
  Wire.onReceive(onData);
  Wire.begin(MY_ADDRESS);

  pinMode(8,OUTPUT);
}

void loop()
{
  if(isSlave == 10)
  {
    digitalWrite(8,LOW);
  }
  if(isSlave == 20)
  {
    digitalWrite(8,HIGH);
  }

  Serial.println(isSlave);

  delay(50);
}