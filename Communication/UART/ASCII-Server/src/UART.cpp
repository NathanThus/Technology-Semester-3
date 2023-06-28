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
const int CollisionDelay = 1;

// Data Bits - Could do some presets with defines, but at this stage of the project, no thanks
const int NumberOfDataBits = 8;
const int NumberOfStopBits = 1;
const int NumberOfParityBits = 0;
const int NumberOfStartBits = 1;
constexpr int NumberOfBits = NumberOfStartBits + NumberOfDataBits + NumberOfParityBits + NumberOfStopBits;

// Positioning
constexpr int DataBitPosition = NumberOfStartBits;
constexpr int ParityBitPosition = DataBitPosition + NumberOfDataBits;
constexpr int StopBitPosition = ParityBitPosition + NumberOfParityBits;

// Lengths
constexpr int DataBitsLength = NumberOfStartBits + NumberOfDataBits;
constexpr int ParityBitsLength = DataBitsLength + NumberOfParityBits;
constexpr int StopBitsLength = ParityBitsLength + NumberOfStopBits;

// Sampling
const int NumberOfSamples = 1;
const int BitsToSample = NumberOfBits * NumberOfSamples;
const int RequiredSampleThreshold = (NumberOfSamples / 2) + 1;

// Required for data between function, unless I completely refactor with bit-shifting or array pointers
int Bits[NumberOfBits];
int SampleBits[BitsToSample];

UART::UART(int baudrate, int inputpin, int outputpin, int samples)
    : SampleTime((MicroSecondsPerSecond / baudrate) / samples),
      TimePerBit(MicroSecondsPerSecond / baudrate),
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
    return ((parityData + Bits[ParityBitPosition]) % 2 == REQUESTED_PARITY);
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
    for (int i = DataBitPosition; i < DataBitsLength; i++)
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
    for (int i = StopBitPosition; i < StopBitsLength; i++)
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
    for (int i = DataBitPosition; i < DataBitsLength; i++)
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

void UART::SetStartBit()
{
    Bits[0] = STARTBIT;
}

void UART::SetDataBits(int data)
{
    for (int i = DataBitPosition; i < DataBitsLength; i++)
    {
        Bits[i] = (data >> (i - NumberOfStartBits)) & 1;
    }
}

void UART::SetParityData(int parityData)
{
    for (int i = ParityBitPosition; i < ParityBitsLength; i++)
    {
        Bits[NumberOfDataBits + i] = parityData;
    }
}

void UART::SetStopBits()
{
    for (int i = StopBitPosition; i < StopBitsLength; i++)
    {
        Bits[i] = STOPBIT;
    }
}

void UART::PrepareData(int data)
{
    Reset();

    SetStartBit();
    SetDataBits(data);

    if (NumberOfParityBits > 0)
    {
        int parityBit = PreCalculateParityData();

        #ifdef EVEN_PARITY
                parityBit = parityBit % 2;
        #endif

        #ifdef ODD_PARITY
                parityBit = (parityBit % 2) + 1;
        #endif

        SetParityData(parityBit);
    }

    SetStopBits();
}

void UART::Send(char data)
{
    PrepareData(data);
    unsigned long nextBitTime = micros();

    for (int i = 0; i < NumberOfBits; i++)
    {
        while (micros() < nextBitTime)
        {
        }
        nextBitTime += TimePerBit;
        digitalWrite(OutputPin, Bits[i]);
    }

    Reset();
    delay(CollisionDelay);
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