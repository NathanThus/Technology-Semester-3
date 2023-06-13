#include <Arduino.h>

// SETTINGS

#define BAUDRATE 9600
#define INPUT_PIN 2

// Define a set of prescalers & Compare Registers, equivelant to thrice the baud rate for each baud rate
// DEBUG
#define TOGGLE_LED PORTB ^= (1 << PORTB5)

enum Event
{
    NONE,
    START_BIT_RECEIVED,
    BYTE_RECEIVED,
    BYTE_REPORTED,
    STOP_BIT_RECEIVED,
    BYTE_VALIDATION_FAILED,
    BYTE_VALIDATION_PASSED,
    BYTE_ADDED_TO_BUFFER

};

enum State
{
    IDLE,
    REPORT_DATA,
    READ_BIT,
    BYTE_VALIDATION,
    ADD_TO_BUFFER
};

constexpr long numberOfSamples = 1;
constexpr long MicroSecondsPerSecond = 1000000;
const unsigned long SampleTime = (MicroSecondsPerSecond / BAUDRATE) / numberOfSamples;
constexpr int RequiredSampleThreshold =  (numberOfSamples / 2) + 1;

constexpr int numberOfDataBits = 8;
constexpr int numberOfStopBits = 1;
constexpr int numberOfParityBits = 0;
constexpr int numberOfStartBits = 1;
constexpr int numberOfBits = numberOfDataBits + numberOfStopBits + numberOfParityBits + numberOfStartBits;
constexpr int BitsToSample = numberOfBits * numberOfSamples;

unsigned long nextBitTime = 0;

int exportByte = 0;

int Bits[numberOfBits] = {0};
int SampleBits[numberOfSamples * numberOfBits] = {0};
int sampleCount = 0;
bool correctParity = false; //TODO Optimise the everloving .... out of this

unsigned long startTime = 0;

State currentState = IDLE;
Event newEvent = NONE;

void setup()
{
    pinMode(INPUT_PIN, INPUT); 
    pinMode(13, OUTPUT);
    Serial.begin(BAUDRATE);
}

/// ================= AUXILLARY FUNCTIONS ================= ///
#pragma region Auxillary Functions
int CheckForStartBit()
{
    return !(PIND & 0b00000100);
}

void SampleByte()
{
    for (int i = 0; i < BitsToSample; i++)
    {
        while (micros() < nextBitTime)
        {
        }
        SampleBits[i] = PIND & 0b00000100;
        nextBitTime = micros() + SampleTime;
    }
    // TIMING ISSUE

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
    
    exportByte = 0;
    correctParity = false;
}

bool CheckParity(int parityData)
{
    // correctParity = (parityData % 2 == PARITY_BIT); // TODO: Fix this

    if(!correctParity)
    {
        Reset();
        currentState = IDLE;
        return false;
    }
    else
    {
        currentState = ADD_TO_BUFFER;
        return true;
    }
}

void ValidateByte()
{
    int parityData = 0;

    if(Bits[0] != 0)
    {
        Reset();
        currentState = IDLE;
        return;
    }

    for (int i = numberOfStartBits + numberOfDataBits + numberOfParityBits; i < numberOfBits - numberOfStopBits; i++)
    {
        if(Bits[i] != 1)
        {
            Reset();
            currentState = IDLE;
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

    for (int i = 1; i < numberOfDataBits; i++) // FINALLY EXPORT THE BYTE
    {
        exportByte += Bits[i] << (i - 1);
    }

}

void StartBitFound()
{
    startTime = 0;
    currentState = READ_BIT;
}
#pragma endregion
/// ================= STATE MACHINE ================= ///
#pragma region State Machine
// Need functions for all states

void Handle_ReadBit(Event ev)
{
    SampleByte();
    currentState = BYTE_VALIDATION;
    return;
}

void Handle_Idle(Event ev)
{
    if(CheckForStartBit())
    {
        StartBitFound();
        currentState = READ_BIT;
        return;
    }

    if(ev == BYTE_RECEIVED)
    {
        currentState = REPORT_DATA;
        return;
    }
    if(ev == BYTE_VALIDATION_FAILED)
    {
        Reset();
        return;
    }
}

void Handle_ReportData(Event ev)
{
    Serial.println();
    Serial.print("Echo: ");
    Serial.println((char)exportByte);
    Reset();
    currentState = IDLE;
}

void Handle_ByteValidation(Event ev)
{
    ValidateByte();
    if(correctParity)
    {
        currentState = ADD_TO_BUFFER;
    }
    else
    {
        newEvent = BYTE_VALIDATION_FAILED;
        currentState = IDLE;
    }
    currentState = ADD_TO_BUFFER;
}

void Handle_AddToBuffer(Event ev)
{
    currentState = REPORT_DATA;
}
#pragma endregion

void loop()
{
    switch (currentState)
    {
    case IDLE:
        Handle_Idle(newEvent);
        break;
    case REPORT_DATA:
        Handle_ReportData(newEvent);
        break;
    case READ_BIT:
        Handle_ReadBit(newEvent);
        break;
    case BYTE_VALIDATION:
        Handle_ByteValidation(newEvent);
        break;
    case ADD_TO_BUFFER:
        Handle_AddToBuffer(newEvent);
        break;
    default:
        break;
    }
}
