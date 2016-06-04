#pragma once

#ifndef __AVR_ATtiny84__
#define __AVR_ATtiny84__
#endif

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum Timer_Attribute_e {
		Timer_Select,
		Timer_ClockSelect,
		Timer_Mode,
		Timer_CompareOutputMode,
	} Timer_Attribute_e;

	typedef enum Timer_Select_e {
		Timer_8_0,
		Timer_16_0,
	} Timer_Select_e;

	typedef enum Timer_ClockSelect_e {
		Timer_ClockSelect_None,
		Timer_ClockSelect_Prescale1,
		Timer_ClockSelect_Prescale8,
		Timer_ClockSelect_Prescale64,
		Timer_ClockSelect_Prescale256,
		Timer_ClockSelect_Prescale1024,
		Timer_ClockSelect_ExternalFallingEdge,
		Timer_ClockSelect_ExternalRisingEdge,
	} Timer_ClockSelect_e;

	typedef enum Timer_Mode_e {
		Timer_Mode_Normal,						//1,0; Top = 0xff or 0xffff, update OCRx at Immediate, TOV flag set on MAX

		Timer_Mode_CTC_TopOCRA,					//1,0; Top = OCRA, update OCRx at Immediate, TOV flag set on MAX
		Timer_Mode_CTC_TopICR,					//1; Top = ICR, update OCRx at Immediate, TOV flag set on MAX
		
		Timer_Mode_FastPWM_TopFF_tovMAX,		//0; Top = 0xff, update OCRx at BOTTTOM, TOV flag set on MAX
		Timer_Mode_FastPWM_TopFF_tovTOP,		//1; Top = 0xff, update OCRx at TOP, TOV flag set on TOP
		Timer_Mode_FastPWM_Top1FF,				//1; Top = 0x1ff, update OCRx at TOP, TOV flag set on TOP
		Timer_Mode_FastPWM_Top3FF,				//1; Top = 0x3ff, update OCRx at TOP, TOV flag set on TOP
		Timer_Mode_FastPWM_TopOCRA_udtBOTTOM,	//0; Top = OCRA, update OCRx at BOTTOM, TOV flag set on TOP
		Timer_Mode_FastPWM_TopOCRA_utdTOP,		//1; Top = OCRA, update OCRx at TOP, TOV flag set on BOTTOM
		Timer_Mode_FastPWM_TopICR,				//1; Top = ICR, update OCRx at TOP, TOV flag set on TOP
		
		Timer_Mode_PhaseCorrectPWM_TopFF,		//1,0; Top = 0xff, update OCRx at TOP, TOV flag set on BOTTOM
		Timer_Mode_PhaseCorrectPWM_Top1FF,		//1; Top = 0x1ff, update OCRx at Immediate, TOV flag set on BOTTOM
		Timer_Mode_PhaseCorrectPWM_Top3FF,		//1; Top = 0x3ff, update OCRx at Immediate, TOV flag set on BOTTOM
		Timer_Mode_PhaseCorrectPWM_TopOCRA,		//1,0; Top = OCRA, update OCRx at TOP, TOV flag set on BOTTOM
		Timer_Mode_PhaseCorrectPWM_TopICR,		//1, Top = ICR, update OCRx at TOP, TOV flag set on BOTTOM
		
		Timer_Mode_PhaseFreqCorrectPWM_TopOCRA,	//1; Top = OCRA, update OCRx at BOTTOM, TOV flag set on BOTTOM
		Timer_Mode_PhaseFreqCorrectPWM_TopICR,	//1; Top = ICR, update OCRx at BOTTOM, TOV flag set on BOTTOM
	} Timer_Mode_e;

	typedef enum Timer_CompareOutputMode_e {
		Timer_CompareOutputMode_Disconnected,

		//Non-PWM Mode
		Timer_CompareOutputMode_ToggleOnCompareMatch,
		Timer_CompareOutputMode_ClearOnCompareMatch,
		Timer_CompareOutputMode_SetOnCompareMatch,

		//Fast PWM Mode
		Timer_CompareOutputMode_ClearOnCompareMatch_SetAtBottom,
		Timer_CompareOutputMode_SetOnCompareMatch_ClearAtBottom,

		//Phase Correct PWM Mode
		Timer_CompareOutputMode_ClearOnCompareMatchUp_SetOnCompareMatchDown,
		Timer_CompareOutputMode_SetOnCompareMatchUp_ClearOnCompareMatchDown,

	} Timer_CompareOutputMode_e;

	typedef struct Timer {
		Timer_Select_e select;
		Timer_ClockSelect_e clockSelect;
		Timer_Mode_e mode;
		Timer_CompareOutputMode_e compareOutputMode;

	} Timer;

	void timer_setAttribute(Timer_Select_e const timer, Timer_Attribute_e const attribute, int const value);
	void timer_set_clockSelect(Timer_Select_e const timer, Timer_ClockSelect_e const value);
	void timer_set_mode(Timer_Select_e const timer, Timer_Mode_e value);
	void timer_set_compareOutputMode(Timer_Select_e const timer, Timer_CompareOutputMode_e const value);
	
	int timer_getAttribute(Timer_Select_e const timer, Timer_Attribute_e const attribute);
	Timer_ClockSelect_e timer_get_clockSelect(Timer_Select_e const timer);
	Timer_Mode_e timer_get_mode(Timer_Select_e const timer);
	Timer_CompareOutputMode_e timer_get_compareOutputMode(Timer_Select_e const timer);

	void timer_get(Timer *const timer);
	void timer_set(Timer *const timer);

#ifdef __cplusplus
}
#endif
