#include "../include/tedavr/servo.h"

//Init
#include "../../tedavr_project/servo_init.cpp"

void Servo::loop() {
	if (moving) {
		if (timer) {
			//Add the step (c_step should be negative if moving in the negative direction)
			c_pos += c_step;
			//If a negative compare
			if (negative) {
				if (c_pos <= d_pos)
					moving = false;
			}
			//If a positive compare
			else {
				if (c_pos >= d_pos)
					moving = false;
			}
			if (moving) {
				update();
				timer.reset();
				timer = static_cast<uint64_t>(c_time);
				timer.start();
			}
			else {
				c_pos = d_pos;
				update();
				timer.reset();
			}
		}
	}
}

void Servo::update() {
#ifdef __AVR_ATtiny84__
	OCR0B = calculate_ticks_angle(c_pos);
#elif __AVR_ATmega8__
	OCR1B = ns_servo::calculate_ticks_angle(c_pos);
#endif
}

void Servo::move(float position, float time, float step) {
	//step = How many degrees it should 'step' by during time

	//Set the destination to pos
	d_pos = position;
	//Work out the difference (how many degrees need to be traveled)
	float diff_signed = position - c_pos;
	//Determine whether it is going in the negative direction
	negative = diff_signed < 0;
	//Create an unsigned/positive version for the following calculation
	float diff_unsigned = negative ? -diff_signed : diff_signed;
	//If step was not assigned
	if (step == 0)
		//Set step to difference, so it does it all at once
		step = diff_unsigned;
	//Work out the time interval between each step
	c_time = time / (diff_unsigned / step);
	//Assign current step with a sign so that the servo can move in both directions
	c_step = negative ? -step : step;

	//Set the timer to the time value
	timer.reset();
	timer = static_cast<uint64_t>(c_time);
	timer.start();
	//Set moving to true
	moving = true;
}
