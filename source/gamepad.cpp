#include "../include/tedavr/gamepad.h"

void Gamepad::gp_reset() {
	for (uint8_t i = 0; i < ButtonCount; i++) {
		button[i] = button_default;
	}

	stick[0].pos_x = 0;
	stick[0].pos_y = 0;
	stick[1].pos_x = 0;
	stick[1].pos_y = 0;
}

void Gamepad::gp_update() {
	for (uint8_t i = 0; i < ButtonCount; i++) {
		if ((button[i].i_analog) && !(button[i].i_digital))
			button[i].d = (button[i].a >= button[i].t_analog);
		if (!(button[i].i_analog) && (button[i].i_digital))
			button[i].a = button[i].d ? button[i].t_analog : 0;
	}
}

Gamepad::Gamepad() {
	//Try some of that fancey struct initialization stuff here	
	button_default.a = 0;
	button_default.d = false;
	button_default.i_analog = false;
	button_default.i_digital = true;
	button_default.t_analog = 0xff;
}