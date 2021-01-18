#include "utils.h"
#include "Arduino.h"

void write_pin(int pin, unsigned char value)
{
  volatile uint8_t *reg = portOutputRegister(digitalPinToPort(pin));
  uint8_t mask = digitalPinToBitMask(pin);
  if (value) *reg |= mask;
  else *reg &= ~mask;
}

uint8_t to_value(char digit)
{
  return digit - '0';
}
