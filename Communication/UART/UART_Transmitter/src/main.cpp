#include <Arduino.h>

#define BAUDRATE 9600
#define STARTBIT 0
#define STOPBIT 1
#define OUTPUT_PIN 2

constexpr long MicroSecondsPerSecond = 1000000;
constexpr long TimePerBit = MicroSecondsPerSecond / BAUDRATE;

constexpr int numberOfDataBits = 8;
constexpr int numberOfStopBits = 1;
constexpr int numberOfParityBits = 0;
constexpr int numberOfStartBits = 1;
constexpr int numberOfBits = numberOfDataBits + numberOfStopBits + numberOfParityBits + numberOfStartBits;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  pinMode(13, OUTPUT); // TODO: Remove
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  // if(!Serial.available())
  // {
  //   return;
  // }

  // char inboundByte = 'a';

  int exportByte[numberOfBits] = {0};
  // int partiyBit = 0;
  // exportByte[0] = STARTBIT;

  
  // for (int i = numberOfDataBits; i >= numberOfStartBits; i--)
  // {
  //   exportByte[numberOfBits - i] = (inboundByte >> (i - 1)) & 1;
  // }

  //Fill exportByte with the data of 'a'
  exportByte[0] = 0;
  exportByte[1] = 1;
  exportByte[2] = 0;
  exportByte[3] = 0;
  exportByte[4] = 0;
  exportByte[5] = 0;
  exportByte[6] = 1;
  exportByte[7] = 1;
  exportByte[8] = 0;

  // Insert parity
  // for (int i = numberOfDataBits; i < numberOfParityBits; i++)
  // {
  //   exportByte[i] = partiyBit % 2; // TODO: Check if this is correct
  //   i++;
  // }
  
  for (int i = numberOfDataBits + numberOfParityBits; i < numberOfStopBits; i++)
  {
    exportByte[i] = STOPBIT;
  }

  unsigned long nextBitTime = micros();

  for (int i = 0; i < numberOfBits; i++)
  {
    while(micros() < nextBitTime)
    {
    }
    nextBitTime = micros() + TimePerBit;
    digitalWrite(OUTPUT_PIN, exportByte[i]);
  }
  delay(100);
}

