#ifndef LEDBUTTON_H
#define LEDBUTTON_H

#include <avr/io.h>

#define LEDBUTTON_ATTRIB_AUTO 0
#define LEDBUTTON_ATTRIB_CURRENT_STATE 1
#define LEDBUTTON_ATTRIB_PREVIOUS_STATE 2
#define LEDBUTTON_ATTRIB_PUSHED 3
#define LEDBUTTON_ATTRIB_RELEASED 4
#define LEDBUTTON_ATTRIB_LED_STATE 5
#define LEDBUTTON_ATTRIB_LED_AT_PUSHED 6
#define LEDBUTTON_ATTRIB_LED_AT_RELEASED 7

#define LEDBUTTON_PORTBIT_IN 0x0F
#define LEDBUTTON_PORTBIT_OUT 0xF0

typedef struct LEDButton {
	uint8_t attrib;
	volatile uint8_t* inPort;
	volatile uint8_t* outPort;
	uint8_t portBit;
}LEDButton;

uint8_t _ledButtonBV(uint8_t bit);

void ledButtonSet_attrib_auto(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_currentState(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_previousState(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_pushed(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_released(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_ledState(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_ledAtPushed(LEDButton* button,uint8_t state);
void ledButtonSet_attrib_ledAtReleased(LEDButton* button,uint8_t state);
void ledButtonSet_inPort(LEDButton* button,volatile uint8_t* state);
void ledButtonSet_inPortBit(LEDButton* button,uint8_t state);
void ledButtonSet_outPort(LEDButton* button,volatile uint8_t* state);
void ledButtonSet_outPortBit(LEDButton* button,uint8_t state);
uint8_t ledButtonGet_attrib_auto(LEDButton* button);
uint8_t ledButtonGet_attrib_currentState(LEDButton* button);
uint8_t ledButtonGet_attrib_previousState(LEDButton* button);
uint8_t ledButtonGet_attrib_pushed(LEDButton* button);
uint8_t ledButtonGet_attrib_released(LEDButton* button);
uint8_t ledButtonGet_attrib_ledState(LEDButton* button);
uint8_t ledButtonGet_attrib_ledAtPushed(LEDButton* button);
uint8_t ledButtonGet_attrib_ledAtReleased(LEDButton* button);
volatile uint8_t* ledButtonGet_inPort(LEDButton* button);
uint8_t ledButtonGet_inPortBit(LEDButton* button);
volatile uint8_t* ledButtonGet_outPort(LEDButton* button);
uint8_t ledButtonGet_outPortBit(LEDButton* button);

void ledButton_clear(LEDButton* button);
void ledButton_setDefault(LEDButton* button);

void ledButton_runover(LEDButton* button);
void ledButton_runoverSegment0(LEDButton* button);
void ledButton_runoverSegment1(LEDButton* button);
void ledButton_runoverSegment2(LEDButton* button);
void ledButton_runoverSegment3(LEDButton* button);
void ledButton_runoverSegment4(LEDButton* button);

void ledButton_turnLight(LEDButton* button,uint8_t state);

#endif //LEDBUTTON_H
