#include <Arduino.h>

#define BAUDRATE 9600
#define STARTBIT 0
#define STOPBIT 1

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
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  // if(!Serial.available())
  // {
  //   return;
  // }

  // char inboundByte = Serial.read();

  int exportByte[numberOfBits] = {0};
  // int partiyBit = 0;

  // exportByte[0] = STARTBIT;
  
  // Insert data //TODO: Fix
  // for (int i = numberOfStartBits; i < numberOfDataBits; i++)
  // {
  //   exportByte[i] = (inboundByte >> (i - 1)) & 1; //TODO: Might not be optimal.
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
  exportByte[9] = 1;

  // Insert parity
  // for (int i = numberOfDataBits; i < numberOfParityBits; i++)
  // {
  //   exportByte[i] = partiyBit % 2; // TODO: Check if this is correct
  //   i++;
  // }
  
  // for (int i = numberOfDataBits + partiyBit; i < numberOfStopBits; i++)
  // {
  //   exportByte[i] = STOPBIT;
  // }

  // int bitcount = 0;
  unsigned long nextBitTime = micros();
  //OLD
  // while (bitcount < numberOfBits)
  // {
  //   if (micros() >= nextBitTime)
  //   {
  //     nextBitTime = micros() + TimePerBit;
  //     digitalWrite(2, exportByte[bitcount]);
  //     ++bitcount;
  //   }
  // }

  for (int i = 0; i < numberOfBits; i++)
  {
    // digitalWrite(13,!digitalRead(13));
    while(micros() < nextBitTime)
    {
    }
    nextBitTime = micros() + TimePerBit;
    digitalWrite(2, exportByte[i]);
  }
  delay(100);
}

