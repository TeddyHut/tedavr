#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <inttypes.h>
#include <avr/sfr_defs.h>

typedef struct {
	uint8_t flag_function_updateCurrent : 1;	//Default = 1; Update current button state
	uint8_t flag_function_updatePrevious : 1;	//Default = 1; Update previous button state
	uint8_t flag_function_updatePushed : 1;		//Default = 1; Update pushed button state
	uint8_t flag_function_updateReleased : 1;	//Default = 1; Update released button state
	uint8_t flag_logic_idle : 1;				//Default = 1; The state in which the button is idle
	uint8_t flag_state_current : 1;				//Current state of the button
	uint8_t flag_state_previous : 1;			//Previous state of the button
	uint8_t flag_state_pushed : 1;				//Pushed state of the button
	uint8_t flag_state_released : 1;			//Released state of the button
	uint8_t data_shift_portBit : 3;				//The port bit on which the button is located
uint8_t: 0;
	volatile uint8_t *data_port_p;				//The port on which the button is located
}Button;

//Clear button contents to 0
void button_clear(Button *const button);
//Setup button according to default values
void button_defaultSetup(Button *const button);	//Maybe defaultInit?
//Update the buttons values
void button_update(Button *const button);

#ifdef __cplusplus
}
#endif
