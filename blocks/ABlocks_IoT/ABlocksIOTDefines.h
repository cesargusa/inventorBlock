#ifndef _ABLOCKSIOTDEFINES_h
#define _ABLOCKSIOTDEFINES_h

#define ABLOCKSIOT_DOMAIN "iot.eclipse.org"
#define ABLOCKSIOT_PORT 1883
#define ABLOCKSIOT_TLS_PORT 8883

#ifndef ABLOCKSIOT_MAX_MESSAGE_SIZE
#define ABLOCKSIOT_MAX_MESSAGE_SIZE 134
#endif

#ifndef ABLOCKSIOT_MAX_PAYLOAD_SIZE
#define ABLOCKSIOT_MAX_PAYLOAD_SIZE 64 
#endif

//Some defines for AVR microcontrollers to allow easier usage of memory in program space.
#if defined(__AVR__) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
#include <avr/pgmspace.h>
#define ABLOCKSIOT_USING_PROGMEM
#define ABLOCKSIOT_PROGMEM PROGMEM
#define ABLOCKSIOT_PSTR(s) PSTR(s)
#define ABLOCKSIOT_STRLEN(s) strlen_P(s)
#define ABLOCKSIOT_STRCAT(s1, s2) strcat_P(s1, s2)
#define ABLOCKSIOT_STRNCMP(s1, s2, n) strncmp_P(s1, s2, n)
#define ABLOCKSIOT_MEMCPY(s1, s2, n) memcpy_P(s1, s2, n)
#define ABLOCKSIOT_READ_BYTE(b) pgm_read_byte(b);
#define ABLOCKSIOT_FLASH(s) F(s)
#else
#define ABLOCKSIOT_PROGMEM
#define ABLOCKSIOT_PSTR(s) s
#define ABLOCKSIOT_STRLEN(s) strlen(s)
#define ABLOCKSIOT_STRCAT(s1, s2) strcat(s1, s2)
#define ABLOCKSIOT_STRNCMP(s1, s2, n) strncmp(s1, s2, n)
#define ABLOCKSIOT_MEMCPY(s1, s2, n) memcpy(s1, s2, n)
#define ABLOCKSIOT_READ_BYTE(b) *b;
#define ABLOCKSIOT_FLASH(s) s
#endif

#endif
