#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>

#define BUTTON_ATTRIB_AUTO 0
#define BUTTON_ATTRIB_CURRENT_STATE 1
#define BUTTON_ATTRIB_PREVIOUS_STATE 2
#define BUTTON_ATTRIB_PUSHED 3
#define BUTTON_ATTRIB_RELEASED 4
#define BUTTON_ATTRIB_PORT_BIT_0 5
#define BUTTON_ATTRIB_PORT_BIT_1 6
#define BUTTON_ATTRIB_PORT_BIT_2 7

#define BUTTON_PORT_BIT_SHIFT 5
#define BUTTON_PORT_BIT_MASK 0b11100000

typedef struct Button {
	uint8_t attrib;
	volatile uint8_t* port;
}Button;

uint8_t _buttonBV(uint8_t bit);

void buttonSet_attrib_auto(Button* button,uint8_t state);
void buttonSet_attrib_currentState(Button* button,uint8_t state);
void buttonSet_attrib_previousState(Button* button,uint8_t state);
void buttonSet_attrib_pushed(Button* button,uint8_t state);
void buttonSet_attrib_released(Button* button,uint8_t state);
void buttonSet_attrib_portBit(Button* button,uint8_t state);
void buttonSet_port(Button* button,volatile uint8_t* state);
uint8_t buttonGet_attrib_auto(Button* button);
uint8_t buttonGet_attrib_currentState(Button* button);
uint8_t buttonGet_attrib_previousState(Button* button);
uint8_t buttonGet_attrib_pushed(Button* button);
uint8_t buttonGet_attrib_released(Button* button);
uint8_t buttonGet_attrib_portBit(Button* button);
volatile uint8_t* buttonGet_port(Button* button);

void button_clear(Button* button);
void button_setDefault(Button* button);

void button_runover(Button* button);
void button_runoverSegment0(Button* button);
void button_runoverSegment1(Button* button);
void button_runoverSegment2(Button* button);
void button_runoverSegment3(Button* button);

#endif //Button.h
