#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN 13
#define INTERRUPT_PIN 2
#define TIMER_TIME_SECONDS 1
#define ACTIVE_TIME_SECONDS 1 // must be a multiple of TIMER_TIME_SECONDS

volatile byte BUTTON_PRESSED = LOW;

// timer
const int ACTIVE_TIME_ITERATIONS = ACTIVE_TIME_SECONDS / TIMER_TIME_SECONDS;
volatile int ACTIVE_TIME_ITERATOR = 0;

// utils
void write_pin(int pin, byte value)
{
  uint8_t *reg = portOutputRegister(digitalPinToPort(pin));
  uint8_t mask = digitalPinToBitMask(pin);
  if (value) *reg |= mask;
  else *reg &= ~mask;
}

// timer
void setup_timer(void)
{
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; //initialize counter value to 0
    
    OCR1A = int((16000000.0 / 1024.0) * TIMER_TIME_SECONDS) - 1;
    
    TCCR1B |= (1 << WGM12); // turn on CTC mode
    
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
}

ISR (TIMER1_COMPA_vect)
{
  if (ACTIVE_TIME_ITERATOR == 0)
  {
    write_pin(LED_PIN, BUTTON_PRESSED);
    BUTTON_PRESSED = !BUTTON_PRESSED;
  }
  ACTIVE_TIME_ITERATOR = (ACTIVE_TIME_ITERATOR + 1) % ACTIVE_TIME_ITERATIONS;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), blink, LOW);

  setup_timer();
  
  sei();
}

void loop() {
  
}

void blink() {
  //if(!BUTTON_PRESSED){
    //BUTTON_PRESSED = HIGH;
  //}
}
