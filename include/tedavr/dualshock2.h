#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <string.h>

#include "../../device/dualshock2.h"
#include "gamepad.h"
#include "dualshock2Data.h"

//Guess I'll just use inheritance. Could just make it a member, but whatever.
class Dualshock2 : public Gamepad {
public:
	//Try to put that mapping in here

	enum class Mode : uint8_t {
		Digital,
		Analog,
		Config,
		Disconnected,
	};

	struct SlaveHeader {
		Mode mode;
		uint8_t packetSize;
	};

	//Reinitialize SPI settings each time a function is called
	static bool spi_reinit;
	//Disable SPI settings at the conclusion of a function
	static bool spi_deinit;
	//Status of SPI: initialized or not (1 = initialized); Probably don't even need it.
	static bool spi_status;
	//Enable/init SPI
	static void spi_enable();
	//Disable SPI
	static void spi_disable();

	//Send a byte over SPI
	uint8_t spi_byte(uint8_t const out, bool const standalone = true) const;
	//Send a buffer over SPI
	void spi_buffer(void *const buffer, uint8_t const len, bool const standalone = true) const;
	//During packet transfer make the max packet length the length returned by the header. Also set the current mode to the mode returned.
	void spi_autoLen_config_buffer(void *const buffer, uint8_t const max_len);

	//Init the Dualshock
	void init();
	//Update the Dualshock
	void update();

	volatile uint8_t *ss_port;
	uint8_t ss_pin : 3;
	SlaveHeader header_prev;
	//Used to give the controller an extra poll commant after it has connected for a little more time.
	bool reconnect_loop;

	Dualshock2();
	Dualshock2(volatile uint8_t *const ss_port, uint8_t const ss_pin);
protected:
	static void spi_open();
	static void spi_close();

	void set_defaultButton();
	void set_specButton();

	void gen_header(uint8_t buf[], Dualshock2Data::Master::Header::Command::e const data, uint8_t const offset = 0) const;
	void degen_header(uint8_t buf[], SlaveHeader *const data, uint8_t const offset = 0) const;
	void gen_buttons(uint8_t buf[], uint16_t const data, uint8_t const offset = 0) const;
	void degen_buttons(uint8_t const buf[], uint16_t *const data, uint8_t const offset = 0) const;

	//General controller poll command
	void gen_command_poll(uint8_t buf[], uint8_t const vibl, uint8_t const vibr, uint8_t const offset = 0) const;
	//Change config mode
	void gen_command_config(uint8_t buf[], bool const config_mode, uint8_t offset = 0) const;
	//Map vibratation motor values
	void gen_command_motorMap(uint8_t buf[], bool const left_right = true, uint8_t const offset = 0) const;
	//Analog or digital mode
	void gen_command_mode(uint8_t buf[], bool const analog, bool const locked, uint8_t const offset = 0) const;
	//Pressure setup
	void gen_command_pressureSetup(uint8_t bufer[], uint8_t const offset = 0) const;

	void dg_button_remap(uint16_t const in, uint16_t *const out, bool in_highNibble = false) const;
};
