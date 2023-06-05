#ifndef UART_RECIEVER_HPP
#define UART_RECIEVER_HPP

#include <Arduino.h>

const int bufferSize = 8;
const int StartBit = 1;
const int StopBitCount = 2;
const int ParityBits = 1;

enum State
{
    Idle,
    Read_UART,
    Read_Bit,
    Byte_Validation,
    Add_To_Buffer,
    Report_Data
};

enum Events
{
    NONE,
    START_BIT_RECIEVED,
    STOP_BIT_RECIEVED,
    BYTE_VALIDATION_PASSED,
    BYTE_VALIDATION_FAILED,
    BYTE_WAS_GARBAGE,
    BYTE_ADDED_TO_BUFFER,
    BYTE_RECEIVED,
    BYTE_REPORTED
};

enum Parity
{
    None = 0,
    Odd = 1,
    Even = 2
};

struct UART_Configuration
{
    int DataBits;
    int StopBits;
    int BaudRate;
};



class UART_Receiver
{
    private:
    UART_Configuration _configuration;

    byte buffer[bufferSize]; // The buffer that holds the completed values. TODO: Might need a rework.
    int currentBufferIndex = 0;

    // ======== INCOMING BYTE ======== //
    bool hasStartBit = false;
    int currentByte[bufferSize];
    Parity parity = None;
    int StopBits[StopBitCount];
    int currentByteIndex = 0;

    int TotalBits = 0;

    // ============ TIMING ============= //
    int TimePerBit = 0;

    public:

        UART_Receiver(UART_Configuration configuration);
        
        bool GetDataFromBuffer(byte* data);
        int GetTimePerBit() { return TimePerBit; }

        bool CheckForStartBit();
        bool ReadBit();
        void ResetCurrentByte();

        // ========================= //
        // ==== Byte Validation ==== //
        // ========================= //

        bool CheckParity();

};

#endif // UART_RECIEVER_HPP