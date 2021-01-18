#include "sleep.h"
#include "utils.h"
#include "keypad.h"
#include "lcd.h"
#include "menu.h"

void setup()
{
  ADCSRA = 0; //disable the ADC
  pinMode(2, INPUT); // interrupt pin

  write_pin(POWER_OUTPUT_PIN, HIGH); // power on

  setup_display();
  setup_keypad();

  sei();

  main_view();

  delay(1000);
  //enable_timer();
}

void loop()
{  
  char key = get_key();
  if(key)
  {
    menu_update(key);
    //disable_timer();
    delay(DEBOUNCE_MS);
  }
  /*
  else{
    enable_timer();
  }
  delay(100);
  */
}
