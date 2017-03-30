#pragma once

#include <inttypes.h>
#include <avr/io.h>

#include "timer.h"
#include "../../tedavr_project/servo_define.h"

namespace ns_servo {

	class Parameter {
	public:
		constexpr Parameter(uint8_t const prescale, uint16_t const ocra, uint16_t const prescale_real) : prescale(prescale), ocra(ocra), prescale_real(prescale_real) {}
		uint8_t prescale;
		uint16_t prescale_real;
		uint16_t ocra;
	};

	static constexpr float calculate_ticks(float const ms, float const prescale, float const clock = F_CPU) {
		return(clock / prescale) / (1 / (ms / 1000));
	}
	static constexpr bool determine_prescale_valid(float const ms, float const prescale, float const clock) {
		return((calculate_ticks(ms, prescale, clock) <= 0xffff) && (static_cast<uint64_t>(calculate_ticks(ms, prescale, clock)) > 0));
	}
	static constexpr Parameter determine_parameters(float const ms, float const clock = F_CPU) {
		return(Parameter(static_cast<uint8_t>(
			determine_prescale_valid(ms, 1, clock) ? 0b001 :
			determine_prescale_valid(ms, 8, clock) ? 0b010 :
			determine_prescale_valid(ms, 64, clock) ? 0b011 :
			determine_prescale_valid(ms, 256, clock) ? 0b100 :
			determine_prescale_valid(ms, 1024, clock) ? 0b101 :
			0),
			static_cast<uint16_t>(
				determine_prescale_valid(ms, 1, clock) ? calculate_ticks(ms, 1, clock) :
				determine_prescale_valid(ms, 8, clock) ? calculate_ticks(ms, 8, clock) :
				determine_prescale_valid(ms, 64, clock) ? calculate_ticks(ms, 64, clock) :
				determine_prescale_valid(ms, 256, clock) ? calculate_ticks(ms, 256, clock) :
				determine_prescale_valid(ms, 1024, clock) ? calculate_ticks(ms, 1024, clock) :
				0),
			static_cast<uint16_t>(
				determine_prescale_valid(ms, 1, clock) ? 1 :
				determine_prescale_valid(ms, 8, clock) ? 8 :
				determine_prescale_valid(ms, 64, clock) ? 64 :
				determine_prescale_valid(ms, 256, clock) ? 256 :
				determine_prescale_valid(ms, 1024, clock) ? 1024 :
				0)));
	}
	static constexpr Parameter param = determine_parameters(SERVO_INTERVAL, F_CPU);

	static constexpr float calculate_ticks_angle(float const degree, float const time_min = SERVO_MIN, float const time_max = SERVO_MAX, float const degree_range = SERVO_DEGREERANGE, float const prescale = param.prescale_real, float const clock = F_CPU) {
		return(calculate_ticks(static_cast<float>((((time_max - time_min) / degree_range) * degree) + time_min), prescale, clock));
	}
	void init();
}

class Servo {
public:
	void loop();
	void update();
	void move(float position, float time = 0, float step = 0);

	Timer timer;
	bool moving = false;
	bool negative = false;
	float c_time;
	float c_pos = 0;
	float d_pos;
	float c_step;
};
