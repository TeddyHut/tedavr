#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define HD44780_CURSOR_BLINK 0
#define HD44780_CURSOR_ON 1

#define HD44780_SHIFT_DISPLAY 0
#define HD44780_SHIFT_CURSOR 1

#define HD44780_DATA0 0
#define HD44780_DATA1 1
#define HD44780_DATA2 2
#define HD44780_DATA3 3
#define HD44780_REGISTER_SELECT 4
#define HD44780_ENABLE 5

#define HD44780_NEWLINE 40

	typedef struct HD44780 {
		uint8_t portBit[3];
		volatile uint8_t* port[6];
	}HD44780;

	uint8_t _hd44780BV(uint8_t bit);

	uint8_t hd44780_calculateLineIndex_portBit(uint8_t line);
	uint8_t hd44780_calculateLineIndex_port(uint8_t line);
	uint8_t hd44780_calculateLineMask_portBit(uint8_t line);
	uint8_t hd44780_calculateLineMask_port(uint8_t line);
	uint8_t hd44780_calculateLineShift_portBit(uint8_t line);
	uint8_t hd44780_calculateLineShift_port(uint8_t line);

	void hd44780Set_portBit(HD44780* hd44780, uint8_t line, uint8_t state);
	void hd44780Set_port(HD44780* hd44780, uint8_t line, volatile uint8_t* state);
	uint8_t hd44780Get_portBit(HD44780* hd44780, uint8_t line);
	volatile uint8_t* hd44780Get_port(HD44780* hd44780, uint8_t line);

	void hd44780Set_line(HD44780* hd44780, uint8_t line, volatile uint8_t* port, uint8_t bit);
	uint8_t hd44780Get_line_portBit(HD44780* hd44780, uint8_t line);
	volatile uint8_t* hd44780Get_line_port(HD44780* hd44780, uint8_t line);

	void hd44780_clear(HD44780* hd44780);
	void hd44780_autoBindToPort(HD44780* hd44780, volatile uint8_t* port, uint8_t startBit);

	void hd44780_setLineState(HD44780* hd44780, uint8_t line, uint8_t state);
	void hd44780_setOutput(HD44780* hd44780, uint8_t byte);
	void hd44780_sendNibble(HD44780* hd44780, uint8_t registerSelect, uint8_t nibble);
	void hd44780_sendByte(HD44780* hd44780, uint8_t registerSelect, uint8_t byte);

	void hd44780_turnDisplayOn(HD44780* hd44780, uint8_t cursor);
	void hd44780_turnDisplayOff(HD44780* hd44780);
	void hd44780_setShift(HD44780* hd44780, uint8_t shift);
	void hd44780_clearDisplay(HD44780* hd44780);
	void hd44780_setPosition(HD44780* hd44780, uint8_t pos);
	void hd44780_init(HD44780* hd44780);

	uint8_t hd44780_calculateCursor(uint8_t on, uint8_t blink);
	uint8_t hd44780_calculateShift(uint8_t cursor, uint8_t display);

	void hd44780_putText(HD44780* hd44780, char* text);

	void hd44780_putNumberi8(HD44780* hd44780, int8_t number);
	void hd44780_putNumberui8(HD44780* hd44780, uint8_t number);
	void hd44780_putNumberi16(HD44780* hd44780, int16_t number);
	void hd44780_putNumberui16(HD44780* hd44780, uint16_t number);
	void hd44780_putNumberf(HD44780* hd44780, float number);
	void hd44780_putNumberd(HD44780* hd44780, double number);
	void hd44780_putBinary(HD44780* hd44780, void* data, uint8_t size);

#ifdef __cplusplus
}
#endif
