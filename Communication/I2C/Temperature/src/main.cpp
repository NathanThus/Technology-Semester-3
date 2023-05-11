#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
// I2C ADDRESSES

#define MY_ADDRESS 50
#define HUMIDITY_ADDRESS 60
#define DISPLAY_ADDRESS 3

// M2M COMMUNICATION

#define MARK_AS_SLAVE 10
#define MARK_AS_MASTER 20

// DISPLAY COMMUNICATION

#define TEMPERATURE_REGISTER 0

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

  pinMode(8, OUTPUT);
}

void loop()
{
  if(isSlave == MARK_AS_MASTER)
  {
    // I can now send my messages!
    Wire.beginTransmission(HUMIDITY_ADDRESS);
    Wire.write(MARK_AS_SLAVE);
    Wire.endTransmission();

    Serial.println(MARK_AS_SLAVE);

    digitalWrite(8, HIGH);
    delay(1000);

    Wire.beginTransmission(HUMIDITY_ADDRESS);
    Wire.write(MARK_AS_MASTER);
    Wire.endTransmission();

    Serial.println(MARK_AS_MASTER);

    digitalWrite(8, LOW);
    delay(1000);
  }
}