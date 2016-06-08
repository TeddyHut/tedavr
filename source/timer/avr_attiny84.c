#include "../../include/tedavr/timer/avr_attiny84.h"

#define Timer_ClockSelect_None_b 0
#define Timer_ClockSelect_Prescale1_b 1
#define Timer_ClockSelect_Prescale8_b 2
#define Timer_ClockSelect_Prescale64_b 3
#define Timer_ClockSelect_Prescale256_b 4
#define Timer_ClockSelect_Prescale1024_b 5
#define Timer_ClockSelect_ExternalFallingEdge_b 6
#define Timer_ClockSelect_ExternalRisingEdge_b 7

#define Timer_0_Mode_Normal_d 0
#define Timer_0_Mode_CTC_TopOCRA_d 2
#define Timer_0_Mode_FastPWM_TopFF_tovMAX_d 3
#define Timer_0_Mode_FastPWM_TopOCRA_udtBOTTOM_d 7
#define Timer_0_Mode_PhaseCorrectPWM_TopFF_d 1
#define Timer_0_Mode_PhaseCorrectPWM_TopOCRA_d 5

#define Timer_1_Mode_Normal_d			//1,0; Top = 0xff or 0xffff, update OCRx at Immediate, TOV flag set on MAX
#define Timer_1_Mode_CTC_TopOCRA_d		//1,0; Top = OCRA, update OCRx at Immediate, TOV flag set on MAX
#define Timer_1_Mode_CTC_TopICR_d,					//1; Top = ICR, update OCRx at Immediate, TOV flag set on MAX
#define Timer_1_Mode_FastPWM_TopFF_tovTOP_d,		//1; Top = 0xff, update OCRx at TOP, TOV flag set on TOP
Timer_Mode_FastPWM_Top1FF,				//1; Top = 0x1ff, update OCRx at TOP, TOV flag set on TOP
Timer_Mode_FastPWM_Top3FF,				//1; Top = 0x3ff, update OCRx at TOP, TOV flag set on TOP
Timer_Mode_FastPWM_TopOCRA_utdTOP,		//1; Top = OCRA, update OCRx at TOP, TOV flag set on BOTTOM
Timer_Mode_FastPWM_TopICR,				//1; Top = ICR, update OCRx at TOP, TOV flag set on TOP
Timer_Mode_PhaseCorrectPWM_TopFF,		//1,0; Top = 0xff, update OCRx at TOP, TOV flag set on BOTTOM
Timer_Mode_PhaseCorrectPWM_Top1FF,		//1; Top = 0x1ff, update OCRx at Immediate, TOV flag set on BOTTOM
Timer_Mode_PhaseCorrectPWM_Top3FF,		//1; Top = 0x3ff, update OCRx at Immediate, TOV flag set on BOTTOM
Timer_Mode_PhaseCorrectPWM_TopOCRA,		//1,0; Top = OCRA, update OCRx at TOP, TOV flag set on BOTTOM
Timer_Mode_PhaseCorrectPWM_TopICR,		//1, Top = ICR, update OCRx at TOP, TOV flag set on BOTTOM
Timer_Mode_PhaseFreqCorrectPWM_TopOCRA,	//1; Top = OCRA, update OCRx at BOTTOM, TOV flag set on BOTTOM
Timer_Mode_PhaseFreqCorrectPWM_TopICR,	//1; Top = ICR, update OCRx at BOTTOM, TOV flag set on BOTTOM


static void timer_0_set_clockSelect_bits(uint8_t const value);
static void timer_1_set_clockSelect_bits(uint8_t const value);
static void timer_0_set_mode_bits(uint8_t const value);
static void timer_1_set_mode_bits(uint8_t const value);
static void timer_0_set_compareOutputMode_A_bits(uint8_t const value);
static void timer_0_set_compareOutputMode_B_bits(uint8_t const value);
//static void timer_1_set_compareOutputMode_A_bits(uint8_t const value);
//static void timer_1_set_compareOutputMode_B_bits(uint8_t const value);
static void timer_0_set_compareOutputMode_bits(Timer_OutputComparePin_e const pin, uint8_t const value);
static uint8_t timer_0_get_clockSelect_bits(void);
static uint8_t timer_1_get_clockSelect_bits(void);
static uint8_t timer_0_get_mode_bits(void);
//static uint8_t timer_0_get_compareOutputMode_A_bits(void);
//static uint8_t timer_0_get_compareOutputMode_B_bits(void);
//static uint8_t timer_0_get_compareOutputMode_bits(void);

