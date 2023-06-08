#ifndef UART_RECIEVER_HPP
#define UART_RECIEVER_HPP

#include <Arduino.h>

constexpr int PossibleTotalBits = 12;

enum State
{
    Idle = 0,
    Report_Data = 1,
    Read_UART = 2,
    Read_Bit = 3,
    Byte_Validation = 4,
    Add_To_Buffer = 5
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
    Parity ParitySettings;
    int ParityBit;
    int BaudRate;
};

class UART_Receiver
{
    private:
    UART_Configuration uartConfiguration;
    int AvailableByte; // The value of the byte that is available to be read.

    // ======== INCOMING BYTE ======== //
    int currentBitIndex = 1;
    int currentByte[PossibleTotalBits];

    bool hasAllData = false;
    // ============ TIMING ============= //
    int TimePerBit = 0;

    int ConvertByteArrayToByte();

    public:

        UART_Receiver(UART_Configuration configuration);
        
        int GetTimePerBit() { return TimePerBit; }
        /// @brief Checks if a start bit is available.
        /// @return True if a start bit is available.
        bool CheckForStartBit();
        /// @brief Reads the next bit in the byte.
        void ReadBit();
        /// @brief Checks if the stop bits are correct.
        /// @return True if the stop bits are correct.
        bool RecievedStopBits();
        /// @brief Checks the parity of the current byte.
        /// @return True if the parity is correct.
        bool CheckParity();
        /// @brief Adds the current byte to the buffer.
        void AddByteToBuffer();
        /// @brief Returns the next byte in the buffer.
        /// @param data Where the data is stored.
        void GetDataFromBuffer(int* data);
        /// @brief Resets all data required for a new byte.
        void ResetCurrentByte();

};

#endif // UART_RECIEVER_HPP