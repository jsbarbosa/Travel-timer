#include "sleep.h"
#include "utils.h"
#include "lcd.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


const int ACTIVE_TIME_ITERATIONS = ACTIVE_TIME_SECONDS / TIMER_TIME_SECONDS;
volatile int ACTIVE_TIME_ITERATOR = 0;
volatile uint8_t TIMER_ACTIVE = 0;

void enable_timer()
{
  if (TIMER_ACTIVE == 0)
  {
    ACTIVE_TIME_ITERATOR = 0;
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; //initialize counter value to 0
    
    OCR1A = ((16000000.0 / 1024.0) * TIMER_TIME_SECONDS) - 1;
    
    TCCR1B |= (1 << WGM12); // turn on CTC mode
    
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    TIMER_ACTIVE = 1;
  }
}

void disable_timer(void)
{
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

void power_down(void)
{
  write_pin(POWER_OUTPUT_PIN, 0);  // disable power pin
  write_pin(LCD_RS, 0);  // disable power pin
  write_pin(LCD_EN, 0);  // disable power pin
  write_pin(LCD_D4, 0);  // disable power pin
  write_pin(LCD_D5, 0);  // disable power pin
  write_pin(LCD_D6, 0);  // disable power pin
  write_pin(LCD_D7, 0);  // disable power pin
  
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
  
  write_pin(POWER_OUTPUT_PIN, 1);
}
