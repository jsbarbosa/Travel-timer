#include "utils.h"
#include "keypad.h"

const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','4','7', '*'},
  {'2','5','8', '0'},
  {'3','6','9', '#'},
  {'A','B','C', 'D'}
};


const uint8_t KEYPAD_RS[KEYPAD_ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4};
const uint8_t KEYPAD_CS[KEYPAD_COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

void setup_keypad(void)
{
  for(int i=0; i < KEYPAD_ROWS; i++){
    pinMode(KEYPAD_RS[i], OUTPUT);
  }

  for(int i=0; i < KEYPAD_COLS; i++){
    pinMode(KEYPAD_CS[i], INPUT);
  }
}


char get_key(void)
{
  char key = 0;
  
  for(uint8_t i=0; (i<KEYPAD_ROWS) & (key == 0); i++)
  {
    // activate row
    write_pin(KEYPAD_RS[i], HIGH);
    for(uint8_t j=0; (j<KEYPAD_COLS) & (key == 0); j++)
    {
      if(digitalRead(KEYPAD_CS[j]))
      {
        key = KEYPAD_KEYS[i][j];
      }
    }
    write_pin(KEYPAD_RS[i], LOW);
  }
  
  return key;
}
