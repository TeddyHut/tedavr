#pragma once

//#define __ATmega8__

#include <avr/io.h>
#include "general.h"

//TODO: Maybe use that op::Assignment class here?

namespace hd {
	enum class instr : uint8_t {
		clear_display,			//Clear entire display and set DDRAM address to 0 in address counter
		return_home,			//Sets DDRAM address to 0 in address counter. Returns display to original position (non-shifted)
		entry_mode_set,			//Sets cursur move direction and specifies display shift;											Accepts 2 parameters.
		display_power,			//Sets display on or off, cursor on or off, blinking cursor on or off;								Accepts 3 parameters.
		cursor_display_shift,	//Moves cursor and shifts display without changing DRAM content;									Accepts 2 parameter.
		function_set,			//Sets interface data length, number of display lines, and character font;							Accepts 3 parameters.
		set_gcram_addr,			//Sets the character generator RAM address;															Accepts 1 u8.
		set_ddram_addr,			//Sets the *whatever it is* RAM address																Accepts 1 u8.
		get_busy_flag,			//Reads the busy flag																				cast to <bool> to get (after instructing, works once)
		get_addresscoutner,		//Reads the address counter																			use res to get (after instructing, works once)
		write,					//Writes to CG or DD RAM																			Accepts infinate u8.
		read,					//Reads from CG or DD RAM																			use res to get (after instructing, works infinate)
		buffer_write,			//Writes to CG or DD RAM from a buffer
		buffer_read,			//Reads from CG ot DD RAM to a buffer
		init_4bit,				//Used when initializing the device, will initialise 4 bit mode.
		neutral,				//Awaiting another instruction
	};
	//Meh. Could do some fancy C++ casting and whatever, but abcdefghijklmnopqrstuvwxyz.
	enum class entry_mode_set : uint8_t {
		cursormove_left,
		cursormove_right,
		displayshift_enable,
		displayshift_disable,
	};
	enum class display_power : uint8_t {
		display_on,
		display_off,
		cursor_on,
		cursor_off,
		cursorblink_on,
		cursorblink_off,
	};
	enum class cursor_display_shift : uint8_t {
		select_cursor,
		select_display,
		direction_left,
		direction_right,
	};
	enum class function_set : uint8_t {
		datalength_8,
		datalength_4,
		lines_1,
		lines_2,
		font_5x10,
		font_5x8,
	};
}

class IC_HD44780 {
public:
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

	static constexpr uint8_t z_entrymodeset_cursormove = 1;
	static constexpr uint8_t z_entrymodeset_displayshift = 0;

	static constexpr uint8_t z_displaypower_display = 2;
	static constexpr uint8_t z_displaypower_cursor = 1;
	static constexpr uint8_t z_displaypower_cursorblink = 0;

	static constexpr uint8_t z_cursordisplayshift_select = 3;
	static constexpr uint8_t z_cursordisplayshift_direction = 2;

	static constexpr uint8_t z_functionset_datalength = 4;
	static constexpr uint8_t z_functionset_lines = 3;
	static constexpr uint8_t z_functionset_font = 2;

	static constexpr uint8_t z_buffer_present = 0;

	explicit operator bool();		//Had to make these explicit because without them <<uint8_t conflicted with <<(int, int) becuase HD44780 would be converted implicitly to int by these
	explicit operator uint8_t();
	uint8_t res();
	//Prints -p0- to the display (or to whichever RAM was last used :)
	IC_HD44780 &operator<<(char const *const p0);
	//Executes command -p0-
	IC_HD44780 &operator<<(hd::instr const p0);
	IC_HD44780 &operator<<(hd::entry_mode_set const p0);
	IC_HD44780 &operator<<(hd::display_power const p0);
	IC_HD44780 &operator<<(hd::cursor_display_shift const p0);
	IC_HD44780 &operator<<(hd::function_set const p0);
	IC_HD44780 &operator<<(uint8_t const p0);
	IC_HD44780 &operator<<(void *const p0);
	//IC_HD44780 &operator<<(void const *const p0);	<- WE don't have infinate memory I guess.

	//Returns whether the busy flag is set in the hd44780
	bool busy(uint8_t *const address_counter = nullptr) const;
	void reset();

	IC_HD44780();
	IC_HD44780(Pin const &npin);

	Pin pin;

	hd::instr current_instr = hd::instr::neutral;

	void *buffer = nullptr;

	bool cast_to_bool = false;
	uint8_t cast_to_u8 = 0;
	uint8_t instr_data = 0;
	uint8_t instr_status = 0;
protected:
	//Reads an 8bit value from the HD44780 (and waits for busy flag)
	uint8_t data_read(uint8_t const rs_data = 1, uint8_t const rw_read = 1) const;
	//Writes an 8bit value to the HD44780 (and waits for busy flag)
	void data_write(uint8_t const value, uint8_t rs_data = 1, uint8_t const rw_read = 0) const;
	//Returns 4bit value on the data pins
	uint8_t data_in() const;
	//Puts 4bit -value- onto the data pins
	void data_out(uint8_t const value) const;
	//Sets all the data pins to inputs
	void set_input() const;
	//Sets all the data pins to outputs
	void set_output() const;
};
