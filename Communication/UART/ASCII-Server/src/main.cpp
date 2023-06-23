#include <Arduino.h>

#define MAX_DIGITAL_PINS 13
#define MIN_DIGITAL_PINS 8
#define MAX_ANALOG_PINS 5
#define BAUDRATE 9600

#define UART_

#ifdef UART_
#define INPUT_PIN 2
#define OUTPUT_PIN 3

#include "UART.hpp"
UART uart(BAUDRATE,INPUT_PIN,OUTPUT_PIN);
#endif

void PrintOptions();

void setup() {
  Serial.begin(BAUDRATE);
  // put your setup code here, to run once:
  #ifdef UART_
  PrintOptions();
  #endif
}

enum MenuChoices
{
  Print_Digital = 'd',
  Print_Analog = 'a',
  Clear = 'c'
};

void PrintDigital()
{
  for(int i = MIN_DIGITAL_PINS; i < MAX_DIGITAL_PINS; i++)
  {
    #ifdef UART_
      String data = "DI";
      data += String(i) + ": " + String(digitalRead(i));
      data += "\n\r";
      uart.SendLine(data);
    #else
      Serial.print("DI");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(digitalRead(i));
    #endif
  }
}

void PrintAnalog()
{
  for(int i = 0; i < MAX_ANALOG_PINS; i++)
  {
    #ifdef UART_
      String data = "A";
      data += String(i) + ": " + String(analogRead(A0 + i));
      data += "\n\r";
      uart.SendLine(data);
    #else
      Serial.print("A");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(analogRead(A0 + i));
    #endif
  }
}

void ClearScreen()
{
  #ifdef UART_
    uart.SendLine("\e[2J\e[H");
  #else
    Serial.print("\e[2J\e[H");
  #endif
}

void PrintOptions()
{
  #ifdef UART_
    uart.SendLine("Options:\n\r");
    uart.SendLine("d: Print Digital\n\r");
    uart.SendLine("a: Print Analog\n\r");
    uart.SendLine("c: Clear Screen\n\r");
  #else
    Serial.println("Options:");
    Serial.println("d: Print Digital");
    Serial.println("a: Print Analog");
    Serial.println("c: Clear Screen");
  #endif
}

void Menu(MenuChoices input)
{
  switch (input)
  {
  case Print_Digital:
    PrintDigital();
    break;
  case Print_Analog:
    PrintAnalog();
    break;
  case Clear:
    ClearScreen();
    break;
  default:
    break;
  }
  PrintOptions();
} 

#ifdef UART_
char data;
#endif

void loop() {
  // put your main code here, to run repeatedly:
  #ifdef UART_
  if(uart.Recieve(data))
  {
    Menu((MenuChoices)data + 1);
  }
  #else
  if(Serial.available())
  {
    Menu((MenuChoices)Serial.read());
  }
  #endif
}
