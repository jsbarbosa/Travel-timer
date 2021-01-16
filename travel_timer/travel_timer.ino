#include "sleep.h"
#include "utils.h"
#include "keypad.h"
#include "lcd.h"

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
