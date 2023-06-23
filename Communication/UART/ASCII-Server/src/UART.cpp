#include "UART.hpp"
#include <Arduino.h>

#define STARTBIT 0
#define STOPBIT 1

constexpr int numberOfSamples = 1;
constexpr long MicroSecondsPerSecond = 1000000;
constexpr int RequiredSampleThreshold = (numberOfSamples / 2) + 1;
unsigned long SampleTime = 0;
unsigned long TimePerBit = 0;

constexpr int numberOfDataBits = 8;
constexpr int numberOfStopBits = 1;
constexpr int numberOfParityBits = 0;
constexpr int numberOfStartBits = 1;
constexpr int numberOfBits = numberOfDataBits + numberOfStopBits + numberOfParityBits + numberOfStartBits;
constexpr int BitsToSample = numberOfBits * numberOfSamples;

unsigned long nextBitTime = 0;

int exportByte = -1;
int requestedParity = 0;

int Bits[numberOfBits] = {0};
int SampleBits[numberOfSamples * numberOfBits] = {0};
int sampleCount = 0;
bool correctParity = false;

unsigned long startTime = 0;

UART::UART(int baudrate, int inputpin, int outputpin)
{
    this->baudRate = baudrate;
    this->inputPin = inputpin;
    this->outputPin = outputpin;

    pinMode(inputpin, INPUT);
    pinMode(outputpin, OUTPUT);

    SampleTime = (MicroSecondsPerSecond / baudrate) / numberOfSamples;
    TimePerBit = MicroSecondsPerSecond / baudrate;
}

int UART::CheckForStartBit()
{
    return !digitalRead(this->inputPin);
}

void UART::SampleByte()
{
    for (int i = 0; i < BitsToSample; i++)
    {
        while (micros() < nextBitTime)
        {
        }
        SampleBits[i] = digitalRead(this->inputPin);
        nextBitTime = micros() + SampleTime;
    }

    // for (int i = 0; i < BitsToSample; i++)
    // {
    //     while (micros() < nextBitTime)
    //     {
    //     }
    //     SampleBits[i] = PIND & 0b00000100;
    //     nextBitTime = micros() + SampleTime;
    // }

    for (int i = 0; i < numberOfBits; i++)
    {
        for (int j = 0; j < numberOfSamples; j++)
        {
            Bits[i] += SampleBits[i * numberOfSamples + j];
        }
        Bits[i] = (Bits[i] >= RequiredSampleThreshold);
    }
}

void Reset()
{
    sampleCount = 0;
    for (int i = 0; i < numberOfBits; i++)
    {
        Bits[i] = 0;
    }

    for (int i = 0; i < numberOfSamples; i++)
    {
        SampleBits[i] = 0;
    }
    
    exportByte = -1;
    correctParity = false;
}

bool CheckParity(int parityData)
{
    correctParity = (parityData % 2 == requestedParity);

    if(!correctParity)
    {
        Reset();
        return false;
    }
    else
    {
        return true;
    }
}

void ValidateByte()
{
    int parityData = 0;

    if(Bits[0] != 0)
    {
        Reset();
        return;
    }

    for (int i = numberOfStartBits + numberOfDataBits + numberOfParityBits; i < numberOfBits - numberOfStopBits; i++)
    {
        if(Bits[i] != 1)
        {
            Reset();
            return;
        }
    }

    if(numberOfParityBits > 0)
    {
        for (int i = numberOfStartBits; i < numberOfStartBits + numberOfDataBits; i++)
        {
            parityData += Bits[i];
        }
        CheckParity(parityData);
    }

    for (int i = 1; i < numberOfDataBits; i++)
    {
        exportByte += Bits[i] << (i - 1);
    }

}

bool UART::Recieve(char &data)
{
    if(CheckForStartBit())
    {
        SampleByte();
        ValidateByte();
        if(exportByte != -1)
        {
            data = (char)exportByte;
            Reset();
            return true;
        }
    }
    return false;
}

void UART::Send(char data)
{
  int outBoundData[numberOfBits] = {0};
  int partiyBit = 0;

  outBoundData[0] = STARTBIT;
  
  for (int i = numberOfStartBits; i < numberOfStartBits + numberOfDataBits; i++)
  {
    outBoundData[i] = (data >> (i - numberOfStartBits)) & 1;
  }

  if(numberOfParityBits > 0)
  {
    // Insert parity
    
    for (int i = numberOfStartBits; i < numberOfDataBits; i++)
    {
      partiyBit += outBoundData[i];
    }
    
    #ifdef EVEN_PARITY
    partiyBit = partiyBit % 2;
    #endif

    #ifdef ODD_PARITY
    partiyBit = (partiyBit % 2) + 1;
    #endif

    for (int i = numberOfStartBits + numberOfDataBits; i < numberOfStartBits + numberOfDataBits + numberOfParityBits; i++)
    {
      outBoundData[numberOfDataBits + i] = partiyBit;
    }
  }
  
  // Needs a for loop to add the stop bits
  for (int i = numberOfStartBits + numberOfDataBits + numberOfParityBits; i < numberOfBits; i++)
  {
    outBoundData[i] = STOPBIT;
  }

  unsigned long nextBitTime = micros();

  for (int i = 0; i < numberOfBits; i++)
  {
    while(micros() < nextBitTime)
    {
    }
    nextBitTime += TimePerBit;
    digitalWrite(outputPin, outBoundData[i]);
  }

  for (int i = 0; i < numberOfBits; i++)
  {
    outBoundData[i] = 0;
  }
  
  delay(1);
}

void UART::Send(int data)
{
    Send((char)data + '0');
}

void UART::SendLine(String string)
{
    for (size_t i = 0; i < string.length(); i++)
    {
        Send(string[i]);
    }
}

void UART::SetParity(int parity)
{
    requestedParity = parity;
}
