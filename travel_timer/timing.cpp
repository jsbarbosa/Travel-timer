#include "timing.h"
#include "utils.h"
#include <EEPROM.h>

// year/month/day hour:minute

String current_time = "21/01/01 00:00";
String travel_time = "21/01/01 00:00";

const uint8_t month_days[12] = {
  31,
  28,
  31,
  30,
  31,
  30,
  31,
  31,
  30,
  31,
  30,
  31
};


uint8_t validate_date(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
  if((month > 12) | (month < 1)) return 0;
  if(hour > 23) return 0;
  if(minute >= 60) return 0;
  
  uint8_t max_day = month_days[month - 1];
  if((month == 2) & (year % 4 == 0)) max_day = 29;
  
  if((day > max_day) | (day < 1)) return 0;
  
  return 1;
}

void split_timestamp(String timestamp, uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute)
{
  *year = to_value(timestamp[0]) * 10 + to_value(timestamp[1]);
  *month = to_value(timestamp[3]) * 10 + to_value(timestamp[4]);
  *day = to_value(timestamp[6]) * 10 + to_value(timestamp[7]);
  *hour = to_value(timestamp[9]) * 10 + to_value(timestamp[10]);
  *minute = to_value(timestamp[12]) * 10 + to_value(timestamp[13]);
}

uint8_t simple_validate_date(String timestamp)
{
  uint8_t year, month, day, hour, minute;
  split_timestamp(timestamp, &year, &month, &day, &hour, &minute);
  return validate_date(year, month, day, hour, minute);
}

String get_current_time(void)
{
  return current_time;
}

String get_travel_time(void)
{
  char temp_time[14];
  
  for(uint8_t i=0; i<14; i++)
  {
    temp_time[i] = (char)EEPROM.read(i);
  }
  
  if(simple_validate_date(temp_time))
  {
    for(uint8_t i=0; i<14; i++)
    {
      travel_time[i] = temp_time[i];
    }
  }

  else
  {
    commit_travel_time();
  }
  
  return travel_time;
}


uint8_t set_date_char(uint8_t pos, char digit, uint8_t time_indicator)
{
  char current_digit;
  uint8_t validity;
  
  if(time_indicator == CURRENT_TIME_INDICATOR)
  {
    current_digit = current_time[pos];
    current_time[pos] = digit;
    validity = simple_validate_date(current_time);
    if(validity == 0)
    {
      current_time[pos] = current_digit;
      return 0;
    }
  }
  else
  {
    current_digit = travel_time[pos];
    travel_time[pos] = digit;
    validity = simple_validate_date(travel_time);
    if(validity == 0)
    {
      travel_time[pos] = current_digit;
      return 0;
    }
  }

  return 1;
}

void commit_current_time(void)
{

}

void commit_travel_time(void)
{
  for(uint8_t i=0; i<travel_time.length(); i++)
  {
    EEPROM.write(i, (char)travel_time[i]);
  }
}
