#include "../include/tedavr/button.h"

uint8_t _buttonBV(uint8_t bit) {
	return(1<<bit);
}

void buttonSet_attrib_auto(Button* button,uint8_t state) {
	if(state) button->attrib|=_buttonBV(BUTTON_ATTRIB_AUTO);
	else button->attrib&=~(_buttonBV(BUTTON_ATTRIB_AUTO));
}

void buttonSet_attrib_currentState(Button* button,uint8_t state) {
	if(state) button->attrib|=_buttonBV(BUTTON_ATTRIB_CURRENT_STATE);
	else button->attrib&=~(_buttonBV(BUTTON_ATTRIB_CURRENT_STATE));
}

void buttonSet_attrib_previousState(Button* button,uint8_t state) {
	if(state) button->attrib|=_buttonBV(BUTTON_ATTRIB_PREVIOUS_STATE);
	else button->attrib&=~(_buttonBV(BUTTON_ATTRIB_PREVIOUS_STATE));
}

void buttonSet_attrib_pushed(Button* button,uint8_t state) {
	if(state) button->attrib|=_buttonBV(BUTTON_ATTRIB_PUSHED);
	else button->attrib&=~(_buttonBV(BUTTON_ATTRIB_PUSHED));
}

void buttonSet_attrib_released(Button* button,uint8_t state) {
	if(state) button->attrib|=_buttonBV(BUTTON_ATTRIB_RELEASED);
	else button->attrib&=~(_buttonBV(BUTTON_ATTRIB_RELEASED));
}

void buttonSet_attrib_portBit(Button* button,uint8_t state) {
	button->attrib&=~((uint8_t)BUTTON_PORT_BIT_MASK);
	button->attrib|=(state<<BUTTON_PORT_BIT_SHIFT);
}

void buttonSet_port(Button* button,volatile uint8_t* state) {
	button->port = state;
}

uint8_t buttonGet_attrib_auto(Button* button) {
	return((button->attrib&_buttonBV(BUTTON_ATTRIB_AUTO))>>BUTTON_ATTRIB_AUTO);
}

uint8_t buttonGet_attrib_currentState(Button* button) {
	return((button->attrib&_buttonBV(BUTTON_ATTRIB_CURRENT_STATE))>>BUTTON_ATTRIB_CURRENT_STATE);
}

uint8_t buttonGet_attrib_previousState(Button* button) {
	return((button->attrib&_buttonBV(BUTTON_ATTRIB_PREVIOUS_STATE))>>BUTTON_ATTRIB_PREVIOUS_STATE);
}

uint8_t buttonGet_attrib_pushed(Button* button) {
	return((button->attrib&_buttonBV(BUTTON_ATTRIB_PUSHED))>>BUTTON_ATTRIB_PUSHED);
}

uint8_t buttonGet_attrib_released(Button* button) {
	return((button->attrib&_buttonBV(BUTTON_ATTRIB_RELEASED))>>BUTTON_ATTRIB_RELEASED);
}

uint8_t buttonGet_attrib_portBit(Button* button) {
	return((button->attrib&BUTTON_PORT_BIT_MASK)>>BUTTON_PORT_BIT_SHIFT);
}

volatile uint8_t* buttonGet_port(Button* button) {
	return(button->port);
}

void button_clear(Button* button) {
	button->attrib = 0;
}

void button_setDefault(Button* button) {
	button_clear(button);
	buttonSet_attrib_auto(button,1);
}

void button_runover(Button* button) {
	button_runoverSegment0(button);
	button_runoverSegment1(button);
	button_runoverSegment2(button);
	button_runoverSegment3(button);
}

void button_runoverSegment0(Button* button) {
	buttonSet_attrib_pushed(button,0);
	buttonSet_attrib_released(button,0);
}

void button_runoverSegment1(Button* button) {
	if(buttonGet_attrib_auto(button)) {
		if(*button->port&_buttonBV(buttonGet_attrib_portBit(button))) {
			buttonSet_attrib_currentState(button,1);
		}
		else {
			buttonSet_attrib_currentState(button,0);
		}
	}
}

void button_runoverSegment2(Button* button) {
	if((buttonGet_attrib_previousState(button) == 0)&&(buttonGet_attrib_currentState(button) == 1)) {
		buttonSet_attrib_pushed(button,1);
	}
	if((buttonGet_attrib_previousState(button) == 1)&&(buttonGet_attrib_currentState(button) == 0)) {
		buttonSet_attrib_released(button,1);
	}
}

void button_runoverSegment3(Button* button) {
	buttonSet_attrib_previousState(button,buttonGet_attrib_currentState(button));
}
