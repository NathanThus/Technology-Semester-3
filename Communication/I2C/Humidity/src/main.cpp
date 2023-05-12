#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
// I2C ADDRESSES

#define MY_ADDRESS 60
#define TEMPERATURE_ADDRESS 50
#define DISPLAY_ADDRESS 3

// M2M COMMUNICATION

#define MARK_AS_SLAVE 10
#define MARK_AS_MASTER 20

// DISPLAY COMMUNICATION

#define HUMIDITY_REGISTER 1

// REGISTER

int busState = MARK_AS_MASTER;
int sensorData = 0; // Is only used for outbound communication

// DHT11
DHT dht11(A0, DHT11);

int testData = 0;

void onData(int howMany)
{
  int inbound = -1;
  while(Wire.available() > 0)
  {
    inbound = Wire.read();
  }
  if(inbound != -1)
  {
    busState = inbound;
    testData = inbound;
  }

  Wire.flush();
}

void setup() {
  // put your setup code here, to run once:
  dht11.begin();
  Serial.begin(9600);
  Wire.onReceive(onData);
  Wire.setTimeout(1000);
  Wire.begin(MY_ADDRESS);

  pinMode(8,OUTPUT);
}

void loop()
{
  if(busState == MARK_AS_MASTER)
  {
    
    // I can now send my messages!
    Wire.beginTransmission(TEMPERATURE_ADDRESS);
    Wire.write(MARK_AS_SLAVE);
    Wire.endTransmission();
    
    digitalWrite(8, HIGH);
    delay(50);

    Wire.beginTransmission(TEMPERATURE_ADDRESS);
    Wire.write(MARK_AS_MASTER);
    Wire.endTransmission();
    delay(random(1,50));
  }
  else if (busState == MARK_AS_SLAVE)
  {
    digitalWrite(8, LOW);
    delay(1000);
  }

  if(testData != 0)
  {
    Serial.println(testData);
    testData = 0;
  }
}