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
  if(Serial.available() == 0)
  {
    return;
  }

  char inboundByte = Serial.read();

  int exportByte[numberOfBits] = {0};
  int parityBit = 0;

  exportByte[0] = STARTBIT;
  
  for (int i = numberOfStartBits; i < numberOfStartBits + numberOfDataBits; i++)
  {
    exportByte[i] = (inboundByte >> (i - numberOfStartBits)) & 1;
  }

  if(numberOfParityBits > 0)
  {
    // Insert parity
    
    for (int i = numberOfStartBits; i < numberOfDataBits; i++)
    {
      parityBit += exportByte[i];
    }
    
    parityBit = parityBit % 2;

    if(parityBit != REQUESTED_PARITY)
    {
      parityBit += 1;
    }

    exportByte[numberOfStartBits + numberOfDataBits] = parityBit;
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

  for (size_t i = 0; i < numberOfBits; i++)
  {
    Serial.println(exportByte[i]);
  }
  
  delay(100);
}
