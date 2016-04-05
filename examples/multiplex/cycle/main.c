#include <avr/io.h>
#include <util/delay.h>
#include "../../../include/multiplex.h"

int main(void) {
	DDRC = 0b00000111;
	Multiplexer multiplexer;
	multiplexer_clear(&multiplexer);
	multiplexer_autoBindToPort(&multiplexer,&PORTC,0,MULTIPLEX_BITWIDTH_2);
	multiplexerSet_line(&multiplexer,MULTIPLEX_DATA_LINE,&PORTC,2);
	multiplexer_setChannel(&multiplexer,0);
	multiplexer_setDataLineState(&multiplexer,0);
	Demultiplexer demultiplexer;
	demultiplexer_clear(&demultiplexer);
	demultiplexer_autoBindToPort(&demultiplexer,&PORTC,0,MULTIPLEX_BITWIDTH_2);
	demultiplexerSet_line(&demultiplexer,MULTIPLEX_DATA_LINE,&PINC,3);
	demultiplexer_getDataLineState(&demultiplexer);
	uint8_t a = 0;
	while(1) {
		switch(a) {
			case(0):{
						multiplexer_setDataLineState(&multiplexer,1);
						multiplexer_setChannel(&multiplexer,a);
						a++;
						_delay_ms(1000);
						break;
					 }
			case(1):{
						multiplexer_setDataLineState(&multiplexer,0);
						multiplexer_setChannel(&multiplexer,a);
						a++;
						_delay_ms(1000);
						break;
					 }
			case(2):{
						multiplexer_setDataLineState(&multiplexer,0);
						multiplexer_setChannel(&multiplexer,a);
						a++;
						_delay_ms(1000);
						break;
					}
			case(3):{
						multiplexer_setDataLineState(&multiplexer,1);
						multiplexer_setChannel(&multiplexer,a);
						a++;
						_delay_ms(1000);
						break;
					}
			case(4):{
						a = 0;
						break;
					}
		}
	}
}
