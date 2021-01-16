#include "lcd.h"
#include "menu.h"

volatile uint8_t CURSOR_COLUMN = 0;
volatile char CURRENT_VIEW = 0;

void update(char key)
{
  switch(key)
  {
    case CURRENT_TIME_KEY:
      LCD.clear();
      CURSOR_COLUMN = 0;
      CURRENT_VIEW = CURRENT_TIME_KEY;
      LCD.print("CURRENT_TIME_KEY");
      break;
    case TRAVEL_TIME_KEY:
      LCD.clear();
      CURSOR_COLUMN = 0;
      CURRENT_VIEW = TRAVEL_TIME_KEY;
      LCD.print("TRAVEL_TIME_KEY");
      break;
    case BACK_KEY:
      LCD.clear();
      CURSOR_COLUMN = 0;
      CURRENT_VIEW = 0;
      LCD.print("BACK_KEY");
      break;
    case ENTER_KEY:
      LCD.clear();
      CURSOR_COLUMN = 0;
      CURRENT_VIEW = 0;
      LCD.print("ENTER_KEY");
      break;
    case LEFT_KEY:
      if (CURSOR_COLUMN > 0) CURSOR_COLUMN -= 1;
      LCD.setCursor(0, 0);
      LCD.print("LEFT_KEY");
      break;
    case RIGHT_KEY:
      if (CURSOR_COLUMN < LCD_COLS-1) CURSOR_COLUMN += 1;
      LCD.setCursor(0, 0);
      LCD.print("RIGHT_KEY");
      break;
    default:
      LCD.setCursor(CURSOR_COLUMN, 1);
      CURSOR_COLUMN += 1;
      LCD.print(key);
    LCD.blink();
  }
}
