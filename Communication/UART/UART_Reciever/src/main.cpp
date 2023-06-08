#include <Arduino.h>

// SETTINGS

#define BAUD_300
#define NO_PARITY

// Define a set of prescalers, equivelant to thrice the baud rate

#ifdef BAUD_9600
#define COMPARE_REGISTER 34
#endif
#ifdef BAUD_300
#define COMPARE_REGISTER 166
#endif


#ifdef NO_PARITY
#define PARITY_BIT -1
#endif
#ifdef PARTITY_EVEN
#define PARITY_BIT 0
#endif
#ifdef PARTITY_ODD
#define PARITY_BIT 1
#endif

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

constexpr int baudRate = 300;
constexpr long MicroSecondsPerSecond = 1000000;
constexpr long SampleTime = MicroSecondsPerSecond / baudRate; // NOTE: Unused
constexpr long MaximumBits = 12;
constexpr long NumberOfSamples = 3;
constexpr int ParityBit = 9;

int exportByte = 0;

int Bits[MaximumBits];
int SampleBits[MaximumBits];
int sampleCount = 0;
int bitCount = 0;
bool correctParity = false;

unsigned long startTime = 0;

State currentState = IDLE;
Event newEvent = NONE;

void setup()
{
    cli(); // Cease all Interrupts for now

    // Using COMPARE_REGISTER, set up timer 1 to interrupt every 3 times the baud rate, so we can sample thrice!
    TCCR1A = 0; // Set all bits to 0
    TCCR1B = 0; // Set all bits to 0
    TCNT1 = 0; // Set timer to 0
    OCR1A = COMPARE_REGISTER; // Set compare register to 34
    TCCR1B |= (1 << WGM12); // Set CTC mode
    TCCR1B |= (1 << CS10); // Set prescaler to 1
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt

    sei(); // Renable Interrupts
    Serial.begin(baudRate);
}

/// ================= AUXILLARY FUNCTIONS ================= ///

int CheckForStartBit()
{
    return !(PINB & 0b00000001);
}

int ReadBit()
{
    return (PINB & 0b00000001);
}

void SampleBit()
{
    SampleBits[sampleCount] = ReadBit();
    sampleCount++;
}

int ValidateBit() //TODO: Might be a bit too slow
{
    int total = 0;
    for (int i = 0; i < NumberOfSamples; i++)
    {
        total += SampleBits[i];
    }
    
    return total >= 2;
}

ISR(TIMER1_COMPA_vect)
{
    TIFR1 |= (1 << OCF1A); // Reset the interrupt flag //TODO: Put this in a define?
    // Sample bit
    SampleBit();
    if(sampleCount == 3)
    {
        Bits[bitCount] = ValidateBit();
        bitCount++;
        sampleCount = 0;
    }
}

void Reset()
{
    bitCount = 0;
    sampleCount = 0;
    for (int i = 0; i < MaximumBits; i++)
    {
        Bits[i] = 0;
        SampleBits[i] = 0;
    }
    exportByte = 0;
    correctParity = false;
}

void ValidateByte()
{
    int parityData = 0;
    // For now, just tally the bits and move to the add buffer.
    for (int i = 1; i <= 8; i++)
    {
        exportByte += (Bits[i] << i);
        if(i == 1)
        {
            parityData++;
        }
    }

    if(PARITY_BIT == -1)
    {
        return;
    }

    CheckParity(parityData);
}

void CheckParity(int parityData)
{
    correctParity = (parityData % 2 == PARITY_BIT);

    if(!correctParity)
    {
        Reset();
        currentState = IDLE;
        return;
    }
    else
    {
        currentState = ADD_TO_BUFFER;
    }
}

void StartBitFound()
{
    startTime = micros();
    Bits[0] = 0;
    bitCount++;
    currentState = READ_BIT;
}

/// ================= STATE MACHINE ================= ///

// Need functions for all states
void Handle_Idle(Event ev)
{
    if(CheckForStartBit())
    {

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
    Serial.print("Byte received: ");
    Serial.println((char)exportByte);
}

void Handle_ReadBit(Event ev)
{
    if(bitCount == MaximumBits)
    {
        TIMSK1 &= ~(0 << OCIE1A); // Disable the timer while we validate the byte
        currentState = BYTE_VALIDATION;
        return;
    }
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
