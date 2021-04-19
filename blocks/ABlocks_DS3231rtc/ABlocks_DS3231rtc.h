#ifndef DS3231RTC_h
#define DS3231RTC_h

#include <Arduino.h>
#include <Wire.h>

#define DS3231_I2C_ADDRESS 0x68

#define DS3231_SUNDAY 1
#define DS3231_MONDAY 2
#define DS3231_TUESDAY 3
#define DS3231_WEDNESDAY 4
#define DS3231_THURSDAY 5
#define DS3231_FRIDAY 6
#define DS3231_SATURDAY 7

class DS3231rtc
{
  // user-accessible "public" interface
  public:
    DS3231rtc();
	  void begin();
	
	  void setTime(byte second, byte minute, byte hour, byte dayOfMonth, byte month, byte year);
    void readTime(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year);
	  byte getHour();
    byte getMinute();
    byte getSecond();
    byte getDay();
    byte getMonth();
    byte getYear();
    byte getDayOfWeek();
    
    void printDate();
    void printTime();
    void printDayOfWeek();
    
  private:
    
	  byte decToBcd(byte val);
	  byte bcdToDec(byte val);
	  byte DayOfWeek(int y, byte m, byte d);

    byte second;
    byte minute;
    byte hour;
    byte dayOfWeek;
    byte day;
    byte month;
    byte year;

};

#endif
