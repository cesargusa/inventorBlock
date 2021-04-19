/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef ABlocks_Button_h
#define ABlocks_Button_h
#include "Arduino.h"

class Button
{
	public:
		Button(uint8_t pin,uint16_t d);
		void begin();
		bool read();
		bool toggled();
		bool pressed();
		bool released();
		bool has_changed();
		
		const static bool PRESSED = HIGH;
		const static bool RELEASED = LOW;
	
	private:
		uint8_t  _pin;
		uint16_t _delay;
		bool     _state;
		bool     _has_changed;
		uint32_t _ignore_until;
};

#endif
