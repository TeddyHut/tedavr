#include "../include/tedavr/timer.h"

//Probs should declare volatile
//TODO: Work out a better way to do this, so that it can be used from main
static timer::Runtime runtime;

#ifndef __INTELLISENSE__
ISR(TEDAVR_TIMER_EXPRESSION_VECT_OVERFLOWINTERRUPT) {
	if (runtime.loop_index == runtime.param.loop) {
		if (runtime.loop_remainder) {
			timer::tick();
			timer::next_tick();
		}
		else {
			runtime.loop_remainder = true;
			TEDAVR_TIMER_EXPRESSION_TIMERVAR = TEDAVR_TIMER_EXPRESSION_TIMERVAR_MAX - runtime.param.home;
		}
	}
	else {
		runtime.loop_index++;
	}
}
#endif

void timer::init() {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		next_tick();
		TEDAVR_TIMER_EXPRESSION_PRESCALEVAR &= ~0b111;
		TEDAVR_TIMER_EXPRESSION_PRESCALEVAR |= runtime.param.prescale;
		TEDAVR_TIMER_EXPRESSION_ENABLEOVRERFLOWINTERRUPT;
#ifndef __INTELLISENSE__
}
#endif
	}

void timer::next_tick() {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		//If it needs to do a loop
		if (runtime.param.loop != 0) {
			runtime.loop_index = 0;
			runtime.loop_remainder = false;
		}
		//And if it's going straight to the remainder
		else {
			runtime.loop_remainder = true;
			TEDAVR_TIMER_EXPRESSION_TIMERVAR = TEDAVR_TIMER_EXPRESSION_TIMERVAR_MAX - runtime.param.home;
		}
#ifndef __INTELLISENSE__
	}
#endif
}

void timer::add(Timer *const ntimer) {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		runtime.timer_amount++;
		runtime.timer_buf = static_cast<Timer **>(realloc(static_cast<void *>(runtime.timer_buf), sizeof(Timer *) * runtime.timer_amount));
		runtime.timer_buf[runtime.timer_amount - 1] = ntimer;
#ifndef __INTELLISENSE__
	}
#endif
}

void timer::remove(Timer const *const ntimer) {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		size_t pos = find(ntimer);
		memcpy(runtime.timer_buf + pos, runtime.timer_buf + pos + 1, runtime.timer_amount - pos - 1);
		runtime.timer_amount--;
		runtime.timer_buf = static_cast<Timer **>(realloc(static_cast<void *>(runtime.timer_buf), sizeof(Timer *) * runtime.timer_amount));
#ifndef __INTELLISENSE__
	}
#endif
}

size_t timer::find(Timer const *const ntimer) {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		for (size_t i = 0; i < runtime.timer_amount; i++) {
			if (ntimer == runtime.timer_buf[i])
				return(i);
		}
#ifndef __INTELLISENSE__
	}
#endif
	return(0);
}

void timer::tick() {
#ifndef __INTELLISENSE__
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
		for (size_t i = 0; i < runtime.timer_amount; i++) {
			runtime.timer_buf[i]->decrement();
		}
#ifndef __INTELLISENSE__
	}
#endif
}

Timer::operator bool() const {
	return(finished);
}

Timer::operator uint64_t() const {
	return(ticks);
}

void Timer::decrement() {
	if (running) {
		ticks--;
		if (ticks == 0) {
			running = false;
			finished = true;
		}
	}
}

Timer &Timer::operator=(uint64_t const p0) {
	ticks = p0;
	return(*this);
}

void Timer::start() {
	if (ticks == 0) {
		running = false;
		finished = true;
	}
	else {
		running = true;
		finished = false;
	}
}

void Timer::stop() {
	running = false;
}

void Timer::reset() {
	finished = false;
	running = false;
	ticks = 0;
}

Timer::Timer() {
	timer::add(this);
}

Timer::~Timer() {
	timer::remove(this);
}

void Stopwatch::decrement() {
	if (running) {
		ticks++;
	}
}

void Stopwatch::start() {
	running = true;
}
