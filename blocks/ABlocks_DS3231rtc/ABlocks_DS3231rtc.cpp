#include "ABlocks_DS3231rtc.h"

DS3231rtc::DS3231rtc(){
  hour=0;
  minute=0;
  second=0;
  day=0;
  dayOfWeek=0;
  month=0; 
  year=0; 
}
void DS3231rtc::begin(){
	Wire.begin();
}

void DS3231rtc::setTime(byte _second, byte _minute, byte _hour, byte _day, byte _month, byte _year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(_second)); // set seconds
  Wire.write(decToBcd(_minute)); // set minutes
  Wire.write(decToBcd(_hour)); // set hours
  Wire.write(DayOfWeek(_year, _month, _day)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(_day)); // set date (1 to 31)
  Wire.write(decToBcd(_month)); // set month
  Wire.write(decToBcd(_year)); // set year (0 to 99)
  Wire.endTransmission();

  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
}

byte DS3231rtc::getHour(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return hour;
}

byte DS3231rtc::getMinute(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return minute;
}

byte DS3231rtc::getSecond(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return second;
}

byte DS3231rtc::getDay(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return day;
}

byte DS3231rtc::getMonth(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return month;
}

byte DS3231rtc::getYear(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  return year;
}

void DS3231rtc::printDate(){
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
  
  Serial.print(day, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.println(year, DEC);
}

void DS3231rtc::printTime(){
 
  readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
 
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.println(second, DEC);  
}

void DS3231rtc::printDayOfWeek()
{
   readTime(&second, &minute, &hour, &dayOfWeek, &day, &month,&year);  
 
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  } 
}

//privates
byte DS3231rtc::DayOfWeek(int y, byte m, byte d) {   // y > 1752, 1 <= m <= 12
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};   
  y+=2000;
  y -= m < 3;
  return ((y + y/4 - y/100 + y/400 + t[m-1] + d) % 7) + 1; // 01 - 07, 01 = Sunday
}

void DS3231rtc::readTime(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

byte DS3231rtc::decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte DS3231rtc::bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}


