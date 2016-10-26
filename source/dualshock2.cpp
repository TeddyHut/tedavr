#include "../include/tedavr/dualshock2.h"

bool Dualshock2::spi_reinit = false;
bool Dualshock2::spi_deinit = false;
bool Dualshock2::spi_status = false;

void Dualshock2::spi_enable() {
	//Set PORTB2 (SS) to output to prevent interrupt in master mode. Set SCK and MOSI to output
	TEDAVR_DUALSHOCK2_EXPRESSION_SS_DDR |= _BV(TEDAVR_DUALSHOCK2_EXPRESSION_SS_PIN);
	TEDAVR_DUALSHOCK2_EXPRESSION_SCK_DDR |= _BV(TEDAVR_DUALSHOCK2_EXPRESSION_SCK_PIN);
	TEDAVR_DUALSHOCK2_EXPRESSION_MOSI_DDR |= _BV(TEDAVR_DUALSHOCK2_EXPRESSION_MOSI_PIN);
	//Set MISO to input
	TEDAVR_DUALSHOCK2_EXPRESSION_MISO_DDR &=~ _BV(TEDAVR_DUALSHOCK2_EXPRESSION_MISO_PIN);
	//Reset the SPI control register
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_RESET;
	//Master mode
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_MASTERMODE;
	//Clock high when idle
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_CLOCKHIGHIDLE;
	//Sample on trailing edge (low->high clock)
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_TRAILSAMPLE;
	//LSB first
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_LSBFIRST;
	//Running on internal 8Mhz clock, prescale 32 to get a 250khz clock speed
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_PRESCALESET;
	//Enable SPI interface
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_ENABLE;
}

void Dualshock2::spi_disable() {
	//Rese the SPI control register
	TEDAVR_DUALSHOCK2_EXPRESSION_SPICONTROL_RESET;
}

uint8_t Dualshock2::spi_byte(uint8_t const out, bool const standalone) const {
	if (standalone)
		//Pull the SS line low
		*ss_port &= ~_BV(ss_pin);
	//Begin transmission
	TEDAVR_DUALSHOCK2_EXPRESSION_SPIOUT = out;
	//Wait for transmission to complete.
	while (TEDAVR_DUALSHOCK2_EXPRESSION_TRANSMISSIONINCOMPLETE);
	if (standalone)
		//Bring the SS line high
		*ss_port |= _BV(ss_pin);
	//Return the data
	return(TEDAVR_DUALSHOCK2_EXPRESSION_SPIOUT);
}

void Dualshock2::spi_buffer(void *const buffer, uint8_t const len, bool const standalone) const {
	uint8_t *const buf = static_cast<uint8_t *const>(buffer);
	//Pull the SS line low
	if(standalone)
		*ss_port &= ~_BV(ss_pin);
	for (uint8_t i = 0; i < len; i++) {
		buf[i] = spi_byte(buf[i], false);
	}
	//Bring the SS line high
	if(standalone)
		*ss_port |= _BV(ss_pin);
}

void Dualshock2::spi_autoLen_config_buffer(void *const buffer, uint8_t const max_len) {
	uint8_t *const buf = static_cast<uint8_t *const>(buffer);
	*ss_port &= ~_BV(ss_pin);

	if (max_len <= Dualshock2Data::Slave::Header::len)
		return;

	//Retreive the header
	spi_buffer(buffer, Dualshock2Data::Slave::Header::len, false);

	//Do a calculation to get the packet size
	uint8_t const packetSize = 2 * ((buf[Dualshock2Data::Slave::Header::PacketWords::offset] & Dualshock2Data::Slave::Header::PacketWords::mask) >> Dualshock2Data::Slave::Header::PacketWords::shift);

	//Get the data
	spi_buffer(buf + Dualshock2Data::Slave::Header::len,
		packetSize > (max_len - Dualshock2Data::Slave::Header::len) ?
		(max_len - Dualshock2Data::Slave::Header::len)
		: packetSize, false);

	*ss_port |= _BV(ss_pin);

	//Update mode and last size
	degen_header(buf, &header_prev);
}

