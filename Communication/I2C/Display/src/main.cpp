#include <Arduino.h>
#include <SFE_MicroOLED.h>

#define I2C_ADDRESS 0x69
#define OLED_RESET 9
#define DC_JUMPER 1

#define FONT_SIZE 0

MicroOLED oled(OLED_RESET, DC_JUMPER);

void setup() {
  // put your setup code here, to run once:
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

int a = 0;
int b = 1;

void loop()
{
  PrintLine("a = ", a);
  PrintLine("b = ", b);
  a += 2;
  b += 2;
  ResetCursor();
  oled.clear(PAGE);
  delay(1000);
}