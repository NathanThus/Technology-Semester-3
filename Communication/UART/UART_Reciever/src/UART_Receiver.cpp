#include "UART_Receiver.hpp"

#define READ_RX_PIN (PIND & B00000001)

const long MicrosecondsPerSecond = 1000000;

// START | B0 | B1 | B2 | B3 | B4 | B5 | B6 | B7 | PARITY | STOP | STOP
// 0     | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  |   9    | 10   | 11

const int StartBitIndex = 0;
const int DataBitIndex = 1;

int TotalBits = 0;

UART_Receiver::UART_Receiver(UART_Configuration configuration) : uartConfiguration(configuration)
{
    TotalBits = uartConfiguration.DataBits + uartConfiguration.StopBits + uartConfiguration.ParityBit;
    TimePerBit = MicrosecondsPerSecond / uartConfiguration.BaudRate;
}

bool UART_Receiver::CheckForStartBit()
{
    if(READ_RX_PIN == 0)
    {
        currentByte[StartBitIndex] = 0;
        return true;
    }
    return false;
}

void UART_Receiver::ReadBit()
{
    currentByte[currentBitIndex] = READ_RX_PIN;
    currentBitIndex++;
    hasAllData = (currentBitIndex == TotalBits);
}

bool UART_Receiver::RecievedStopBits()
{
    if(currentBitIndex != TotalBits)
    {
        return false;
    }
    // TODO: Cleanup
    switch (uartConfiguration.StopBits)
    {
    case 1:
        if(currentByte[TotalBits - 1] == 1)
        {
            return true;
        }
        break;

    case 2:
        if(currentByte[TotalBits - 1] == 1 && currentByte[TotalBits - 2] == 1)
        {
            return true;
        }
        break;
    default:
    return false;
    }
    return false;
}

bool UART_Receiver::CheckParity()
{
    int byteParity = 0;
    for(int i = 0; i < uartConfiguration.DataBits; i++)
    {
        byteParity += currentByte[i];
    }

    switch (uartConfiguration.ParitySettings)
    {
    case Parity::None:
        return true;
        break;
    case Parity::Odd:
        return (byteParity % 2 == 1);
        break;
    case Parity::Even:
        return (byteParity % 2 == 0);
        break;
    default:
    return true;
        break;
    }
}

int UART_Receiver::ConvertByteArrayToByte()
{
    int result = 0;
    for (int i = DataBitIndex; i <= uartConfiguration.DataBits; i++)
    {
        result |= currentByte[i] << (i - DataBitIndex);
    }
    return result;
}

void UART_Receiver::AddByteToBuffer()
{
    AvailableBytes[0] = ConvertByteArrayToByte();
    ResetCurrentByte();
}


void UART_Receiver::GetDataFromBuffer(int& data)
{
    data = AvailableBytes[0];
    currentBufferIndex++;

    if(currentBufferIndex == bufferSize)
    {
        currentBufferIndex = 0;
    }
}



void UART_Receiver::ResetCurrentByte()
{
    for(int i = 0; i < TotalBits; i++)
    {
        currentByte[i] = 0;
    }
    currentBitIndex = 1;
    hasAllData = false;
}