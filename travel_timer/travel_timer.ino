#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <LiquidCrystal.h>

// PIN 2 is the interrupt pin
#define POWER_OUTPUT_PIN 13
#define TIMER_TIME_SECONDS 1
#define ACTIVE_TIME_SECONDS 10 // must be a multiple of TIMER_TIME_SECONDS

#define LCD_RS 7
#define LCD_EN 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12


volatile byte BUTTON_PRESSED = LOW;

// timer
const int ACTIVE_TIME_ITERATIONS = ACTIVE_TIME_SECONDS / TIMER_TIME_SECONDS;
volatile int ACTIVE_TIME_ITERATOR = 0;

// LCD
LiquidCrystal LCD(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// utils
void write_pin(int pin, byte value)
{
  volatile uint8_t *reg = portOutputRegister(digitalPinToPort(pin));
  uint8_t mask = digitalPinToBitMask(pin);
  if (value) *reg |= mask;
  else *reg &= ~mask;
}

// timer
void enable_timer(void)
{
  ACTIVE_TIME_ITERATOR = 0;
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; //initialize counter value to 0
  
  OCR1A = int((16000000.0 / 1024.0) * TIMER_TIME_SECONDS) - 1;
  
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
}

void disable_timer(void){
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0; //initialize counter value to 0
  TIMSK1 &= ~(1 << OCIE1A); // disable timer compare interrupt
}

ISR (TIMER1_COMPA_vect)
{
  if(ACTIVE_TIME_ITERATOR == ACTIVE_TIME_ITERATIONS)
  {
    disable_timer();
    power_down();
  }
  ACTIVE_TIME_ITERATOR = (ACTIVE_TIME_ITERATOR + 1) % (ACTIVE_TIME_ITERATIONS + 1);
}


// sleep
void power_down(void)
{
    ADCSRA = 0;                    //disable the ADC
    
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
    
    //disable brown-out detection while sleeping (20-25µA)
    sleep_bod_disable();
    sei();                         //ensure interrupts enabled so we can wake up again
    sleep_cpu();                   //go to sleep
    sleep_disable();               //wake up here
}

void setup() {
  pinMode(POWER_OUTPUT_PIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  write_pin(POWER_OUTPUT_PIN, HIGH);

  LCD.begin(16, 2);  
  LCD.print("hello, world!");

  sei();
}

void loop() {
  write_pin(POWER_OUTPUT_PIN, HIGH);
  for(int i = 0; i < 10; i++)
  {
    LCD.setCursor(2, 1);
    LCD.print(millis() / 1000);
    delay(1000);
  }
  enable_timer();
}
