#include <Arduino.h>
#include <SFE_MicroOLED.h>
#include <Wire.h>

#define I2C_ADDRESS 0x3
#define OLED_RESET 9
#define DC_JUMPER 1

#define FONT_SIZE 0

MicroOLED oled(OLED_RESET, DC_JUMPER);

enum I2CData
{
  None = -1,
  Temperature = 0,
  Humidity = 1
};

// Registers

I2CData incomingData = None;
int Register_Temperature = 0;
int Register_Humidity = 0;
int lastBytes = 0;

void recieveData(int bytes)
{
  int data = Wire.read();
  Register_Temperature = data;
}

void setup() {
  // put your setup code here, to run once:
  Wire.onReceive(recieveData);
  Wire.begin(I2C_ADDRESS);

  oled.begin();
  oled.setFontType(1);
  oled.clear(ALL);
  oled.display();
  delay(500);
  oled.clear(PAGE);

  Serial.begin(9600);
}

int cursorY = 0;

void PrintLine(String title, int data)
{
  oled.print(title);
  oled.print(data);
  oled.display();
  cursorY += 16;
  oled.setCursor(0,cursorY);
}

void ResetCursor()
{
  oled.setCursor(0,0);
  cursorY = 0;
}

void loop()
{
  PrintLine("T: ", Register_Temperature);
  PrintLine("H: ", Register_Humidity);
  ResetCursor();
  oled.clear(PAGE);

  delay(1000);
}

