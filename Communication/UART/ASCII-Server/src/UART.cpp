#include "UART.hpp"
#include <Arduino.h>

#define STARTBIT 0
#define STOPBIT 1

#define EVEN_PARITY

#ifdef ODD_PARITY
#define REQUESTED_PARITY 1
#endif
#ifdef EVEN_PARITY
#define REQUESTED_PARITY 0
#endif

// Timing
const long MicroSecondsPerSecond = 1000000;

// Data Bits
const int NumberOfDataBits = 8;
const int NumberOfStopBits = 1;
const int NumberOfParityBits = 0;
const int NumberOfStartBits = 1;

const int NumberOfBits = NumberOfStartBits + NumberOfDataBits + NumberOfParityBits + NumberOfStopBits;

// Sampling
const int NumberOfSamples = 1;
const int BitsToSample = NumberOfBits * NumberOfSamples;
const int RequiredSampleThreshold = (NumberOfSamples / 2) + 1;

// Required for data between function, unless I completely refactor with bit-shifting
int Bits[NumberOfBits];
int SampleBits[BitsToSample];

UART::UART(int baudrate, int inputpin, int outputpin, int samples)
    : SampleTime((MicroSecondsPerSecond / baudrate) / samples),
      TimePerBit(MicroSecondsPerSecond / baudrate),
      NumberOfSamples(samples),
      BaudRate(baudrate),
      InputPin(inputpin),
      OutputPin(outputpin)
{
    pinMode(inputpin, INPUT);
    pinMode(outputpin, OUTPUT);
}

int UART::CheckForStartBit()
{
    return !digitalRead(this->InputPin);
}

void UART::SampleByte()
{
    unsigned long nextBitTime = micros();
    for (int i = 0; i < BitsToSample; i++)
    {
        while (micros() < nextBitTime)
        {
        }
        SampleBits[i] = digitalRead(this->InputPin);
        nextBitTime += SampleTime;
    }

    for (int i = 0; i < NumberOfBits; i++)
    {
        for (int j = 0; j < NumberOfSamples; j++)
        {
            Bits[i] += SampleBits[i * NumberOfSamples + j];
        }
        Bits[i] = (Bits[i] >= RequiredSampleThreshold);
    }
}

void UART::Reset()
{
    for (int i = 0; i < NumberOfBits; i++)
    {
        Bits[i] = 0;
    }

    for (int i = 0; i < NumberOfSamples; i++)
    {
        SampleBits[i] = 0;
    }
}

bool UART::CheckParity(int parityData)
{
    return ((parityData + Bits[NumberOfStartBits + NumberOfDataBits]) % 2 == REQUESTED_PARITY);
}

bool UART::ValidateByte()
{
    int parityData = 0;

    if (!ValidateStartBits())
    {
        Reset();
        return false;
    }

    if (!ValidateStopBits())
    {
        Reset();
        return false;
    }

    if (NumberOfParityBits > 0)
    {
        parityData = PreCalculateParityData();
        if (!CheckParity(parityData))
        {
            Reset();
            return false;
        }
    }

    return true;
}

char UART::RetrieveData()
{
    int data = 0;
    for (int i = NumberOfStartBits; i < NumberOfStartBits + NumberOfDataBits; i++)
    {
        data += Bits[i] << (i - NumberOfStartBits);
    }
    return (char)data;
}

bool UART::ValidateStartBits()
{
    if (Bits[0] != 0)
    {
        Reset();
        return false;
    }
    return true;
}

bool UART::ValidateStopBits()
{
    for (int i = NumberOfStartBits + NumberOfDataBits + NumberOfParityBits; i < NumberOfBits - NumberOfStopBits; i++)
    {
        if (Bits[i] != 1)
        {
            Reset();
            return false;
        }
    }
    return true;
}

int UART::PreCalculateParityData()
{
    int parityData = 0;
    for (int i = NumberOfStartBits; i < NumberOfStartBits + NumberOfDataBits; i++)
    {
        parityData += Bits[i];
    }
    return parityData;
}

bool UART::Receive(char &data)
{
    if (CheckForStartBit())
    {
        SampleByte();
        if (ValidateByte())
        {
            data = (char)RetrieveData();
            Reset();
            return true;
        }
    }
    return false;
}

void UART::Send(char data) // Absolute Spaghetti
{
    int outBoundData[NumberOfBits] = {0};
    int parityBit = 0;

    outBoundData[0] = STARTBIT;

    for (int i = NumberOfStartBits; i < NumberOfStartBits + NumberOfDataBits; i++)
    {
        outBoundData[i] = (data >> (i - NumberOfStartBits)) & 1;
    }

    if (NumberOfParityBits > 0)
    {
        for (int i = NumberOfStartBits; i < NumberOfDataBits; i++)
        {
            parityBit += outBoundData[i];
        }

#ifdef EVEN_PARITY
        parityBit = parityBit % 2;
#endif

#ifdef ODD_PARITY
        parityBit = (parityBit % 2) + 1;
#endif

        for (int i = NumberOfStartBits + NumberOfDataBits; i < NumberOfStartBits + NumberOfDataBits + NumberOfParityBits; i++)
        {
            outBoundData[NumberOfDataBits + i] = parityBit;
        }
    }

    // Needs a for loop to add the stop bits
    for (int i = NumberOfStartBits + NumberOfDataBits + NumberOfParityBits; i < NumberOfBits; i++)
    {
        outBoundData[i] = STOPBIT;
    }

    unsigned long nextBitTime = micros();

    for (int i = 0; i < NumberOfBits; i++)
    {
        while (micros() < nextBitTime)
        {
        }
        nextBitTime += TimePerBit;
        digitalWrite(OutputPin, outBoundData[i]);
    }

    for (int i = 0; i < NumberOfBits; i++)
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