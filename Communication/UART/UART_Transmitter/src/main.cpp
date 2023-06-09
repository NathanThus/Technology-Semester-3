#include <Arduino.h>

#define BAUDRATE 300
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
}

void loop() {
  if(!Serial.available())
  {
    return;
  }

  char inboundByte = Serial.read();
  int bitCount = 0;

  int exportByte[numberOfBits];
  int partiyBit = 0;

  for (int i = 0; i < numberOfStartBits; i++)
  {
    exportByte[bitCount] = STARTBIT;
    bitCount++;
  }
  
  // Insert data
  for (int i = numberOfStartBits; i < numberOfDataBits; i++)
  {
    exportByte[bitCount] = (inboundByte >> i) & 1;
    partiyBit += exportByte[bitCount];
    bitCount++;
  }

  // // Insert parity
  // for (int i = numberOfDataBits; i < numberOfParityBits; i++)
  // {
  //   exportByte[bitCount] = partiyBit % 2; // TODO: Check if this is correct
  //   bitCount++;
  // }
  
  for (int i = numberOfDataBits + partiyBit; i < numberOfStopBits; i++)
  {
    exportByte[bitCount] = STOPBIT;
    bitCount++;
  }
  

  bitCount = 0;
//TRANSMITTING
  while(bitCount < numberOfBits)
  {

    bitCount++;
  }
}

