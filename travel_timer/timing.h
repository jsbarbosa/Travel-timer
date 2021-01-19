#include "Arduino.h"

#define CURRENT_TIME_INDICATOR 0
#define TRAVEL_TIME_INDICATOR 1

String get_current_time(void);
String get_travel_time(void);
String get_travel_date(void);
void commit_current_time(void);
void commit_travel_time(void);
uint16_t days_between(void);
uint8_t set_date_char(uint8_t, char, uint8_t);
