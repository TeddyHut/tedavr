#include "../../include/tedavr/timer/avr_attiny84.h"

//TODO: Just make it so that there are functions for setting the bits of these SFRs.

void timer_setAttribute(Timer_Select_e const timer, Timer_Attribute_e const attribute, int const value) {
	switch (attribute) {
	case Timer_ClockSelect:
		timer_set_clockSelect(timer, value);
		break;
	case Timer_Mode:
		timer_set_mode(timer, value);
		break;
	case Timer_CompareOutputMode:
		timer_set_compareOutputMode(timer, value);
		break;
	default:
		break;
	}
}

void timer_set_clockSelect(Timer_Select_e const timer, Timer_ClockSelect_e const value) {
	switch (timer) {
	case Timer_8_0:
		switch (value) {
		case Timer_ClockSelect_None:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			break;
		case Timer_ClockSelect_Prescale1:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000001;	//CS00 = 1
			break;
		case Timer_ClockSelect_Prescale8:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000010;	//CS01 = 1
			break;
		case Timer_ClockSelect_Prescale64:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000011;	//CS01 = 1, CS00 = 1
			break;
		case Timer_ClockSelect_Prescale256:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000100;	//CS02 = 1
			break;
		case Timer_ClockSelect_Prescale1024:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000101;	//CS02 = 1, CS00 = 1
			break;
		case Timer_ClockSelect_ExternalFallingEdge:
			TCCR0B &= 0b11111000;	//CS02 = 0, CS01 = 0, CS00 = 0
			TCCR0B |= 0b00000110;	//CS02 = 1, CS01 = 1
			break;
		case Timer_ClockSelect_ExternalRisingEdge:
			TCCR0B |= 0b00000111;	//CS02 = 1, CS01 = 1, CS00 = 1
			break;
		default:
			break;
		}
	case Timer_16_0:
		switch (value) {
		case Timer_ClockSelect_None:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			break;
		case Timer_ClockSelect_Prescale1:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000001;	//CS10 = 1
			break;
		case Timer_ClockSelect_Prescale8:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000010;	//CS11 = 1
			break;
		case Timer_ClockSelect_Prescale64:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000011;	//CS11 = 1, CS10 = 1
			break;
		case Timer_ClockSelect_Prescale256:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000100;	//CS12 = 1
			break;
		case Timer_ClockSelect_Prescale1024:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000101;	//CS12 = 1, CS10 = 1
			break;
		case Timer_ClockSelect_ExternalFallingEdge:
			TCCR1B &= 0b11111000;	//CS12 = 0, CS11 = 0, CS10 = 0
			TCCR1B |= 0b00000110;	//CS12 = 1, CS11 = 1
			break;
		case Timer_ClockSelect_ExternalRisingEdge:
			TCCR1B |= 0b00000111;	//CS12 = 1, CS11 = 1, CS10 = 1
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void timer_set_mode(Timer_Select_e const timer, Timer_Mode_e value) {
	switch (timer) {
	case Timer_8_0:
		switch (value) {
		case Timer_Mode_Normal:
			TCCR0A &= 0b11111100;	//WGM01 = 0, WGM00 = 0
			TCCR0B &= 0b11110111;	//WGM02 = 0
			break;
		case Timer_Mode_CTC_TopOCRA:
			TCCR0A &= 0b11111100;	//WGM01 = 0, WGM00 = 0
			TCCR0A |= 0b00000010;	//WGM01 = 1
			TCCR0B &= 0b11110111;	//WGM02 = 0
			break;
		case Timer_Mode_FastPWM_TopFF_tovMAX:
			TCCR0A |= 0b00000011;	//WGM01 = 1, WGM00 = 1
			TCCR0B &= 0b11110111;	//WGM02 = 0
			break;
		case Timer_Mode_FastPWM_TopOCRA_udtBOTTOM:
			TCCR0A |= 0b00000011;	//WGM01 = 1, WGM00 = 1
			TCCR0B |= 0b00001000;	//WGM02 = 1
			break;
		case Timer_Mode_PhaseCorrectPWM_TopFF:
			TCCR0A &= 0b11111100;	//WGM01 = 0, WGM00 = 0
			TCCR0A |= 0b00000001;	//WGM00 = 1
			TCCR0B &= 0b11110111;	//WGM02 = 0
			break;
		case Timer_Mode_PhaseCorrectPWM_TopOCRA:
			TCCR0A &= 0b11111100;	//WGM01 = 1, WGM00 = 0
			TCCR0A |= 0b00000001;	//WGM00 = 1
			TCCR0B |= 0b00001000;	//WGM02 = 1
			break;
		default:
			break;
		}
	case Timer_16_0:
		switch (value) {
		case Timer_Mode_Normal:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1B &= 0b11100111;	//WGM13 - 0, WGM12 = 0
			break;
		case Timer_Mode_CTC_TopOCRA:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00001000;	//WGM12 = 1
			break;
		case Timer_Mode_CTC_TopICR:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1B |= 0b00011000;	//WGM13 = 1, WGM12 = 1
			break;
		case Timer_Mode_FastPWM_TopFF_tovTOP:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000001;	//WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00001000;	//WGM12 = 1
			break;
		case Timer_Mode_FastPWM_Top1FF:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000010;	//WGM11 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00001000;	//WGM12 = 1
			break;
		case Timer_Mode_FastPWM_Top3FF:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000011;	//WGM11 = 1, WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00001000;	//WGM12 = 1
			break;
		case Timer_Mode_FastPWM_TopOCRA_utdTOP:
			TCCR1A |= 0b00000011;	//WGM11 = 1, WGM10 = 1
			TCCR1B |= 0b00011000;	//WGM13 = 1, WGM12 = 1
			break;
		case Timer_Mode_FastPWM_TopICR:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000010;	//WGM11 = 1
			TCCR1B |= 0b00011000;	//WGM13 = 1, WGM12 = 1
			break;
		case Timer_Mode_PhaseCorrectPWM_TopFF:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000001;	//WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			break;
		case Timer_Mode_PhaseCorrectPWM_Top1FF:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000010;	//WGM11 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			break;
		case Timer_Mode_PhaseCorrectPWM_Top3FF:
			TCCR1A |= 0b00000011;	//WGM11 = 1, WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			break;
		case Timer_Mode_PhaseCorrectPWM_TopOCRA:
			TCCR1A |= 0b00000011;	//WGM11 = 1, WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00010000;	//WGM13 = 1
			break;
		case Timer_Mode_PhaseCorrectPWM_TopICR:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000010;	//WGM11 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00010000;	//WGM13 = 1
			break;
		case Timer_Mode_PhaseFreqCorrectPWM_TopOCRA:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1A |= 0b00000001;	//WGM10 = 1
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00010000;	//WGM13 = 1
			break;
		case Timer_Mode_PhaseFreqCorrectPWM_TopICR:
			TCCR1A &= 0b11111100;	//WGM11 = 0, WGM10 = 0
			TCCR1B &= 0b11100111;	//WGM13 = 0, WGM12 = 0
			TCCR1B |= 0b00010000;	//WGM13 = 1
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void timer_set_compareOutputMode(Timer_Select_e const timer, Timer_CompareOutputMode_e const value) {
}

int timer_getAttribute(Timer_Select_e const timer, Timer_Attribute_e const attribute) {
	switch (attribute) {
	case Timer_ClockSelect:
		return(timer_get_clockSelect(timer));
		break;
	case Timer_Mode:
		return(timer_get_mode(timer));
		break;
	case Timer_CompareOutputMode:
		return(timer_get_compareOutputMode(timer));
		break;
	default:
		break;
	}
}

Timer_ClockSelect_e timer_get_clockSelect(Timer_Select_e const timer) {
}

Timer_Mode_e timer_get_mode(Timer_Select_e const timer) {
}

Timer_CompareOutputMode_e timer_get_compareOutputMode(Timer_Select_e const timer) {
}
