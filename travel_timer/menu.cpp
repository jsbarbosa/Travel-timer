#include "lcd.h"
#include "menu.h"
#include "timing.h"
//#include <DateTime.h>

volatile uint8_t CURSOR_COLUMN = 0;
volatile char CURRENT_VIEW = 0;

void main_view(void)
{
  if (CURRENT_VIEW != 0)
  {
    LCD.clear();
    CURRENT_VIEW = 0;
  }
  
  CURSOR_COLUMN = 0;
  LCD.setCursor(0, 0);  // second column, first row
  LCD.print("C ");
  LCD.print(get_current_time());
  LCD.setCursor(0, 1);  // first column, second row
  LCD.print("T ");
  LCD.print(get_travel_date());
  //LCD.setCursor(10, 1);
  LCD.print(" D:");
  LCD.print(days_between());
}

void move_cursor_left(void)
{
  // limit on the left
  if(CURSOR_COLUMN > 0)
  {
    CURSOR_COLUMN -= 1; 
  }

  // jump separators
  if(CURSOR_COLUMN % 3 == 2)
  {
    CURSOR_COLUMN -= 1;
  }

  LCD.setCursor(CURSOR_COLUMN, 1);
}

void move_cursor_right(void)
{
  // limit on the right (13 characters in datetime)
  if(CURSOR_COLUMN < 13)
  {
    CURSOR_COLUMN += 1; 
  }
  
  // jump separators
  if(CURSOR_COLUMN % 3 == 2)
  {
    CURSOR_COLUMN += 1;
  }

  LCD.setCursor(CURSOR_COLUMN, 1);
}

void current_time_view(char key)
{
  if(key == 0) return;
  if(CURRENT_VIEW != CURRENT_TIME_KEY)
  {
    LCD.clear();
    LCD.print("Setup C. Time");
    LCD.setCursor(0, 1);
    LCD.print(get_current_time());
    LCD.setCursor(0, 1);
    LCD.blink();
    
    CURRENT_VIEW = CURRENT_TIME_KEY;
  }

  switch(key)
  {
    case BACK_KEY:
      main_view();
      return ;
    case ENTER_KEY:
      commit_current_time();
      main_view();
      return ;
    case LEFT_KEY:
      move_cursor_left();
      return ;
    case RIGHT_KEY:
      move_cursor_right();
      return ;
    case CURRENT_TIME_KEY:
    case TRAVEL_TIME_KEY:
      return ;
  }

  if(set_date_char(CURSOR_COLUMN, key, CURRENT_TIME_INDICATOR))
  {
    LCD.setCursor(CURSOR_COLUMN, 1);
    LCD.print(key);
    if(CURSOR_COLUMN == 13)
    {
      commit_current_time();
      main_view();
    }
    else{
      move_cursor_right();
    }
  }
}

void travel_time_view(char key)
{
  if(key == 0) return;
  if(CURRENT_VIEW != TRAVEL_TIME_KEY)
  {
    LCD.clear();
    LCD.print("Setup T. Time");
    LCD.setCursor(0, 1);
    LCD.print(get_travel_time());
    LCD.setCursor(0, 1);
    LCD.blink();
    
    CURRENT_VIEW = TRAVEL_TIME_KEY;
  }

  switch(key)
  {
    case BACK_KEY:
      main_view();
      return ;
    case ENTER_KEY:
      commit_travel_time();
      main_view();
      return ;
    case LEFT_KEY:
      move_cursor_left();
      return ;
    case RIGHT_KEY:
      move_cursor_right();
      return ;
    case CURRENT_TIME_KEY:
    case TRAVEL_TIME_KEY:
      return ;
  }

  if(set_date_char(CURSOR_COLUMN, key, TRAVEL_TIME_INDICATOR))
  {
    LCD.setCursor(CURSOR_COLUMN, 1);
    LCD.print(key);
    if(CURSOR_COLUMN == 13)
    {
      commit_travel_time();
      main_view();
    }
    else{
      move_cursor_right();
    }
  }
}

void menu_update(char key)
{
  switch(CURRENT_VIEW)
  {
    case CURRENT_TIME_KEY:
      return current_time_view(key);
    case TRAVEL_TIME_KEY:
      return travel_time_view(key);
  }

  switch(key)
  {
    case CURRENT_TIME_KEY:
      return current_time_view(key);
    case TRAVEL_TIME_KEY:
      return travel_time_view(key);
  }

  main_view();
}
