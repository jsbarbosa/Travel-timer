#include "timing.h"
#include "utils.h"
#include <EEPROM.h>
#include "virtuabotixRTC.h"

#define RTC_CLK 4
#define RTC_DAT 3
#define RTC_RST 1
#define TIME_STAMP_LENGTH 15
#define DATE_STAMP_LENGTH 9

virtuabotixRTC myRTC(RTC_CLK, RTC_DAT, RTC_RST); //CLK, DAT, RST

// year/month/day hour:minute
char current_time[TIME_STAMP_LENGTH] = "21/01/01 00:00";
char travel_time[TIME_STAMP_LENGTH] = "21/01/01 00:00";

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


String get_current_time()
{
  myRTC.updateTime();
  
  uint8_t year = myRTC.year - 2000;
  
  snprintf_P(
    current_time,
    TIME_STAMP_LENGTH,
    PSTR("%02u/%02u/%02u %02u:%02u"),
    year, myRTC.month, myRTC.dayofmonth,
    myRTC.hours, myRTC.minutes
  );
  
  return current_time;
}

String get_travel_time(void)
{
  char temp_time[TIME_STAMP_LENGTH];
  
  for(uint8_t i=0; i < TIME_STAMP_LENGTH - 1; i++)
  {
    temp_time[i] = EEPROM.read(i);
  }
  
  if(simple_validate_date(temp_time))
  {
    for(uint8_t i=0; i<TIME_STAMP_LENGTH - 1; i++)
    {
      travel_time[i] = temp_time[i];
    }
  }

  else
  {
    commit_travel_time();
  }

  travel_time[TIME_STAMP_LENGTH - 1] = 0;
  
  return travel_time;
}


String get_travel_date(void)
{
  get_travel_time();

  char date[DATE_STAMP_LENGTH];
  
  for(uint8_t i=0; i<DATE_STAMP_LENGTH - 1; i++)
  {
    date[i] = travel_time[i];
  }

  date[DATE_STAMP_LENGTH - 1] = 0;
  
  return date;
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
  uint8_t year, month, day, hour, minute;

  split_timestamp(current_time, &year, &month, &day, &hour, &minute);

  myRTC.setDS1302Time(
    0, // seconds
    minute,
    hour,
    1, // day of week,
    day,
    month,
    year + 2000
  );
}

void commit_travel_time(void)
{
  for(uint8_t i=0; i < TIME_STAMP_LENGTH - 1; i++)
  {
    EEPROM.update(i, travel_time[i]);
  }
}


uint8_t countLeapYears(uint8_t year, uint8_t month)
{
    uint16_t years = year + 2000;
 
    if (month <= 2)
        years--;
 
    // An year is a leap year if it 
    // is a multiple of 4,
    // multiple of 400 and not a 
     // multiple of 100.
    return years / 4 
           - years / 100
           + years / 400;
}
 

uint16_t days_between(void)
{
  uint8_t c_year, c_month, c_day, c_hour, c_minute;
  uint8_t t_year, t_month, t_day, t_hour, t_minute;

  split_timestamp(current_time, &c_year, &c_month, &c_day, &c_hour, &c_minute);
  split_timestamp(travel_time, &t_year, &t_month, &t_day, &t_hour, &t_minute);

  long int n1 = (c_year + 2000) * 365 + c_day;
  long int n2=(t_year + 2000) * 365 + t_day;
 
  for (uint8_t i = 0; i < c_month - 1; i++)
      n1 += month_days[i];
  n1 += countLeapYears(c_year, c_month);
 
  for (uint8_t i = 0; i < t_month - 1; i++)
      n2 += month_days[i];
  n2 += countLeapYears(t_year, t_month);

  return (n2 - n1);
}
