#include <LiquidCrystal.h>
#include "lcd.h"

LiquidCrystal LCD(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup_display(void)
{
  LCD.begin(16, 2);  
  LCD.print("hello, world!");
}

