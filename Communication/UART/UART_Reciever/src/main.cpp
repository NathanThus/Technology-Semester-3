#include <Arduino.h>
#include "UART_Receiver.hpp"

const int dataBits = 8;
const int stopBits = 1;
const int baudrate = 1;

// ============ //
// === TIME === //
// ============ //

unsigned long currentTime = 0;
unsigned long startTime = 0;
long bitTime = 0;

UART_Receiver *uart;

State currentState = Idle;
Events currentEvent = NONE;

void setup() {
  UART_Configuration uartConfig = {dataBits, stopBits, baudrate};
  uart = new UART_Receiver(uartConfig);

  Serial.begin(baudrate); // THIS WILL NEED TO CHANGE.
}

void HandleIdleState(Events ev)
{
    if(ev == START_BIT_RECIEVED)
    {
      currentState = Read_UART;
      return;
    }

    if(uart->CheckForStartBit()) // TODO: Move this
    {
      currentEvent = START_BIT_RECIEVED;
      startTime = micros() + (0.5 * uart->GetTimePerBit()); // TODO: Potential slowdown
      return;
    }
}

void HandleAddToBuffer(Events ev)
{
    if(ev == BYTE_ADDED_TO_BUFFER)
    {
      currentState = Idle;
      return;
    }
    else if(ev == BYTE_WAS_GARBAGE)
    {
      currentState = Idle;
      return;
    }
}

void HandleReadBitState(Events ev)
{
  if(micros() >= startTime + uart->GetTimePerBit())
  {
    startTime = micros(); // Have correct timing
    if(uart->ReadBit())
    {
      currentEvent = BYTE_RECEIVED;
      return;
    }
  }
    if(ev == BYTE_ADDED_TO_BUFFER)
    {
      currentState = Byte_Validation;
      return;
    }
    else if(ev == BYTE_WAS_GARBAGE)
    {
      currentState = Idle;
      return;
    }
}

void HandleByteValidation(Events ev)
{
    if(ev == BYTE_VALIDATION_PASSED)
    {
      currentState = Report_Data;
      return;
    }
    else if(ev == BYTE_VALIDATION_FAILED)
    {
      currentState = Idle;
      return;
    }
}

void HandleReportData(Events ev)
{
  if(ev == BYTE_REPORTED)
  {
    currentState = Idle;
    return;
  }
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
  // if(uart->ReadBit() && ev == NONE)
  // {
  //   currentEvent = BYTE_RECEIVED;
  //   return;
  // }

  // if(ev == BYTE_RECEIVED)
  // {
  //   currentState = Read_Bit;
  //   return;
  // }
}

void HandleEvent(Events ev)
{
  switch (currentState)
  {
  case Idle:
  HandleIdleState(ev);
    break;
  case Read_UART:
  HandleReadUARTState(ev);
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

