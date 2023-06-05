#include "UART_Receiver.hpp"

const int RX_PIN = 0;

UART_Receiver::UART_Receiver(UART_Configuration configuration) : _configuration(configuration)
{
    TotalBits = _configuration.DataBits + _configuration.StopBits + ParityBits;
    TimePerBit = 1000000/_configuration.BaudRate; //TODO: Remove magic number
}

// TODO: Implement this.
bool UART_Receiver::GetDataFromBuffer(byte *data)
{
    if(data == NULL)
    {
        return false;
    }

    return false; // NOT IMPLEMENTED

    *data = buffer[currentBufferIndex];
    currentBufferIndex++;

    if(currentBufferIndex >= bufferSize)
    {
        currentBufferIndex = 0;
    }

    return true;
}

bool UART_Receiver::CheckForStartBit()
{
    if(digitalRead(0) == HIGH)
    {
        currentByte[0] = 1;
        Serial.print((int)currentByte[0]);
        currentByteIndex++;
        ResetCurrentByte();
        return true;
    }
    return false;
}

bool UART_Receiver::ReadBit()
{
    // Wrap this in a while loop?
    currentByte[currentByteIndex] = !digitalRead(RX_PIN); // Might need to be a register call.
    digitalWrite(13,currentByte[currentByteIndex]);
    currentByteIndex++;
    return currentByteIndex == TotalBits;
}

void UART_Receiver::ResetCurrentByte()
{
    currentBufferIndex = 0;
    currentByteIndex = 0;

    for (int i = 0; i < TotalBits; i++)
    {
        currentByte[i] = 0;
    }

    hasStartBit = false;

    for (int i = 0; i < StopBitCount; i++)
    {
        StopBits[i] = 0;
    }
        
    parity = None;
}

bool UART_Receiver::CheckParity()
{
    return false; // TODO: Implement this.
}
