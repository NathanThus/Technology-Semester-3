#include <Arduino.h>

#define BAUDRATE 9600
#define STARTBIT 0
#define STOPBIT 1
#define OUTPUT_PIN 2
#define ODD_PARITY

constexpr long MicroSecondsPerSecond = 1000000;
constexpr long TimePerBit = MicroSecondsPerSecond / BAUDRATE;

constexpr int numberOfDataBits = 8;
constexpr int numberOfStopBits = 1;
constexpr int numberOfParityBits = 0;
constexpr int numberOfStartBits = 1;
constexpr int numberOfBits = numberOfDataBits + numberOfStopBits + numberOfParityBits + numberOfStartBits;

void setup() {
  Serial.begin(BAUDRATE);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {

  char inboundByte = 'a';

  int exportByte[numberOfBits] = {0};
  int partiyBit = 0;

  exportByte[0] = STARTBIT;
  
  for (int i = numberOfStartBits; i < numberOfStartBits + numberOfDataBits; i++)
  {
    exportByte[i] = inboundByte & 128;
    inboundByte <<= 1;
  }

  if(numberOfParityBits > 0)
  {
    // Insert parity
    
    for (int i = numberOfStartBits; i < numberOfDataBits; i++)
    {
      partiyBit += exportByte[i];
    }
    
    #ifdef EVEN_PARITY
    partiyBit = partiyBit % 2;
    #endif

    #ifdef ODD_PARITY
    partiyBit = (partiyBit % 2) + 1;
    #endif

    for (int i = numberOfStartBits + numberOfDataBits; i < numberOfStartBits + numberOfDataBits + numberOfParityBits; i++)
    {
      exportByte[numberOfDataBits + i] = partiyBit;
    }
  }
  
  // Needs a for loop to add the stop bits
  for (int i = numberOfStartBits + numberOfDataBits + numberOfParityBits; i < numberOfBits; i++)
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
