#pragma once

//#define __ATmega8__

#include <avr/io.h>
#include "general.h"

//TODO: Maybe use that op::Assignment class here?

class IC_HD44780 {
public:
	enum class Pin_e : uint8_t {
		data0,
		data1,
		data2,
		data3,
		en,
		rw,
		rs,
	};

	struct Pin {
		uint8_t shift_data0 : 3;
		uint8_t shift_data1 : 3;
		uint8_t shift_data2 : 3;
		uint8_t shift_data3 : 3;
		uint8_t shift_en : 3;
		uint8_t shift_rw : 3;
		uint8_t shift_rs : 3;
		//Bleh. Come up with a better way to do this that doesn't use so much RAM
		volatile uint8_t *port_out_data0;
		volatile uint8_t *port_out_data1;
		volatile uint8_t *port_out_data2;
		volatile uint8_t *port_out_data3;
		volatile uint8_t *port_out_en;
		volatile uint8_t *port_out_rw;
		volatile uint8_t *port_out_rs;
		volatile uint8_t *port_in_data0;
		volatile uint8_t *port_in_data1;
		volatile uint8_t *port_in_data2;
		volatile uint8_t *port_in_data3;
		volatile uint8_t *ddr_data0;
		volatile uint8_t *ddr_data1;
		volatile uint8_t *ddr_data2;
		volatile uint8_t *ddr_data3;
	};

	enum class instr : uint8_t {
		clear_display,			//Clear entire display and set DDRAM address to 0 in address counter
		return_home,			//Sets DDRAM address to 0 in address counter. Returns display to original position (non-shifted)
		entry_mode_set,			//Sets cursur move direction and specifies display shift
		display_power,			//Sets display on or off, cursor on or off, blinking cursor on or off.
		cursor_display_shift,	//Moves cursor and shifts display without changing DRAM contents
	};
	//Meh. Could do some fancy C++ casting and whatever, but abcdefghijklmnopqrstuvwxyz.
	enum class entry_mode_set : uint8_t {
		cursormove_left,
		cursormove_right,
		displayshift_left,
		displayshift_right
	};
	enum class display_power : uint8_t {
		display_on,
		display_off,
		cursor_on,
		cursor_off,
		cursorblink_on,
		cursorblink_off,
	};

	//Prints -p0- to the display
	IC_HD44780 const &operator<<(char const *const p0) const;
	//Executes command -p0-
	IC_HD44780 const &operator<<(instr const p0) const;


	//Returns whether the busy flag is set in the hd44780
	bool busy() const;

	IC_HD44780();
	IC_HD44780(Pin const &npin);

	Pin pin;
protected:
	//Puts 4bit -value- onto the data pins
	void data_out(uint8_t const value) const;
	//Returns 4bit value on the data pins
	uint8_t data_in() const;
};