static void timer_0_set_clockSelect_bits(uint8_t const value) {
	TCCR0B &= 0b11111000;
	TCCR0B |= (value & 0b00000111);
}

static void timer_1_set_clockSelect_bits(uint8_t const value) {
	TCCR1B &= 0b11111000;
	TCCR1B |= (value & 0b00000111);
}

void timer_set_clockSelect(Timer_Select_e const timer, Timer_ClockSelect_e const value) {
	//This could just be pulled out of the enum.
	uint8_t bits;
	switch (value) {
	case Timer_ClockSelect_None:
		bits = Timer_ClockSelect_None_b;
		break;
	case Timer_ClockSelect_Prescale1:
		bits = Timer_ClockSelect_Prescale1_b;
		break;
	case Timer_ClockSelect_Prescale8:
		bits = Timer_ClockSelect_Prescale8_b;
		break;
	case Timer_ClockSelect_Prescale64:
		bits = Timer_ClockSelect_Prescale64_b;
		break;
	case Timer_ClockSelect_Prescale256:
		bits = Timer_ClockSelect_Prescale256_b;
		break;
	case Timer_ClockSelect_Prescale1024:
		bits = Timer_ClockSelect_Prescale1024_b;
		break;
	case Timer_ClockSelect_ExternalFallingEdge:
		bits = Timer_ClockSelect_ExternalFallingEdge_b;
		break;
	case Timer_ClockSelect_ExternalRisingEdge:
		bits = Timer_ClockSelect_ExternalRisingEdge_b;
		break;
	default:
		bits = 0;
		break;
	}
	switch (timer) {
	case Timer_8_0:
		timer_0_set_clockSelect_bits(bits);
	case Timer_16_0:
		timer_1_set_clockSelect_bits(bits);
	default:
		break;
	}
}

static void timer_0_set_mode_bits(uint8_t const value) {
	TCCR0A &= 0b11111100;
	TCCR0A |= (value & 0b00000011);
	TCCR0B &= 0b11110111;
	TCCR0B |= ((value & 0b00000100) << 1);
}

static void timer_1_set_mode_bits(uint8_t const value) {
	TCCR1A &= 0b11111100;
	TCCR1A |= (value & 0b00000011);
	TCCR1B &= 0b11100111;
	TCCR1B |= ((value & 0b00001100) << 1);
}

void timer_set_mode(Timer_Select_e const timer, Timer_Mode_e value) {
	switch (timer) {
	case Timer_8_0:
		switch (value) {
		case Timer_Mode_Normal:
			timer_0_set_mode_bits(Timer_0_Mode_Normal_d);
			break;
		case Timer_Mode_CTC_TopOCRA:
			timer_0_set_mode_bits(Timer_0_Mode_CTC_TopOCRA_d);
			break;
		case Timer_Mode_FastPWM_TopFF_tovMAX:
			timer_0_set_mode_bits(Timer_0_Mode_FastPWM_TopFF_tovMAX_d);
			break;
		case Timer_Mode_FastPWM_TopOCRA_udtBOTTOM:
			timer_0_set_mode_bits(Timer_0_Mode_FastPWM_TopOCRA_udtBOTTOM_d);
			break;
		case Timer_Mode_PhaseCorrectPWM_TopFF:
			timer_0_set_mode_bits(Timer_0_Mode_PhaseCorrectPWM_TopFF_d);
			break;
		case Timer_Mode_PhaseCorrectPWM_TopOCRA:
			timer_0_set_mode_bits(Timer_0_Mode_PhaseCorrectPWM_TopOCRA_d);
			break;
		default:
			break;
		}
	case Timer_16_0:
		switch (value) {
		case Timer_Mode_Normal:
			timer_1_set_mode_bits(0);
			break;
		case Timer_Mode_CTC_TopOCRA:
			timer_1_set_mode_bits(4);
			break;
		case Timer_Mode_CTC_TopICR:
			timer_1_set_mode_bits(12);
			break;
		case Timer_Mode_FastPWM_TopFF_tovTOP:
			timer_1_set_mode_bits(5);
			break;
		case Timer_Mode_FastPWM_Top1FF:
			timer_1_set_mode_bits(6);
			break;
		case Timer_Mode_FastPWM_Top3FF:
			timer_1_set_mode_bits(7);
			break;
		case Timer_Mode_FastPWM_TopOCRA_utdTOP:
			timer_1_set_mode_bits(15);
			break;
		case Timer_Mode_FastPWM_TopICR:
			timer_1_set_mode_bits(14);
			break;
		case Timer_Mode_PhaseCorrectPWM_TopFF:
			timer_1_set_mode_bits(1);
			break;
		case Timer_Mode_PhaseCorrectPWM_Top1FF:
			timer_1_set_mode_bits(2);
			break;
		case Timer_Mode_PhaseCorrectPWM_Top3FF:
			timer_1_set_mode_bits(3);
			break;
		case Timer_Mode_PhaseCorrectPWM_TopOCRA:
			timer_1_set_mode_bits(11);
			break;
		case Timer_Mode_PhaseCorrectPWM_TopICR:
			timer_1_set_mode_bits(10);
			break;
		case Timer_Mode_PhaseFreqCorrectPWM_TopOCRA:
			timer_1_set_mode_bits(9);
			break;
		case Timer_Mode_PhaseFreqCorrectPWM_TopICR:
			timer_1_set_mode_bits(8);
			break;
		default:
			break;
		}
	default:
		break;
	}
}

