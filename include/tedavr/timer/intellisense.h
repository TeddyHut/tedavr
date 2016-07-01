#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum Timer_Parameter_e {
		Timer_Select,
		Timer_ClockSelect,
		Timer_Mode,
		Timer_CompareOutputMode,
	} Timer_Parameter_e;

	typedef enum Timer_Select_e {
		Timer_8_0,
		Timer_8_1,
		Timer_16_0,
	} Timer_e;

	typedef enum Timer_ClockSelect_e {
		Timer_ClockSelect_None,
		Timer_ClockSelect_Prescale1,
		Timer_ClockSelect_Prescale8,
		Timer_ClockSelect_Prescale64,
		Timer_ClockSelect_Prescale256,
		Timer_ClockSelect_Prescale1024,
		Timer_ClockSelect_ExternalFallingEdge,
		Timer_ClockSelect_ExternalRisingEdge,
	} Timer_Prescale_e;

	typedef enum Timer_Mode_e {
		Timer_Mode_Normal,					//Top = 0xff, update OCRx at Immediate, TOV flag set on MAX
		Timer_Mode_CTC,						//Top = OCRA, update OCRx at Immediate, TOV flag set on MAX
		Timer_Mode_FastPWM_TopFF,			//Top = 0xff, update OCRx at BOTTTOM, TOV flag set on MAX
		Timer_Mode_FastPWM_TopOCRA,			//Top = OCRA, update OCRx at BOTTOM, TOV flag set on TOP
		Timer_Mode_PhaseCorrectPWMTopFF,	//Top = 0xff, update OCRx at TOP, TOV flag set on BOTTOM
		Timer_Mode_PhaseCorrectPWMTopOCRA,	//Top = OCRA, update OCRx at TOP, TOV flat set on BOTTOM
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

	void timer_get(Timer *const timer);
	void timer_set(Timer *const timer);

#ifdef __cplusplus
}
#endif
