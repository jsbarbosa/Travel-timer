#include "utils.h"
#include "keypad.h"

const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};


const uint8_t KEYPAD_RS[KEYPAD_ROWS] = {5, 6, A5, A4};
const uint8_t KEYPAD_CS[KEYPAD_COLS] = {A3, A2, A1, A0};

void setup_keypad(void)
{
  for(int i=0; i < KEYPAD_ROWS; i++){
    pinMode(KEYPAD_RS[i], OUTPUT);
  }

  for(int i=0; i < KEYPAD_COLS; i++){
    pinMode(KEYPAD_CS[i], INPUT_PULLUP);
  }
}


char get_key(void)
{
  char key = 0;
  // set to high
  for(uint8_t i=0; i<KEYPAD_ROWS; i++)
  {
    write_pin(KEYPAD_RS[i], HIGH);
  }
  
  for(uint8_t i=0; (i<KEYPAD_ROWS) & (key == 0); i++)
  {
    // activate row by droping it
    write_pin(KEYPAD_RS[i], LOW);
    for(uint8_t j=0; (j<KEYPAD_COLS) & (key == 0); j++)
    {
      if(!digitalRead(KEYPAD_CS[j])) key = KEYPAD_KEYS[i][j];
    }
    write_pin(KEYPAD_RS[i], HIGH);
  }

  // set to low
  for(uint8_t i=0; i<KEYPAD_ROWS; i++)
  {
    write_pin(KEYPAD_RS[i], LOW);
  }

  return key;
}

