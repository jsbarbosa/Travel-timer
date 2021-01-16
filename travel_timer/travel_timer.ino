#include "sleep.h"
#include "utils.h"
#include "keypad.h"
#include "lcd.h"
#include "menu.h"

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
  LCD.blink();
  LCD.setCursor(13, 1);
  LCD.print(millis() / 1000);

  char key = get_key();
  if(key)
  {
    update(key);
    delay(DEBOUNCE_MS);
    //disable_timer();
  }
  else{
    //enable_timer();
  }
}