void Dualshock2::init() {
	using namespace Dualshock2Data::Master;

	spi_open();

	uint8_t buffer[Dualshock2Data::Common::max_len];
#define DELAY_TIME 5000
	//Generate config buffer
	gen_header(buffer, Header::Command::Config, 0);
	gen_command_config(buffer, true, Header::len);
	_delay_us(DELAY_TIME);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	gen_header(buffer, Header::Command::Mode, 0);
	gen_command_mode(buffer, true, true, Header::len);
	_delay_us(DELAY_TIME);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	gen_header(buffer, Header::Command::MotorMap, 0);
	gen_command_motorMap(buffer, true, Header::len);
	_delay_us(DELAY_TIME);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	gen_header(buffer, Header::Command::PressureSetup, 0);
	gen_command_pressureSetup(buffer, Header::len);
	_delay_us(DELAY_TIME);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	gen_header(buffer, Header::Command::Config, 0);
	gen_command_config(buffer, false, Header::len);
	_delay_us(DELAY_TIME);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	spi_close();
}

void Dualshock2::update() {
	using namespace Dualshock2Data;

	spi_open();

	uint8_t buffer[Common::max_len];

	//Generate command to poll device
	gen_header(buffer, Master::Header::Command::Poll, 0);
	gen_command_poll(buffer, button[Button::LeftRumble].a, button[Button::RightRumble].a, Master::Header::len);
	spi_autoLen_config_buffer(buffer, sizeof(buffer));

	if (header_prev.mode == Mode::Disconnected) {
		init();
		return;
	}

	uint16_t buttonData;
	degen_buttons(buffer, &buttonData, Master::Header::len);

	button[Button::A].d = !(_BV(Button::A) & buttonData);
	button[Button::B].d = !(_BV(Button::B) & buttonData);
	button[Button::X].d = !(_BV(Button::X) & buttonData);
	button[Button::Y].d = !(_BV(Button::Y) & buttonData);
	button[Button::L1].d = !(_BV(Button::L1) & buttonData);
	button[Button::R1].d = !(_BV(Button::R1) & buttonData);
	button[Button::L2].d = !(_BV(Button::L2) & buttonData);
	button[Button::R2].d = !(_BV(Button::R2) & buttonData);
	button[Button::L3].d = !(_BV(Button::L3) & buttonData);
	button[Button::R3].d = !(_BV(Button::R3) & buttonData);
	button[Button::Up].d = !(_BV(Button::Up) & buttonData);
	button[Button::Down].d = !(_BV(Button::Down) & buttonData);
	button[Button::Left].d = !(_BV(Button::Left) & buttonData);
	button[Button::Right].d = !(_BV(Button::Right) & buttonData);
	button[Button::Start].d = !(_BV(Button::Start) & buttonData);
	button[Button::Select].d = !(_BV(Button::Select) & buttonData);

	button[Button::A].a = buffer[Slave::Packet::Poll::Pressure::A::offset + Master::Header::len];
	button[Button::B].a = buffer[Slave::Packet::Poll::Pressure::B::offset + Master::Header::len];
	button[Button::X].a = buffer[Slave::Packet::Poll::Pressure::X::offset + Master::Header::len];
	button[Button::Y].a = buffer[Slave::Packet::Poll::Pressure::Y::offset + Master::Header::len];
	button[Button::L1].a = buffer[Slave::Packet::Poll::Pressure::L1::offset + Master::Header::len];
	button[Button::R1].a = buffer[Slave::Packet::Poll::Pressure::R1::offset + Master::Header::len];
	button[Button::L2].a = buffer[Slave::Packet::Poll::Pressure::L2::offset + Master::Header::len];
	button[Button::R2].a = buffer[Slave::Packet::Poll::Pressure::R2::offset + Master::Header::len];
	button[Button::Up].a = buffer[Slave::Packet::Poll::Pressure::Up::offset + Master::Header::len];
	button[Button::Down].a = buffer[Slave::Packet::Poll::Pressure::Down::offset + Master::Header::len];
	button[Button::Left].a = buffer[Slave::Packet::Poll::Pressure::Left::offset + Master::Header::len];
	button[Button::Right].a = buffer[Slave::Packet::Poll::Pressure::Right::offset + Master::Header::len];

	//For some reason reinterpret_cast didn't work here.
	stick[Stick::Left].pos_x = (static_cast<int16_t>(buffer[Slave::Packet::Poll::Stick::Left::X::offset + Master::Header::len]) - 0x7f) * 0xff;
	stick[Stick::Left].pos_y = (static_cast<int16_t>(buffer[Slave::Packet::Poll::Stick::Left::Y::offset + Master::Header::len]) - 0x7f) * 0xff;
	stick[Stick::Right].pos_x = (static_cast<int16_t>(buffer[Slave::Packet::Poll::Stick::Right::X::offset + Master::Header::len]) - 0x7f) * 0xff;
	stick[Stick::Right].pos_y = (static_cast<int16_t>(buffer[Slave::Packet::Poll::Stick::Right::Y::offset + Master::Header::len]) - 0x7f) * 0xff;

	gp_update();

	spi_close();
}

