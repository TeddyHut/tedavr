#include "../include/ledButton.h"

uint8_t _ledButtonBV(uint8_t bit) {
	return(1<<bit);
}

void ledButtonSet_attrib_auto(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_AUTO);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_AUTO));
}

void ledButtonSet_attrib_currentState(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_CURRENT_STATE);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_CURRENT_STATE));
}

void ledButtonSet_attrib_previousState(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_PREVIOUS_STATE);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_PREVIOUS_STATE));
}

void ledButtonSet_attrib_pushed(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_PUSHED);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_PUSHED));
}

void ledButtonSet_attrib_released(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_RELEASED);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_RELEASED));
}

void ledButtonSet_attrib_ledState(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_LED_STATE);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_LED_STATE));
}

void ledButtonSet_attrib_ledAtPushed(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_PUSHED);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_PUSHED));
}

void ledButtonSet_attrib_ledAtReleased(LEDButton* button,uint8_t state) {
	if(state) button->attrib|=_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_RELEASED);
	else button->attrib&=~(_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_RELEASED));
}

void ledButtonSet_inPort(LEDButton* button,volatile uint8_t* state) {
	button->inPort = state;
}

void ledButtonSet_inPortBit(LEDButton* button,uint8_t state) {
	button->portBit&=LEDBUTTON_PORTBIT_OUT;
	button->portBit|=(state<<0);
}

void ledButtonSet_outPort(LEDButton* button,volatile uint8_t* state) {
	button->outPort = state;
}

void ledButtonSet_outPortBit(LEDButton* button,uint8_t state) {
	button->portBit&=LEDBUTTON_PORTBIT_IN;
	button->portBit|=(state<<4);
}

uint8_t ledButtonGet_attrib_auto(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_AUTO))>>LEDBUTTON_ATTRIB_AUTO);
}

uint8_t ledButtonGet_attrib_currentState(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_CURRENT_STATE))>>LEDBUTTON_ATTRIB_CURRENT_STATE);
}

uint8_t ledButtonGet_attrib_previousState(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_PREVIOUS_STATE))>>LEDBUTTON_ATTRIB_PREVIOUS_STATE);
}

uint8_t ledButtonGet_attrib_pushed(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_PUSHED))>>LEDBUTTON_ATTRIB_PUSHED);
}

uint8_t ledButtonGet_attrib_released(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_RELEASED))>>LEDBUTTON_ATTRIB_RELEASED);
}

uint8_t ledButtonGet_attrib_ledState(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_LED_STATE))>>LEDBUTTON_ATTRIB_LED_STATE);
}

uint8_t ledButtonGet_attrib_ledAtPushed(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_PUSHED))>>LEDBUTTON_ATTRIB_LED_AT_PUSHED);
}

uint8_t ledButtonGet_attrib_ledAtReleased(LEDButton* button) {
	return((button->attrib&_ledButtonBV(LEDBUTTON_ATTRIB_LED_AT_RELEASED))>>LEDBUTTON_ATTRIB_LED_AT_RELEASED);
}

volatile uint8_t* ledButtonGet_inPort(LEDButton* button) {
	return(button->inPort);
}

uint8_t ledButtonGet_inPortBit(LEDButton* button) {
	return((button->portBit&LEDBUTTON_PORTBIT_IN)>>0);
}

volatile uint8_t* ledButtonGet_outPort(LEDButton* button) {
	return(button->outPort);
}

uint8_t ledButtonGet_outPortBit(LEDButton* button) {
	return((button->portBit&LEDBUTTON_PORTBIT_OUT)>>4);
}

void ledButton_clear(LEDButton* button) {
	button->attrib = 0;
	ledButtonSet_inPortBit(button,0);
	ledButtonSet_outPortBit(button,0);
}

void ledButtonSetDefault(LEDButton* button) {
	ledButtonSet_attrib_auto(button,1);
	ledButtonSet_attrib_currentState(button,0);
	ledButtonSet_attrib_previousState(button,0);
	ledButtonSet_attrib_pushed(button,0);
	ledButtonSet_attrib_released(button,0);
	ledButtonSet_attrib_ledState(button,0);
	ledButtonSet_attrib_ledAtPushed(button,0);
	ledButtonSet_attrib_ledAtReleased(button,1);
}

void ledButton_runover(LEDButton* button) {
	ledButton_runoverSegment0(button);
	ledButton_runoverSegment1(button);
	ledButton_runoverSegment2(button);	
	ledButton_runoverSegment3(button);	
	ledButton_runoverSegment4(button);
}	

void ledButton_runoverSegment0(LEDButton* button) {
	ledButtonSet_attrib_pushed(button,0);
	ledButtonSet_attrib_released(button,0);
}

void ledButton_runoverSegment1(LEDButton* button) {
	if(ledButtonGet_attrib_auto(button)) {
		if(*button->inPort&_ledButtonBV(ledButtonGet_inPortBit(button))) {
			ledButtonSet_attrib_currentState(button,1);
		}
		else {
			ledButtonSet_attrib_currentState(button,0);
		}
	}
}

void ledButton_runoverSegment2(LEDButton* button) {
	if((ledButtonGet_attrib_previousState(button) == 0)&&(ledButtonGet_attrib_currentState(button) == 1)) {
		ledButtonSet_attrib_pushed(button,1);
		if(ledButtonGet_attrib_ledAtPushed(button)) {
			if(ledButtonGet_attrib_ledState(button)) {
				ledButtonSet_attrib_ledState(button,0);
			}
			else {
				ledButtonSet_attrib_ledState(button,1);
			}
		}
	}
	if((ledButtonGet_attrib_previousState(button) == 1)&&(ledButtonGet_attrib_currentState(button) == 0)) {
		ledButtonSet_attrib_released(button,1);
		if(ledButtonGet_attrib_ledAtReleased(button)) {
			if(ledButtonGet_attrib_ledState(button)) {
				ledButtonSet_attrib_ledState(button,0);
			}
			else {
				ledButtonSet_attrib_ledState(button,1);
			}
		}
	}
}

void ledButton_runoverSegment3(LEDButton* button) {
	if(ledButtonGet_attrib_ledState(button)) {
		*button->outPort|=_ledButtonBV(ledButtonGet_outPortBit(button));
	}
	else {
		*button->outPort&=~(_ledButtonBV(ledButtonGet_outPortBit(button)));
	}
}

void ledButton_runoverSegment4(LEDButton* button) {
	ledButtonSet_attrib_previousState(button,ledButtonGet_attrib_currentState(button));
}

void ledButton_turnLight(LEDButton* button,uint8_t state) {
	if(state) *button->outPort|=_ledButtonBV(ledButtonGet_outPortBit(button));
	else *button->outPort&=~(_ledButtonBV(ledButtonGet_outPortBit(button)));
}
