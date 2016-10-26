#pragma once

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#ifndef __INTELLISENSE__
#include <util/atomic.h>
#endif

//Device specific things
#include "../../device/timer.h"

class Timer;

namespace timer {
	struct Parameter {
		constexpr Parameter() {}
		constexpr Parameter(uint8_t const nprescale, uint8_t const nhome, uint8_t const nloop) : prescale(nprescale), home(nhome), loop(nloop) {}
		uint8_t prescale = 0;
		uint8_t home = 0;
		size_t loop = 0;
	};
	constexpr double determine_cycles(double const prescale, double const interval, double const cpu_freq) {
		return(interval / (1 / (cpu_freq / prescale)));
	}
	constexpr double determine_home(double const prescale, double const interval, double const cpu_freq) {
		return(static_cast<uint64_t>(determine_cycles(prescale, interval, cpu_freq)) % 0xff);
	}
	constexpr double determine_loop(double const prescale, double const interval, double const cpu_freq) {
		return(determine_cycles(prescale, interval, cpu_freq) / 0xff);
	}
	constexpr bool determine_prescale_valid(double const prescale, double const interval, double const cpu_freq) {
		return((static_cast<uint64_t>(determine_cycles(prescale, interval, cpu_freq)) > 0) ? true : false);
	}
	constexpr Parameter determine_parameters(double const interval = 0.001, double const cpu_freq = F_CPU) {
		return(Parameter(static_cast<uint8_t>(
			determine_prescale_valid(1024, interval, cpu_freq) ? 0b101 :
			determine_prescale_valid(256, interval, cpu_freq) ? 0b100 :
			//determine_prescale_valid(128, interval, cpu_freq) ? 0b101 :
			determine_prescale_valid(64, interval, cpu_freq) ? 0b011 :
			//(32, interval, cpu_freq) ? 0b011 :
			determine_prescale_valid(8, interval, cpu_freq) ? 0b010 :
			determine_prescale_valid(1, interval, cpu_freq) ? 0b001 :
			0), static_cast<uint8_t>(
				determine_prescale_valid(1024, interval, cpu_freq) ? determine_home(1024, interval, cpu_freq) :
				determine_prescale_valid(256, interval, cpu_freq) ? determine_home(256, interval, cpu_freq) :
				//determine_prescale_valid(128, interval, cpu_freq) ? determine_home(128, interval, cpu_freq) :
				determine_prescale_valid(64, interval, cpu_freq) ? determine_home(64, interval, cpu_freq) :
				//determine_prescale_valid(32, interval, cpu_freq) ? determine_home(32, interval, cpu_freq) :
				determine_prescale_valid(8, interval, cpu_freq) ? determine_home(8, interval, cpu_freq) :
				determine_prescale_valid(1, interval, cpu_freq) ? determine_home(1, interval, cpu_freq) :
				0), static_cast<size_t>(
					determine_prescale_valid(1024, interval, cpu_freq) ? determine_loop(1024, interval, cpu_freq) :
					determine_prescale_valid(256, interval, cpu_freq) ? determine_loop(256, interval, cpu_freq) :
					//determine_prescale_valid(128, interval, cpu_freq) ? determine_loop(128, interval, cpu_freq) :
					determine_prescale_valid(64, interval, cpu_freq) ? determine_loop(64, interval, cpu_freq) :
					//determine_prescale_valid(32, interval, cpu_freq) ? determine_loop(32, interval, cpu_freq) :
					determine_prescale_valid(8, interval, cpu_freq) ? determine_loop(8, interval, cpu_freq) :
					determine_prescale_valid(1, interval, cpu_freq) ? determine_loop(1, interval, cpu_freq) :
					0)));
	}
	struct Runtime {
		static constexpr timer::Parameter param = timer::determine_parameters(TIMER_INTERVAL);
		size_t loop_index = 0;
		bool loop_remainder = false;
		size_t timer_amount = 0;
		//Array of pointers
		Timer **timer_buf = nullptr;
	};
	void init();
	void next_tick();
	void add(Timer *const ntimer);
	void remove(Timer const *const ntimer);
	size_t find(Timer const *const ntimer);
	void tick();
}

class Timer {
public:
	//Returns whether the timer is finished or not
	operator bool() const;
	//Returns the timers ticks
	operator uint64_t() const;
	//Subtract a tick from the timer
	virtual void decrement();
	//Assign timer ticks
	Timer &operator=(uint64_t const p0);

	//Starts the timer
	virtual void start();
	//Stops (pauses) the timer
	virtual void stop();
	//Resets (sets everything to default value) the timer
	void reset();

	//Adds the timer to the timer management
	Timer();
	//Removes the timer from the timer management
	~Timer();

	bool finished = false;
	bool running = false;
	uint64_t ticks = 0;
};

class Stopwatch : public Timer {
public:
	void decrement() override;
	void start() override;
};
