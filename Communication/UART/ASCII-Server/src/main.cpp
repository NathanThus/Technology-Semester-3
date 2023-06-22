#include <Arduino.h>

#define MAX_DIGITAL_PINS 13
#define MIN_DIGITAL_PINS 8
#define MAX_ANALOG_PINS 5

// #define UART

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting transmitter");
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
    #ifdef UART

    #endif
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
    #ifdef UART
    #endif
    #else
      Serial.print("A");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(analogRead(i));
    #endif
  }
}

void ClearScreen()
{
  Serial.println("Clearing");
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
} 

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    Menu((MenuChoices)Serial.read());
  }
}