static void timer_0_set_compareOutputMode_A_bits(uint8_t const value) {
	TCCR0A &= 0b00111111;
	TCCR0A |= ((value & 0b00000011) << 6);
}

static void timer_0_set_compareOutputMode_B_bits(uint8_t const value) {
	TCCR0A &= 0b11001111;
	TCCR0A |= ((value & 0b00000011) << 4);
}

/*
static void timer_1_set_compareOutputMode_A_bits(uint8_t const value) {
	TCCR1A &= 0b00111111;
	TCCR1A |= ((value & 0b00000011) << 6);
}

static void timer_1_set_compareOutputMode_B_bits(uint8_t const value) {
	TCCR1A &= 0b11001111;
	TCCR1A |= ((value & 0b00000011) << 4);
}
*/

static void timer_0_set_compareOutputMode_bits(Timer_OutputComparePin_e const pin, uint8_t const value) {
	switch (pin) {
	case Timer_OutputComparePin_A:
		timer_0_set_compareOutputMode_A_bits(value);
		break;
	case Timer_OutputComparePin_B:
		timer_0_set_compareOutputMode_B_bits(value);
		break;
	default:
		break;
	}
}

void timer_set_compareOutputMode(Timer_Select_e const timer, Timer_OutputComparePin_e const pin, Timer_CompareOutputMode_e const value) {
	Timer_ModeGeneral_e mode = timer_get_modeGeneral(timer);
	switch (timer) {
	case Timer_8_0:
		switch (value) {
		case Timer_CompareOutputMode_Disconnected:
			timer_0_set_compareOutputMode_bits(pin, 0);
			break;
		case Timer_CompareOutputMode_ToggleOnCompareMatch:
			if ((mode == Timer_ModeGeneral_Normal) || (mode == Timer_ModeGeneral_CTC))
				timer_0_set_compareOutputMode_bits(pin, 1);
			else if (((mode == Timer_ModeGeneral_FastPWM) || (mode == Timer_ModeGeneral_PhaseCorrectPWM)) && (pin == Timer_OutputComparePin_A)) {
				if (TCCR0B & 0b00001000)	//if WGM02
					timer_0_set_compareOutputMode_A_bits(1);
			}
			break;
		case Timer_CompareOutputMode_ClearOnCompareMatch:
			if ((mode == Timer_ModeGeneral_Normal) || (mode == Timer_ModeGeneral_CTC))
				timer_0_set_compareOutputMode_bits(pin, 2);
			break;
		case Timer_CompareOutputMode_SetOnCompareMatch:
			if ((mode == Timer_ModeGeneral_Normal) || (mode == Timer_ModeGeneral_CTC))
				timer_0_set_compareOutputMode_bits(pin, 3);
			break;
		case Timer_CompareOutputMode_ClearOnCompareMatch_SetAtBottom:
			if (mode == Timer_ModeGeneral_FastPWM)
				timer_0_set_compareOutputMode_bits(pin, 2);
			break;
		case Timer_CompareOutputMode_SetOnCompareMatch_ClearAtBottom:
			if (mode == Timer_ModeGeneral_FastPWM)
				timer_0_set_compareOutputMode_bits(pin, 3);
			break;
		case Timer_CompareOutputMode_ClearOnCompareMatchUp_SetOnCompareMatchDown:
			if (mode == Timer_ModeGeneral_PhaseCorrectPWM)
				timer_0_set_compareOutputMode_bits(pin, 2);
			break;
		case Timer_CompareOutputMode_SetOnCompareMatchUp_ClearOnCompareMatchDown:
			if (mode == Timer_ModeGeneral_PhaseCorrectPWM)
				timer_0_set_compareOutputMode_bits(pin, 3);
			break;
		default:
			break;
		}
	default:
		break;
	}
}

