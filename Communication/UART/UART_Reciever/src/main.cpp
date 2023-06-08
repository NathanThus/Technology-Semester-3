#include <Arduino.h>
#include "UART_Receiver.hpp"

// ============= //
// === DEBUG === //
// ============= //

#define TOGGLE_LED digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN))

// ============ //
// === DATA === //
// ============ //

const int dataBits = 8;
const int stopBits = 1;
const int baudrate = 300; // 300 is the target for 7/6/23

// ============ //
// === TIME === //
// ============ //

unsigned long startTime = 0;
long bitTime = 0;

UART_Receiver *uart;

State currentState = Idle;
Events currentEvent = NONE;

void setup() {
  UART_Configuration uartConfig = {dataBits, stopBits, None, 0, baudrate};
  uart = new UART_Receiver(uartConfig);

  Serial.begin(baudrate);
}

void HandleIdleState(Events ev)
{
    if(uart->CheckForStartBit())
    {
      startTime = micros() + (0.5 * uart->GetTimePerBit()); // TODO: Potential slowdown
      currentEvent = START_BIT_RECIEVED; // TODO: ???
      currentState = Read_Bit;
      return;
    }
}

void HandleReadBitState(Events ev)
{
  if(micros() >= startTime + uart->GetTimePerBit())
  {
    uart->ReadBit();
    startTime = micros();
    return;
  }

  if(uart->RecievedStopBits())
  {
    currentEvent = STOP_BIT_RECIEVED;
    return;
  }

  if(ev == STOP_BIT_RECIEVED)
  {
    currentState = Byte_Validation;
    return;
  }

}

void HandleAddToBuffer(Events ev)
{
    if(ev == BYTE_ADDED_TO_BUFFER)
    {
      uart->AddByteToBuffer();
      ev = BYTE_RECEIVED;
      currentState = Idle;
      return;
    }
    else if(ev == BYTE_WAS_GARBAGE)
    {
      uart->ResetCurrentByte();
      currentState = Idle;
      return;
    }
}

void HandleByteValidation(Events ev) //TODO: Needs some SERIOUS cleanup.
{
    if(uart->CheckParity())
    {
      ev = BYTE_VALIDATION_PASSED;
      currentState = Report_Data;
    }
    else
    {
      ev = BYTE_VALIDATION_FAILED;
    }

    if(ev == BYTE_VALIDATION_FAILED)
    {
      uart->ResetCurrentByte();
      currentState = Idle;
      return;
    }
}

void HandleReportData(Events ev)
{
  uart->AddByteToBuffer();
  int data;
  
  uart->GetDataFromBuffer(&data);
  if(data == 0)
  {
    Serial.write("NULL"); // This will eventually handle /r/n
  }
  else
  {
    Serial.write(data);
  }

  currentState = Idle;
  currentEvent = BYTE_REPORTED;
}

void HandleReadUARTState(Events ev)
{
  switch (currentState)
  {
    case Read_Bit:
    HandleReadBitState(ev);
    break;
    case Byte_Validation:
    HandleByteValidation(ev);
    break;
    case Add_To_Buffer:
    HandleAddToBuffer(ev);
    break;
  default:
    break;
  }
}

void HandleEvent(Events ev)
{
  if (currentState >= Read_UART)
  {
    HandleReadUARTState(ev);
    return;
  }

  switch (currentState)
  {
  case Idle:
    HandleIdleState(ev);
    break;
  case Report_Data:
    HandleReportData(ev);
    break;
  default:
    break;
  }
}

void loop()
{
  HandleEvent(currentEvent);
}
