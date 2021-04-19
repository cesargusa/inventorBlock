/*********************************************************************************
 * SnoozeLib: This Arduino Library make the use of Low Power Mode                *
 * very Easy. This library Helps you make your arduino based microcontroller     *
 * use the Low Power Mode it use the WatchDog Functionallity and sleep the       *
 * CPU clock for low consumption.                                                *
 * this library is under GNU GPL v3 license.                                     *
 * Version: 1.0.                                                                 *
 * Based on the library Narcoleptic: https://code.google.com/p/narcoleptic/      *
 * Author: Victor Suarez Garcia<suarez.garcia.victor@gmail.com>                  *
 *********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>


#include <avr/wdt.h>
#include <avr/sleep.h>

#include "SnoozeLib.h"

SIGNAL(WDT_vect) {
  wdt_disable();
  wdt_reset();
  WDTCSR &= ~_BV(WDIE);
}
/**********************
 * Default Contructor *
 **********************/
SnoozeLib::SnoozeLib(){
 this->sleeping=0; 
}
/*****************************************************************************
 *Sleep the microcontroller using the WatchDog                               *
 * parameters:                                                               *
 * uint8_t wdt_period: WatchDog Sleep Period.                                *
 * See http://nongnu.org/avr-libc/user-manual/group__avr__watchdog.html      *
 *****************************************************************************/
void SnoozeLib::sleep(uint8_t wdt_period){
  wdt_enable(wdt_period);
  wdt_reset();
  WDTCSR |= _BV(WDIE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  wdt_disable();
  WDTCSR &= ~_BV(WDIE);
}
/**************************************************************************************
 * Snooze: sleep the microcontroller for the sepecified milliseconds or the function  *
 * 'wakeup()' is called.                                                              *
 * parameter:                                                                         *
 * milliseconds: Number of milliseconds where the microcontroller is sleeped.         *
 **************************************************************************************/
void SnoozeLib::snooze(long milliseconds){
  this->sleeping=HIGH;
  while (milliseconds >= 8000) { sleep(WDTO_8S); milliseconds -= 8000; if(this->sleeping==LOW){break;}}
  if(this->sleeping==LOW){milliseconds=0;}
  if (milliseconds >= 4000)    { sleep(WDTO_4S); milliseconds -= 4000; }
  if (milliseconds >= 2000)    { sleep(WDTO_2S); milliseconds -= 2000; }
  if (milliseconds >= 1000)    { sleep(WDTO_1S); milliseconds -= 1000; }
  if (milliseconds >= 500)     { sleep(WDTO_500MS); milliseconds -= 500; }
  if (milliseconds >= 250)     { sleep(WDTO_250MS); milliseconds -= 250; }
  if (milliseconds >= 125)     { sleep(WDTO_120MS); milliseconds -= 120; }
  if (milliseconds >= 64)      { sleep(WDTO_60MS); milliseconds -= 60; }
  if (milliseconds >= 32)      { sleep(WDTO_30MS); milliseconds -= 30; }
  if (milliseconds >= 16)      { sleep(WDTO_15MS); milliseconds -= 15; }
  wdt_disable();
  sleep_disable();
}

/******************************************************************************************
 * wakeup: wake up the CPU microcontroller and stop the watchdog. if the board is already *
 * powered On this call is ommited.                                                       *
 ******************************************************************************************/
void SnoozeLib::wakeup(){
  this->sleeping=LOW;
  wdt_disable();
}

SnoozeLib snoozeLib;