static uint8_t timer_0_get_clockSelect_bits(void) {
	return(TCCR0B & 0b00000111);
}

static uint8_t timer_1_get_clockSelect_bits(void) {
	return(TCCR1B & 0b00000111);
}

Timer_ClockSelect_e timer_get_clockSelect(Timer_Select_e const timer) {
	uint8_t bits;
	switch (timer) {
	case Timer_8_0:
		bits = timer_0_get_clockSelect_bits();
		break;
	case Timer_16_0:
		bits = timer_1_get_clockSelect_bits();
		break;
	default:
		bits = 0;
		break;
	}
	switch (bits) {
	case Timer_ClockSelect_None_b:
		return(Timer_ClockSelect_None);
		break;
	case Timer_ClockSelect_Prescale1_b:
		return(Timer_ClockSelect_Prescale1);
		break;
	case Timer_ClockSelect_Prescale8_b:
		return(Timer_ClockSelect_Prescale8);
		break;
	case Timer_ClockSelect_Prescale64_b:
		return(Timer_ClockSelect_Prescale64);
		break;
	case Timer_ClockSelect_Prescale256_b:
		return(Timer_ClockSelect_Prescale256);
		break;
	case Timer_ClockSelect_Prescale1024_b:
		return(Timer_ClockSelect_Prescale1024);
		break;
	case Timer_ClockSelect_ExternalFallingEdge_b:
		return(Timer_ClockSelect_ExternalFallingEdge);
		break;
	case Timer_ClockSelect_ExternalRisingEdge_b:
		return(Timer_ClockSelect_ExternalRisingEdge);
		break;
	default:
		break;
	}
	return(Timer_ClockSelect_None);
}

static uint8_t timer_0_get_mode_bits(void) {
	return(((TCCR0B & 0b00001000) >> 1) | (TCCR0A & 0b00000011));
}

Timer_Mode_e timer_get_mode(Timer_Select_e const timer) {
	uint8_t bits = timer_0_get_mode_bits();
	switch (bits) {
	case Timer_0_Mode_Normal_d:
		return(Timer_Mode_Normal);
		break;
	case Timer_0_Mode_CTC_TopOCRA_d:
		return(Timer_Mode_CTC_TopOCRA);
		break;
	case Timer_0_Mode_FastPWM_TopFF_tovMAX_d:
		return(Timer_Mode_FastPWM_TopFF_tovMAX);
		break;
	case Timer_0_Mode_FastPWM_TopOCRA_udtBOTTOM_d:
		return(Timer_Mode_FastPWM_TopOCRA_udtBOTTOM);
		break;
	case Timer_0_Mode_PhaseCorrectPWM_TopFF_d:
		return(Timer_Mode_PhaseCorrectPWM_TopFF);
		break;
	case Timer_0_Mode_PhaseCorrectPWM_TopOCRA_d:
		return(Timer_Mode_PhaseCorrectPWM_TopOCRA);
		break;
	default:
		break;
	}
	return(Timer_Mode_Normal);
}

Timer_ModeGeneral_e timer_get_modeGeneral(Timer_Select_e const timer) {
	Timer_Mode_e mode = timer_get_mode(timer);
	switch (mode) {
	case Timer_Mode_Normal:
		return(Timer_ModeGeneral_Normal);
		break;
	case Timer_Mode_CTC_TopOCRA:
		return(Timer_ModeGeneral_CTC);
		break;
	case Timer_Mode_FastPWM_TopFF_tovMAX:
		return(Timer_ModeGeneral_FastPWM);
		break;
	case Timer_Mode_FastPWM_TopOCRA_udtBOTTOM:
		return(Timer_ModeGeneral_FastPWM);
		break;
	case Timer_Mode_PhaseCorrectPWM_TopFF:
		return(Timer_ModeGeneral_PhaseCorrectPWM);
		break;
	case Timer_Mode_PhaseCorrectPWM_TopOCRA:
		return(Timer_ModeGeneral_PhaseCorrectPWM);
		break;
	default:
		break;
	}
	return(Timer_ModeGeneral_Normal);
}

Timer_CompareOutputMode_e timer_get_compareOutputMode(Timer_Select_e const timer, Timer_OutputComparePin_e const pin) {
	return(Timer_CompareOutputMode_Disconnected);
}
