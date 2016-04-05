#include <avr/io.h>
#include <util/delay.h>
#include "../../../include/button.h"

int main(void) {
	DDRD = 0b00000011;
	PORTD = 0x00;
	Button button;
	button_setDefault(&button);
	buttonSet_port(&button,&PIND);
	buttonSet_attrib_portBit(&button,6);
	uint8_t ledState = 0;
	while(1) {
		if(buttonGet_attrib_pushed(&button)) {
			if(ledState) {
			   	ledState = 0;
				PORTD&=~(_buttonBV(0));
			}
			else {
				ledState = 1;
				PORTD|=_buttonBV(0);
			}
		}
		button_runover(&button);
	}
}
