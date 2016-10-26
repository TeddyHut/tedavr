#pragma once

#include <inttypes.h>
#undef ZL	//Something in AVR libc allready had a ZL

class Gamepad {
public:
	static constexpr uint8_t ButtonCount = 19;
	static constexpr uint8_t StickCount = 2;

	struct s_Stick {
		int16_t pos_x;
		int16_t pos_y;
	};

	struct Stick {
		enum e {
			Left = 0, 
			Right
		};
	};

	enum Button {
		A = 0,
		B,
		X,
		Y,
		L1,
		R1,
		L2,
		R2,
		L3,
		R3,
		Up,
		Down,
		Left,
		Right,
		Start,
		Select,
		Home,
		LeftRumble,
		RightRumble,

	};

	struct s_Button {
		bool d : 1;			//Digital value
		bool i_digital : 1;	//Receive/send a digital value through the interface
		bool i_analog : 1;	//Receive/send an analog value through the interface
		uint8_t t_analog;	//The analog threshold in which an analog value will trigger a digital value
		uint8_t a;			//The analog value
	};

	//Resets all buttons to default values and configurations.
	virtual void gp_reset();
	virtual void gp_update();

	Gamepad();

	s_Button button_default;
	s_Button button[ButtonCount];
	s_Stick stick[StickCount];
};