Dualshock2::Dualshock2() {
	set_defaultButton();
	gp_reset();
	set_specButton();
}

Dualshock2::Dualshock2(volatile uint8_t *const ss_port, uint8_t const ss_pin) : ss_port(ss_port), ss_pin(ss_pin) {
	set_defaultButton();
	gp_reset();
	set_specButton();
}

void Dualshock2::spi_open() {
	if (spi_reinit && (!spi_status)) {
		spi_enable();
		spi_status = true;
	}
}

void Dualshock2::spi_close() {
	if (spi_deinit && spi_status) {
		spi_disable();
		spi_status = false;
	}
}

void Dualshock2::set_defaultButton() {
	button_default.a = 0;
	button_default.d = false;
	button_default.i_analog = true;
	button_default.i_digital = true;
	button_default.t_analog = 0xff;
}

void Dualshock2::set_specButton() {
	button[Button::L3].i_analog = false;
	button[Button::R3].i_analog = false;
	button[Button::Start].i_analog = false;
	button[Button::Select].i_analog = false;
	button[Button::RightRumble].i_analog = false;
}

void Dualshock2::gen_header(uint8_t buf[], Dualshock2Data::Master::Header::Command::e const data, uint8_t const offset) const {
	//Copy in the seed
	memcpy_P(buf+offset, Dualshock2Data::Master::Header::seed, Dualshock2Data::Master::Header::len);
	//Fill in the command
	buf[Dualshock2Data::Master::Header::Command::offset+offset] = static_cast<uint8_t const>(data);
}

void Dualshock2::degen_header(uint8_t buf[], Dualshock2::SlaveHeader *const data, uint8_t const offset) const {
	if (buf[Dualshock2Data::Slave::Header::Response::offset + offset] == Dualshock2Data::Slave::Header::Response::bad_value) {
		data->mode = Mode::Disconnected;
		return;
	}
	uint8_t mode = ((buf[Dualshock2Data::Slave::Header::Mode::offset + offset] & Dualshock2Data::Slave::Header::Mode::mask) >> Dualshock2Data::Slave::Header::Mode::shift);
	switch (mode) {
	case Dualshock2Data::Slave::Header::Mode::Digital:
		data->mode = Mode::Digital;
		break;
	case Dualshock2Data::Slave::Header::Mode::Analog:
		data->mode = Mode::Analog;
		break;
	case Dualshock2Data::Slave::Header::Mode::Config:
		data->mode = Mode::Config;
		break;
	default:
		break;
	}
	//*2 because returned size is words, +3 becuase of header.
	data->packetSize = 2 * ((buf[Dualshock2Data::Slave::Header::PacketWords::offset + offset] & Dualshock2Data::Slave::Header::PacketWords::mask) >> Dualshock2Data::Slave::Header::PacketWords::shift) + Dualshock2Data::Slave::Header::len;
}

void Dualshock2::gen_buttons(uint8_t buf[], uint16_t const data, uint8_t const offset) const {
	uint16_t out;
	dg_button_remap(data, &out, true);
	buf[offset] = out & 0x00ff;
	buf[offset+1] = (out & 0xff00) >> 8;
}

void Dualshock2::degen_buttons(uint8_t const buf[], uint16_t *const data, uint8_t const offset) const {
	//Just being carful of edians
	uint16_t in = buf[offset] | (buf[offset+1] << 8);	
	dg_button_remap(in, data, false);
}

