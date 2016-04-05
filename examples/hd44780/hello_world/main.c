#include <avr/io.h>
#include <util/delay.h>
#include "../../../include/tedavr/hd44780.h"

int main(void) {
	DDRC = 0b00111111;
	DDRD = 0b11111111;
	PORTC = 0x00;
	HD44780 hd44780;
	hd44780_autoBindToPort(&hd44780,&PORTC,0);
	hd44780_init(&hd44780);
	hd44780_turnDisplayOn(&hd44780,hd44780_calculateCursor(1,1));
	hd44780_setShift(&hd44780,hd44780_calculateShift(1,0));
	hd44780_clearDisplay(&hd44780);
	hd44780_putText(&hd44780,"Hello World!");
	while(1);
}
