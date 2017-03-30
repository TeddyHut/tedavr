#include "../include/tedavr/ic_hd44780.h"

IC_HD44780::operator bool() {
	switch (current_instr) {
	case hd::instr::get_busy_flag:
		cast_to_bool = busy();
		reset();
		break;
	}
	return(cast_to_bool);
}

IC_HD44780::operator uint8_t() {
	return(res());
}

uint8_t IC_HD44780::res() {
	switch (current_instr) {
	case hd::instr::get_busy_flag:
		cast_to_u8 = busy();
		reset();
		break;
	case hd::instr::get_addresscoutner:
		busy(&cast_to_u8);
		reset();
	case hd::instr::read:
		cast_to_u8 = data_read();
		break;
	default:
		break;
	}
	return(cast_to_u8);
}

IC_HD44780 &IC_HD44780::operator<<(char const *const p0) {
	uint8_t len = 0;
	bool nline = false;
	uint8_t nline_pos = 0;
	for (; p0[len] != '\0'; len++) {
		if ((p0[len] == '\n') && (nline == false)) {
			nline = true;
			nline_pos = len;
		}
	}
	if (nline) {
		*this << hd::instr::buffer_write << static_cast<void *const>(const_cast<char *const>(p0)) << nline_pos;
		*this << hd::instr::set_ddram_addr << 0x40;
		*this << hd::instr::buffer_write << static_cast<void *const>(const_cast<char *const>(p0 + nline_pos + 1)) << (len - nline_pos - 1);
	}
	else {
		*this << hd::instr::buffer_write << static_cast<void *const>(const_cast<char *const>(p0)) << len;
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(hd::instr const p0) {
	if (p0 != current_instr)
		reset();
	switch (p0) {
	case hd::instr::clear_display:
		data_write(0b00000001, 0);
		reset();
		break;
	case hd::instr::return_home:
		data_write(0b00000010, 0);
		reset();
		break;
	case hd::instr::init_4bit:
		general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
		general_SetBitFlag_u8(pin.port_out_rw, pin.shift_rw, false);
		general_SetBitFlag_u8(pin.port_out_rs, pin.shift_rs, false);
		data_out(0b0010);
		general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
		general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
		//Wait for busy flag?
		reset();
		break;
	default:
		current_instr = p0;
	}
	return(*this);
}

//TODO: Intergrate all these repetitive things into a PROGMEM parser with templates and whatnot (forgot I was in C++ for a second there).

IC_HD44780 &IC_HD44780::operator<<(hd::entry_mode_set const p0) {
	switch (p0) {
	case hd::entry_mode_set::cursormove_left:
		instr_data |= _BV(z_entrymodeset_cursormove);
		instr_status |= _BV(z_entrymodeset_cursormove);
		break;
	case hd::entry_mode_set::cursormove_right:
		instr_data &= ~_BV(z_entrymodeset_cursormove);
		instr_status |= _BV(z_entrymodeset_cursormove);
		break;
	case hd::entry_mode_set::displayshift_enable:
		instr_data |= _BV(z_entrymodeset_displayshift);
		instr_status |= _BV(z_entrymodeset_displayshift);
		break;
	case hd::entry_mode_set::displayshift_disable:
		instr_data &= ~_BV(z_entrymodeset_displayshift);
		instr_status |= _BV(z_entrymodeset_displayshift);
		break;
	default:
		break;
	}
	if (instr_status == (_BV(z_entrymodeset_cursormove) | _BV(z_entrymodeset_displayshift))) {
		data_write(0b00000100 | instr_data, 0);
		reset();
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(hd::display_power const p0) {
	switch (p0) {
	case hd::display_power::display_on:
		instr_data |= _BV(z_displaypower_display);
		instr_status |= _BV(z_displaypower_display);
		break;
	case hd::display_power::display_off:
		instr_data &= ~_BV(z_displaypower_display);
		instr_status |= _BV(z_displaypower_display);
		break;
	case hd::display_power::cursor_on:
		instr_data |= _BV(z_displaypower_cursor);
		instr_status |= _BV(z_displaypower_cursor);
		break;
	case hd::display_power::cursor_off:
		instr_data &= ~_BV(z_displaypower_cursor);
		instr_status |= _BV(z_displaypower_cursor);
		break;
	case hd::display_power::cursorblink_on:
		instr_data |= _BV(z_displaypower_cursorblink);
		instr_status |= _BV(z_displaypower_cursorblink);
		break;
	case hd::display_power::cursorblink_off:
		instr_data &= ~_BV(z_displaypower_cursorblink);
		instr_status |= _BV(z_displaypower_cursorblink);
		break;
	default:
		break;
	}
	if (instr_status == (_BV(z_displaypower_display) | _BV(z_displaypower_cursor) | _BV(z_displaypower_cursorblink))) {
		data_write(0b00001000 | instr_data, 0);
		reset();
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(hd::cursor_display_shift const p0) {
	switch (p0) {
	case hd::cursor_display_shift::select_cursor:
		instr_data &= ~_BV(z_cursordisplayshift_select);
		instr_status |= _BV(z_cursordisplayshift_select);
		break;
	case hd::cursor_display_shift::select_display:
		instr_data |= _BV(z_cursordisplayshift_select);
		instr_status |= _BV(z_cursordisplayshift_select);
		break;
	case hd::cursor_display_shift::direction_left:
		instr_data &= ~_BV(z_cursordisplayshift_direction);
		instr_status |= _BV(z_cursordisplayshift_direction);
		break;
	case hd::cursor_display_shift::direction_right:
		instr_data |= _BV(z_cursordisplayshift_direction);
		instr_status |= _BV(z_cursordisplayshift_direction);
		break;
	}
	if (instr_status == (_BV(z_cursordisplayshift_select) | _BV(z_cursordisplayshift_direction))) {
		data_write(0b00010000 | instr_data, 0);
		reset();
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(hd::function_set const p0) {
	switch (p0) {
	case hd::function_set::datalength_8:
		instr_data |= _BV(z_functionset_datalength);
		instr_status |= _BV(z_functionset_datalength);
		break;
	case hd::function_set::datalength_4:
		instr_data &= ~_BV(z_functionset_datalength);
		instr_status |= _BV(z_functionset_datalength);
		break;
	case hd::function_set::lines_1:
		instr_data &= ~_BV(z_functionset_lines);
		instr_status |= _BV(z_functionset_lines);
		break;
	case hd::function_set::lines_2:
		instr_data |= _BV(z_functionset_lines);
		instr_status |= _BV(z_functionset_lines);
		break;
	case hd::function_set::font_5x10:
		instr_data |= _BV(z_functionset_font);
		instr_status |= _BV(z_functionset_font);
		break;
	case hd::function_set::font_5x8:
		instr_data &= ~_BV(z_functionset_font);
		instr_status |= _BV(z_functionset_font);
		break;
	default:
		break;
	}
	if (instr_status == (_BV(z_functionset_datalength) | _BV(z_functionset_lines) | _BV(z_functionset_font))) {
		data_write(0b00100000 | instr_data, 0);
		reset();
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(uint8_t const p0) {
	switch (current_instr) {
	case hd::instr::set_gcram_addr:
		data_write(0b01000000 | (p0 & 0b00111111), 0);
		reset();
		break;
	case hd::instr::set_ddram_addr:
		data_write(0b10000000 | (p0 & 0b01111111), 0);
		reset();
		break;
	case hd::instr::write:
		data_write(p0);
		break;
	case hd::instr::buffer_write:
		if (instr_status == _BV(z_buffer_present)) {
			for (uint8_t i = 0; i < p0; i++) {
				data_write(static_cast<uint8_t const *const>(buffer)[i]);
			}
		}
		reset();
		break;
	case hd::instr::buffer_read:
		if (instr_status == _BV(z_buffer_present)) {
			for (uint8_t i = 0; i < p0; i++) {
				static_cast<uint8_t *const>(buffer)[i] = data_read();
			}
		}
		reset();
		break;
	}
	return(*this);
}

IC_HD44780 &IC_HD44780::operator<<(void *const p0) {
	if ((current_instr == hd::instr::buffer_read) || (current_instr == hd::instr::buffer_write)) {
		buffer = p0;
		instr_status |= _BV(z_buffer_present);
	}
	else {
		reset();
	}
	return(*this);
}

bool IC_HD44780::busy(uint8_t *const address_counter) const {
	uint8_t rtrn;
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	general_SetBitFlag_u8(pin.port_out_rs, pin.shift_rs, false);
	general_SetBitFlag_u8(pin.port_out_rw, pin.shift_rw, true);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	rtrn = data_in() << 4;
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	rtrn |= data_in();
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	if (address_counter != nullptr)
		*address_counter = rtrn & 0b01111111;
	return(rtrn & 0b10000000 ? true : false);
}

void IC_HD44780::reset() {
	buffer = nullptr;
	instr_data = 0;
	instr_status = 0;
	current_instr = hd::instr::neutral;
}

IC_HD44780::IC_HD44780() {
}

IC_HD44780::IC_HD44780(Pin const &npin) : pin(npin) {
}

uint8_t IC_HD44780::data_read(uint8_t const rs_data, uint8_t const rw_read) const {
	uint8_t rtrn;
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	general_SetBitFlag_u8(pin.port_out_rs, pin.shift_rs, rs_data);
	general_SetBitFlag_u8(pin.port_out_rw, pin.shift_rw, rw_read);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	rtrn = data_in() << 4;
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	rtrn |= data_in();
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	while (busy());
	return(rtrn);
}

void IC_HD44780::data_write(uint8_t const value, uint8_t const rs_data, uint8_t const rw_read) const {
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	general_SetBitFlag_u8(pin.port_out_rs, pin.shift_rs, rs_data);
	general_SetBitFlag_u8(pin.port_out_rw, pin.shift_rw, rw_read);
	data_out(value >> 4);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	data_out(value & 0x0f);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, true);
	general_SetBitFlag_u8(pin.port_out_en, pin.shift_en, false);
	while (busy());
}

uint8_t IC_HD44780::data_in() const {
	set_input();
	return((general_GetBitFlag_u8(pin.port_in_data3, pin.shift_data3) << 3) |
		(general_GetBitFlag_u8(pin.port_in_data2, pin.shift_data2) << 2) |
		(general_GetBitFlag_u8(pin.port_in_data1, pin.shift_data1) << 1) |
		(general_GetBitFlag_u8(pin.port_in_data0, pin.shift_data0) << 0));
}

void IC_HD44780::data_out(uint8_t const value) const {
	set_output();
	general_SetBitFlag_u8(pin.port_out_data3, pin.shift_data3, value & 0b1000);
	general_SetBitFlag_u8(pin.port_out_data2, pin.shift_data2, value & 0b0100);
	general_SetBitFlag_u8(pin.port_out_data1, pin.shift_data1, value & 0b0010);
	general_SetBitFlag_u8(pin.port_out_data0, pin.shift_data0, value & 0b0001);
}

void IC_HD44780::set_input() const {
	general_SetBitFlag_u8(pin.ddr_data0, pin.shift_data0, false);
	general_SetBitFlag_u8(pin.ddr_data1, pin.shift_data1, false);
	general_SetBitFlag_u8(pin.ddr_data2, pin.shift_data2, false);
	general_SetBitFlag_u8(pin.ddr_data3, pin.shift_data3, false);
}

void IC_HD44780::set_output() const {
	general_SetBitFlag_u8(pin.ddr_data0, pin.shift_data0, true);
	general_SetBitFlag_u8(pin.ddr_data1, pin.shift_data1, true);
	general_SetBitFlag_u8(pin.ddr_data2, pin.shift_data2, true);
	general_SetBitFlag_u8(pin.ddr_data3, pin.shift_data3, true);
}
