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
#ifndef SnoozeLib_h
#define SnoozeLi_h

#include <inttypes.h>
#include <stdbool.h>
#include <Arduino.h>

/***********************************************
 * SnoozeClass. provides Low Power Mode Control*
 * Version: 1.0.                               *
 ***********************************************/
class SnoozeLib
{
  public:
    SnoozeLib();
    void snooze(long miliseconds);//Snooze the Microcontroler for the indicates Miliseconds
    void wakeup();//Wake Up The MicroController
    private:
     int sleeping;//Variable that indicates if is SLEEPING or NOT.
     void sleep(uint8_t);//Sleep the microcontroler using the Watchdog
  
};

extern SnoozeLib snoozeLib;
#endif
