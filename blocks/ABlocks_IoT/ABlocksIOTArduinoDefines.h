
#ifndef _ABLOCKSIOTARDUINODEFINES_h
#define _ABLOCKSIOTARDUINODEFINES_h

#include "ABlocksIOTDefines.h"

#define ERROR_INCORRECT_PARAM "Bad param"

#if defined(ARDUINO)
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef ABLOCKSIOT_PRINT
	static void log(const char* ABLOCKSIOT_PROGMEM message, ...)
	{
		va_list args;
		va_start(args, message);
		char buffer[12];
		ABLOCKSIOT_PRINT.print('[');
		ABLOCKSIOT_PRINT.print(millis());
		ABLOCKSIOT_PRINT.print(ABLOCKSIOT_FLASH("] "));
		const char* p = message;
		size_t n = 0;
		while (1) {
			char c = ABLOCKSIOT_READ_BYTE(p++);
			if (c == '%') {
				buffer[n] = 0;
				ABLOCKSIOT_PRINT.print(buffer);
				n = 0;
				char next = ABLOCKSIOT_READ_BYTE(p++);
				switch (next) {
				case 's':
					ABLOCKSIOT_PRINT.print(va_arg(args, char*));
					break;
				case 'd':
					ABLOCKSIOT_PRINT.print(va_arg(args, int));
					break;
				case 'u':
					ABLOCKSIOT_PRINT.print(va_arg(args, unsigned int));
					break;
				case 'f':
					ABLOCKSIOT_PRINT.print(va_arg(args, double));
					break;
				case 'l':
					next = ABLOCKSIOT_READ_BYTE(p++);
					switch (next) {
					case 'x':
					case 'X':
						ABLOCKSIOT_PRINT.print(va_arg(args, long), 16);
						break;
					default:
						ABLOCKSIOT_PRINT.print(va_arg(args, long));
						break;
					}
					break;
				case 'x':
				case 'X':
					ABLOCKSIOT_PRINT.print(va_arg(args, int), 16);
					break;
				default:
					ABLOCKSIOT_PRINT.print(next);
					break;
				}
			}
			else {
				buffer[n++] = c;
				if (n >= sizeof(buffer) - 1)
				{
					buffer[n] = 0;
					ABLOCKSIOT_PRINT.print(buffer);
					n = 0;
					buffer[0] = 0;
				}
			}
			if (c == 0) {
				break;
			}
		}
		ABLOCKSIOT_PRINT.println(buffer);
		va_end(args);
	}
	#define ABLOCKSIOT_LOG(message, ...) log(ABLOCKSIOT_PSTR(message), ##__VA_ARGS__)
#else
	#define ABLOCKSIOT_LOG(message, ...)
#endif

#ifdef ABLOCKSIOT_DEBUG
	#define ABLOCKSIOT_LOG_DEBUG(message, ...) ABLOCKSIOT_LOG((message), ##__VA_ARGS__)
#else
	#define ABLOCKSIOT_LOG_DEBUG(message, ...)
#endif

#endif
