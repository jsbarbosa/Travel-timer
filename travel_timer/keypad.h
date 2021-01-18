#include "Arduino.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define KEYPAD_R1 6
#define KEYPAD_R2 5
#define KEYPAD_R3 A4
#define KEYPAD_R4 A5
#define KEYPAD_C1 A0
#define KEYPAD_C2 A1
#define KEYPAD_C3 A2
#define KEYPAD_C4 A3
#define DEBOUNCE_MS 350 

extern const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS];

extern const uint8_t KEYPAD_RS[KEYPAD_ROWS];
extern const uint8_t KEYPAD_CS[KEYPAD_COLS];

void setup_keypad(void);
char get_key(void);