void Dualshock2::gen_command_poll(uint8_t buf[], uint8_t const vibl, uint8_t const vibr, uint8_t const offset) const {
	//Copy the poll seed into the buffer
	memcpy_P(buf + offset, Dualshock2Data::Master::Packet::Poll::seed, Dualshock2Data::Master::Packet::Poll::len);
	//Add in the vibration motor values
	buf[Dualshock2Data::Master::Packet::Poll::Rumble::Left::offset + offset] = vibl;
	buf[Dualshock2Data::Master::Packet::Poll::Rumble::Right::offset + offset] = vibr;
}

void Dualshock2::gen_command_config(uint8_t buf[], bool const config_mode, uint8_t const offset) const {
	//Copy the config seed into the buffer
	memcpy_P(buf + offset, Dualshock2Data::Master::Packet::Config::seed, Dualshock2Data::Master::Packet::Config::len);
	//Add in the config value
	buf[Dualshock2Data::Master::Packet::Config::Mode::offset + offset] = (config_mode ? Dualshock2Data::Master::Packet::Config::Mode::Config : Dualshock2Data::Master::Packet::Config::Mode::Normal);
}

void Dualshock2::gen_command_motorMap(uint8_t buf[], bool const left_right, uint8_t const offset) const {
	//Copy the motormap seed into the buffer
	memcpy_P(buf + offset, Dualshock2Data::Master::Packet::MotorMap::seed, Dualshock2Data::Master::Packet::MotorMap::len);
	if (left_right) {
		buf[Dualshock2Data::Master::Packet::MotorMap::X00::offset + offset] = Dualshock2Data::Master::Packet::MotorMap::Left;
		buf[Dualshock2Data::Master::Packet::MotorMap::X01::offset + offset] = Dualshock2Data::Master::Packet::MotorMap::Right;
	}
	else {
		buf[Dualshock2Data::Master::Packet::MotorMap::X00::offset + offset] = Dualshock2Data::Master::Packet::MotorMap::Right;
		buf[Dualshock2Data::Master::Packet::MotorMap::X01::offset + offset] = Dualshock2Data::Master::Packet::MotorMap::Left;
	}
}

void Dualshock2::gen_command_mode(uint8_t buf[], bool const analog, bool const locked, uint8_t const offset) const {
	//Copy the mode seed into the buffer
	memcpy_P(buf + offset, Dualshock2Data::Master::Packet::Mode::seed, Dualshock2Data::Master::Packet::Mode::len);
	buf[Dualshock2Data::Master::Packet::Mode::Mode::offset + offset] = analog ? Dualshock2Data::Master::Packet::Mode::Mode::Analog : Dualshock2Data::Master::Packet::Mode::Mode::Digital;
	buf[Dualshock2Data::Master::Packet::Mode::Lock::offset + offset] = locked ? Dualshock2Data::Master::Packet::Mode::Lock::Locked : Dualshock2Data::Master::Packet::Mode::Lock::Unlocked;
}

void Dualshock2::gen_command_pressureSetup(uint8_t buf[], uint8_t const offset) const {
	//Copy the seed into the buffer
	memcpy_P(buf + offset, Dualshock2Data::Master::Packet::PressureSetup::seed, Dualshock2Data::Master::Packet::PressureSetup::len);
}

void Dualshock2::dg_button_remap(uint16_t const in, uint16_t *const out, bool in_highNibble) const {
	//Iterate through all the digital buttons
	for (uint8_t i = 0; i < 0x10; i++) {
		//Iterate through all buttonmaps to find the one that matches the current digital button
		uint8_t nibble_out;
		for (uint8_t j = 0; j < 0x10; j++) {
			uint8_t pgm_byte;
#ifndef __INTELLISENSE__
			pgm_byte = pgm_read_byte(Dualshock2Data::buttonMap + j);
#endif
			if ((in_highNibble ? ((pgm_byte & 0xf0) >> 4) : (pgm_byte & 0x0f)) == i) {
				nibble_out = (in_highNibble ? (pgm_byte & 0x0f) : ((pgm_byte & 0xf0) >> 4));
				break;
			}
		}
		in & _BV(i) ? *out |= _BV(nibble_out) : *out &= ~_BV(nibble_out);
	}
}
