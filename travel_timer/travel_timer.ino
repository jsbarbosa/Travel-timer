#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <LiquidCrystal.h>


// PIN 2 is the interrupt pin
#define POWER_OUTPUT_PIN 13
#define TIMER_TIME_SECONDS 1
#define ACTIVE_TIME_SECONDS 5 // must be a multiple of TIMER_TIME_SECONDS

#define LCD_RS 7
#define LCD_EN 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define KEYPAD_R1 5
#define KEYPAD_R2 6
#define KEYPAD_R3 A5
#define KEYPAD_R4 A4
#define KEYPAD_C1 A3
#define KEYPAD_C2 A2
#define KEYPAD_C3 A1
#define KEYPAD_C4 A0

// timer
const int ACTIVE_TIME_ITERATIONS = ACTIVE_TIME_SECONDS / TIMER_TIME_SECONDS;
volatile int ACTIVE_TIME_ITERATOR = 0;
volatile byte TIMER_ACTIVE = 0;

// LCD
LiquidCrystal LCD(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// KEYPAD
const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

const uint8_t KEYPAD_RS[KEYPAD_ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4};
const uint8_t KEYPAD_CS[KEYPAD_COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

// utils
void write_pin(int pin, byte value)
{
  volatile uint8_t *reg = portOutputRegister(digitalPinToPort(pin));
  uint8_t mask = digitalPinToBitMask(pin);
  if (value) *reg |= mask;
  else *reg &= ~mask;
}

// timer
void enable_timer()
{
  if (TIMER_ACTIVE == 0)
  {
    ACTIVE_TIME_ITERATOR = 0;
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; //initialize counter value to 0
    
    OCR1A = int((16000000.0 / 1024.0) * TIMER_TIME_SECONDS) - 1;
    
    TCCR1B |= (1 << WGM12); // turn on CTC mode
    
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    TIMER_ACTIVE = 1;
  }
}

void disable_timer(void){
  if(TIMER_ACTIVE)
  {
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0; //initialize counter value to 0
    TIMSK1 &= ~(1 << OCIE1A); // disable timer compare interrupt
    TIMER_ACTIVE = 0;
    ACTIVE_TIME_ITERATOR = 0;
  }
}

ISR (TIMER1_COMPA_vect)
{
  if((ACTIVE_TIME_ITERATOR == ACTIVE_TIME_ITERATIONS) & TIMER_ACTIVE)
  {
    disable_timer();
    power_down();
  }
  ACTIVE_TIME_ITERATOR = (ACTIVE_TIME_ITERATOR + 1) % (ACTIVE_TIME_ITERATIONS + 1);
}

// sleep
void power_down(void)
{
  write_pin(POWER_OUTPUT_PIN, LOW);  // disable power pin
  write_pin(LCD_RS, LOW);  // disable power pin
  write_pin(LCD_EN, LOW);  // disable power pin
  write_pin(LCD_D4, LOW);  // disable power pin
  write_pin(LCD_D5, LOW);  // disable power pin
  write_pin(LCD_D6, LOW);  // disable power pin
  write_pin(LCD_D7, LOW);  // disable power pin
  
  EICRA = _BV(ISC01);            //configure INT0 to trigger on falling edge
  EIMSK = _BV(INT0);             //enable INT0
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();                         //stop interrupts to ensure the BOD timed sequence executes as required
  
  sleep_enable();
  sleep_bod_disable();  //disable brown-out detection while sleeping (20-25µA)
  sei();  //ensure interrupts enabled so we can wake up again
  sleep_cpu();  //go to sleep
  sleep_disable();  //wake up here
  enable_timer();
  
  write_pin(POWER_OUTPUT_PIN, HIGH);
}

// keypad
void setup_keypad(void)
{
  for(int i=0; i < KEYPAD_ROWS; i++){
    pinMode(KEYPAD_RS[i], OUTPUT);
  }

  for(int i=0; i < KEYPAD_COLS; i++){
    pinMode(KEYPAD_CS[i], INPUT_PULLUP);
  }
}

void setup_display(void)
{
  LCD.begin(16, 2);  
  LCD.print("hello, world!");
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


void setup()
{
  ADCSRA = 0; //disable the ADC
  pinMode(2, INPUT_PULLUP); // interrupt pin

  write_pin(POWER_OUTPUT_PIN, HIGH); // power on

  setup_display();
  setup_keypad();

  sei();
}

void loop()
{
  LCD.setCursor(5, 1);
  LCD.print(millis() / 1000);

  char key = get_key();
  if(key)
  {
    LCD.setCursor(1, 1);
    LCD.print(key);
    disable_timer();
  }
  else{
    LCD.setCursor(1, 1);
    LCD.print(' ');
    enable_timer();
  }
}
