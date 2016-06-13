#include "../../include/tedavr/timer/avr_attiny84.h"

#define Timer_ClockSelect_size 8
#define Timer_ClockSelect_None_b 0
#define Timer_ClockSelect_Prescale1_b 1
#define Timer_ClockSelect_Prescale8_b 2
#define Timer_ClockSelect_Prescale64_b 3
#define Timer_ClockSelect_Prescale256_b 4
#define Timer_ClockSelect_Prescale1024_b 5
#define Timer_ClockSelect_ExternalFallingEdge_b 6
#define Timer_ClockSelect_ExternalRisingEdge_b 7

#define Timer_0_Mode_size 6
#define Timer_0_Mode_Normal_d 0
#define Timer_0_Mode_CTC_TopOCRA_d 2
#define Timer_0_Mode_FastPWM_TopFF_tovMAX_d 3
#define Timer_0_Mode_FastPWM_TopOCRA_udtBOTTOM_d 7
#define Timer_0_Mode_PhaseCorrectPWM_TopFF_d 1
#define Timer_0_Mode_PhaseCorrectPWM_TopOCRA_d 5

#define Timer_1_Mode_size 15
#define Timer_1_Mode_Normal_d 0
#define Timer_1_Mode_CTC_TopOCRA_d 4
#define Timer_1_Mode_CTC_TopICR_d 12
#define Timer_1_Mode_FastPWM_TopFF_tovTOP_d 5
#define Timer_1_Mode_FastPWM_Top1FF_d 6
#define Timer_1_Mode_FastPWM_Top3FF_d 7
#define Timer_1_Mode_FastPWM_TopOCRA_utdTOP_d 15
#define Timer_1_Mode_FastPWM_TopICR_d 14
#define Timer_1_Mode_PhaseCorrectPWM_TopFF_d 1
#define Timer_1_Mode_PhaseCorrectPWM_Top1FF_d 2
#define Timer_1_Mode_PhaseCorrectPWM_Top3FF_d 3
#define Timer_1_Mode_PhaseCorrectPWM_TopOCRA_d 11
#define Timer_1_Mode_PhaseCorrectPWM_TopICR_d 10
#define Timer_1_Mode_PhaseFreqCorrectPWM_TopOCRA_d 9
#define Timer_1_Mode_PhaseFreqCorrectPWM_TopICR_d 8

#define Tiemr_0_A_CompareOutputMode_size 16
#define Timer_0_A_CompareOutputMode_index_size 6
#define Timer_0_A_CompareOutputMode_NonPWM_Disconnected_d 0
#define Timer_0_A_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d 1
#define Timer_0_A_CompareOutputMode_NonPWM_ClearOnCompareMatch_d 2
#define Timer_0_A_CompareOutputMode_NonPWM_SetOnCompareMatch_d 3
#define Timer_0_A_CompareOutputMode_FastPWM_Disconnected_d 0
#define Timer_0_A_CompareOutputMode_FastPWM_ToggleOnCompareMatch_d 1
#define Timer_0_A_CompareOutputMode_FastPWM_ClearOnCompareMatch_SetAtBottom_d 2
#define Timer_0_A_CompareOutputMode_FastPWM_SetOnCompareMatch_ClearAtBottom_d 3
#define Timer_0_A_CompareOutputMode_PhaseCorrectPWM_Disconnected_d 0
#define Timer_0_A_CompareOutputMode_PhaseCorrectPWM_ToggleOnCompareMatch_d 1
#define Timer_0_A_CompareOutputMode_PhaseCorrectPWM_ClearOnCompareMatchUp_SetOnCompareMatchDown_d 2
#define Timer_0_A_CompareOutputMode_PhaseCorrectPWM_SetOnCompareMatchUp_ClearOnCompareMatchDown_d 3

#define Timer_0_B_CompareOutputMode_size 14
#define Timer_0_B_CompareOutputMode_index_size 4
#define Timer_0_B_CompareOutputMode_NonPWM_Disconnected_d 0
#define Timer_0_B_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d 1
#define Timer_0_B_CompareOutputMode_NonPWM_ClearOnCompareMatch_d 2
#define Timer_0_B_CompareOutputMode_NonPWM_SetOnCompareMatch_d 3
#define Timer_0_B_CompareOutputMode_FastPWM_Disconnected_d 0
#define Timer_0_B_CompareOutputMode_FastPWM_ClearOnCompareMatch_SetAtBottom_d 2
#define Timer_0_B_CompareOutputMode_FastPWM_SetOnCompareMatch_ClearAtBottom_d 3
#define Timer_0_B_CompareOutputMode_PhaseCorrectPWM_Disconnected_d 0
#define Timer_0_B_CompareOutputMode_PhaseCorrectPWM_ClearOnCompareMatchUp_SetOnCompareMatchDown_d 2
#define Timer_0_B_CompareOutputMode_PhaseCorrectPWM_SetOnCompareMatchUp_ClearOnCompareMatchDown_d 3


static uint8_t const array_clockSelect[Timer_ClockSelect_size][2] PROGMEM = {
	{ Timer_ClockSelect_None, Timer_ClockSelect_None_b },
	{ Timer_ClockSelect_Prescale1, Timer_ClockSelect_Prescale1_b },
	{ Timer_ClockSelect_Prescale8, Timer_ClockSelect_Prescale8_b },
	{ Timer_ClockSelect_Prescale64, Timer_ClockSelect_Prescale64_b },
	{ Timer_ClockSelect_Prescale256, Timer_ClockSelect_Prescale256_b },
	{ Timer_ClockSelect_Prescale1024, Timer_ClockSelect_Prescale1024_b },
	{ Timer_ClockSelect_ExternalFallingEdge, Timer_ClockSelect_ExternalFallingEdge_b },
	{ Timer_ClockSelect_ExternalRisingEdge, Timer_ClockSelect_ExternalRisingEdge_b },
};

static uint8_t const array_0_mode[Timer_0_Mode_size][2] PROGMEM = {
	{ Timer_Mode_Normal, Timer_0_Mode_Normal_d },
	{ Timer_Mode_CTC_TopOCRA, Timer_0_Mode_CTC_TopOCRA_d },
	{ Timer_Mode_FastPWM_TopFF_tovMAX, Timer_0_Mode_FastPWM_TopFF_tovMAX_d },
	{ Timer_Mode_FastPWM_TopOCRA_udtBOTTOM, Timer_0_Mode_FastPWM_TopOCRA_udtBOTTOM_d },
	{ Timer_Mode_PhaseCorrectPWM_TopFF, Timer_0_Mode_PhaseCorrectPWM_TopFF_d },
	{ Timer_Mode_PhaseCorrectPWM_TopOCRA, Timer_0_Mode_PhaseCorrectPWM_TopOCRA_d },
};

static uint8_t const array_1_mode[Timer_1_Mode_size][2] PROGMEM = {
	{ Timer_Mode_Normal, Timer_1_Mode_Normal_d },
	{ Timer_Mode_CTC_TopOCRA, Timer_1_Mode_CTC_TopOCRA_d },
	{ Timer_Mode_CTC_TopICR, Timer_1_Mode_CTC_TopICR_d },
	{ Timer_Mode_FastPWM_TopFF_tovTOP, Timer_1_Mode_FastPWM_TopFF_tovTOP_d },
	{ Timer_Mode_FastPWM_Top1FF, Timer_1_Mode_FastPWM_Top1FF_d },
	{ Timer_Mode_FastPWM_Top3FF, Timer_1_Mode_FastPWM_Top3FF_d },
	{ Timer_Mode_FastPWM_TopOCRA_utdTOP, Timer_1_Mode_FastPWM_TopOCRA_utdTOP_d },
	{ Timer_Mode_FastPWM_TopICR, Timer_1_Mode_FastPWM_TopICR_d },
	{ Timer_Mode_PhaseCorrectPWM_TopFF, Timer_1_Mode_PhaseCorrectPWM_TopFF_d },
	{ Timer_Mode_PhaseCorrectPWM_Top1FF, Timer_1_Mode_PhaseCorrectPWM_Top1FF_d },
	{ Timer_Mode_PhaseCorrectPWM_Top3FF, Timer_1_Mode_PhaseCorrectPWM_Top3FF_d },
	{ Timer_Mode_PhaseCorrectPWM_TopOCRA, Timer_1_Mode_PhaseCorrectPWM_TopOCRA_d },
	{ Timer_Mode_PhaseCorrectPWM_TopICR, Timer_1_Mode_PhaseCorrectPWM_TopICR_d },
	{ Timer_Mode_PhaseFreqCorrectPWM_TopOCRA, Timer_1_Mode_PhaseFreqCorrectPWM_TopOCRA_d },
	{ Timer_Mode_PhaseFreqCorrectPWM_TopICR, Timer_1_Mode_PhaseFreqCorrectPWM_TopICR_d },
};

static uint8_t const array_0_A_compareOutputMode_index[Timer_0_A_CompareOutputMode_index_size][3] PROGMEM = {
	{ Timer_ModeGeneral_Normal, 0, 4 },
	{ Timer_ModeGeneral_CTC, 4, 4 },
	{ Timer_ModeGeneral_FastPWM, 8, 3 },
	{ Timer_ModeGeneral_PhaseCorrectPWM, 11, 3 },
	{ Timer_Mode_FastPWM_TopOCRA_udtBOTTOM, 14, 1 },
	{ Timer_Mode_PhaseCorrectPWM_TopOCRA, 15, 1 },
};

static uint8_t const array_0_A_compareOutputMode[Tiemr_0_A_CompareOutputMode_size][2] PROGMEM = {
	{ Timer_CompareOutputMode_Disconnected, Timer_0_A_CompareOutputMode_NonPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_ClearOnCompareMatch_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_SetOnCompareMatch_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_A_CompareOutputMode_NonPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_ClearOnCompareMatch_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch, Timer_0_A_CompareOutputMode_NonPWM_SetOnCompareMatch_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_A_CompareOutputMode_FastPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch_SetAtBottom, Timer_0_A_CompareOutputMode_FastPWM_ClearOnCompareMatch_SetAtBottom_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch_ClearAtBottom, Timer_0_A_CompareOutputMode_FastPWM_SetOnCompareMatch_ClearAtBottom_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_A_CompareOutputMode_PhaseCorrectPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatchUp_SetOnCompareMatchDown, Timer_0_A_CompareOutputMode_PhaseCorrectPWM_ClearOnCompareMatchUp_SetOnCompareMatchDown_d },
	{ Timer_CompareOutputMode_SetOnCompareMatchUp_ClearOnCompareMatchDown, Timer_0_A_CompareOutputMode_PhaseCorrectPWM_SetOnCompareMatchUp_ClearOnCompareMatchDown_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_A_CompareOutputMode_FastPWM_ToggleOnCompareMatch_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_A_CompareOutputMode_PhaseCorrectPWM_ToggleOnCompareMatch_d },
};

static uint8_t const array_0_B_compareOutputMode_index[Timer_0_B_CompareOutputMode_index_size][3] PROGMEM = {
	{ Timer_ModeGeneral_Normal, 0, 4 },
	{ Timer_ModeGeneral_CTC, 4, 4 },
	{ Timer_ModeGeneral_FastPWM, 8, 3 },
	{ Timer_ModeGeneral_PhaseCorrectPWM, 11, 3 },
};

static uint8_t const array_0_B_compareOutputMode[Timer_0_B_CompareOutputMode_size][2] PROGMEM = {
	{ Timer_CompareOutputMode_Disconnected, Timer_0_B_CompareOutputMode_NonPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_ClearOnCompareMatch_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_SetOnCompareMatch_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_B_CompareOutputMode_NonPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ToggleOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_ToggleOnCompareMatch_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_ClearOnCompareMatch_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch, Timer_0_B_CompareOutputMode_NonPWM_SetOnCompareMatch_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_B_CompareOutputMode_FastPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatch_SetAtBottom, Timer_0_B_CompareOutputMode_FastPWM_ClearOnCompareMatch_SetAtBottom_d },
	{ Timer_CompareOutputMode_SetOnCompareMatch_ClearAtBottom, Timer_0_B_CompareOutputMode_FastPWM_SetOnCompareMatch_ClearAtBottom_d },
	{ Timer_CompareOutputMode_Disconnected, Timer_0_B_CompareOutputMode_PhaseCorrectPWM_Disconnected_d },
	{ Timer_CompareOutputMode_ClearOnCompareMatchUp_SetOnCompareMatchDown, Timer_0_B_CompareOutputMode_PhaseCorrectPWM_ClearOnCompareMatchUp_SetOnCompareMatchDown_d },
	{ Timer_CompareOutputMode_SetOnCompareMatchUp_ClearOnCompareMatchDown, Timer_0_B_CompareOutputMode_PhaseCorrectPWM_SetOnCompareMatchUp_ClearOnCompareMatchDown_d },
};

static uint8_t find_pair_pgm(uint8_t const addr[][2], uint8_t const len, uint8_t const find_bit_value, uint8_t const pair);

static uint8_t find_pair_pgm(uint8_t const addr[][2], uint8_t const len, uint8_t const find_bit_value, uint8_t const pair) {
	uint8_t x = find_bit_value ? 0 : 1;
	uint8_t y = find_bit_value ? 1 : 0;
	for (uint8_t i = 0; i < len; i++) {
		if (pgm_read_byte(&(addr[i][x])) == pair)
			return(pgm_read_byte(&(addr[i][y])));
	}
	return(UINT8_MAX);
}

static uint8_t find_compareOutputMode_pgm(uint8_t const addr[][2], uint8_t const addr_len, uint8_t const index[][3], uint8_t const index_len, uint8_t const find_bit_value, uint8_t const mode, uint8_t const com);

static uint8_t find_compareOutputMode_pgm(uint8_t const addr[][2], uint8_t const addr_len, uint8_t const index[][3], uint8_t const index_len, uint8_t const find_bit_value, uint8_t const mode, uint8_t const com) {
	uint8_t const mode_general = timer_get_modeGeneral_m(mode);
	for (uint8_t i = 0; i < index_len; i++) {
		if (pgm_read_byte(&(index[i][0])) == mode_general) {
			uint8_t result = find_pair_pgm(addr[pgm_read_byte(&(index[i][1]))], pgm_read_byte(&(index[i][2])), find_bit_value, com);
			if (result != UINT8_MAX)
				return(result);
			break;
		}
	}
	for (uint8_t i = 0; i < index_len; i++) {
		uint8_t const com_mode = pgm_read_byte(&(index[i][0]));
		if (com_mode < 128) {
			if (com_mode == mode) {
				uint8_t result = find_pair_pgm(addr[pgm_read_byte(&(index[i][1]))], pgm_read_byte(&(index[i][2])), find_bit_value, com);
				if (result != UINT8_MAX)
					return(result);
				break;
			}
		}
	}
	return(UINT8_MAX);
}

static void timer_0_set_clockSelect_bits(uint8_t const value);
static void timer_1_set_clockSelect_bits(uint8_t const value);
static void timer_0_set_mode_bits(uint8_t const value);
static void timer_1_set_mode_bits(uint8_t const value);
static void timer_0_set_compareOutputMode_A_bits(uint8_t const value);
static void timer_0_set_compareOutputMode_B_bits(uint8_t const value);
static void timer_1_set_compareOutputMode_A_bits(uint8_t const value);
static void timer_1_set_compareOutputMode_B_bits(uint8_t const value);
static uint8_t timer_0_get_clockSelect_bits(void);
static uint8_t timer_1_get_clockSelect_bits(void);
static uint8_t timer_0_get_mode_bits(void);
static uint8_t timer_1_get_mode_bits(void);
static Timer_ModeGeneral_e timer_get_modeGeneral_m(uint8_t const mode);
static uint8_t timer_0_get_compareOutputMode_A_bits(void);
static uint8_t timer_0_get_compareOutputMode_B_bits(void);
static void timer_0_set_clockSelect_bits(uint8_t const value) {
	TCCR0B &= 0b11111000;
	TCCR0B |= (value & 0b00000111);
}

static void timer_1_set_clockSelect_bits(uint8_t const value) {
	TCCR1B &= 0b11111000;
	TCCR1B |= (value & 0b00000111);
}

void timer_set_clockSelect(Timer_Select_e const timer, Timer_ClockSelect_e const value) {
	switch (timer) {
	case Timer_8_0:
		timer_0_set_clockSelect_bits(find_pair_pgm(array_clockSelect, Timer_ClockSelect_size, 1, value));
		break;
	case Timer_16_0:
		timer_1_set_clockSelect_bits(find_pair_pgm(array_clockSelect, Timer_ClockSelect_size, 1, value));
		break;
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
		timer_0_set_mode_bits(find_pair_pgm(array_0_mode, Timer_0_Mode_size, 1, value));
		break;
	case Timer_16_0:
		timer_1_set_mode_bits(find_pair_pgm(array_1_mode, Timer_1_Mode_size, 1, value));
		break;
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

static void timer_1_set_compareOutputMode_A_bits(uint8_t const value) {
	TCCR1A &= 0b00111111;
	TCCR1A |= ((value & 0b00000011) << 6);
}

static void timer_1_set_compareOutputMode_B_bits(uint8_t const value) {
	TCCR1A &= 0b11001111;
	TCCR1A |= ((value & 0b00000011) << 4);
}

void timer_set_compareOutputMode(Timer_Select_e const timer, Timer_OutputComparePin_e const pin, Timer_CompareOutputMode_e const value) {
	uint8_t res;
	switch (timer) {
	case Timer_8_0:
		switch (pin) {
		case Timer_OutputComparePin_A:
			res = find_compareOutputMode_pgm(array_0_A_compareOutputMode, Tiemr_0_A_CompareOutputMode_size, array_0_A_compareOutputMode_index, Timer_0_A_CompareOutputMode_index_size, 1, timer_0_get_mode_bits(), value);
			if (res != UINT8_MAX)
				timer_0_set_compareOutputMode_A_bits(res);
			break;
		case Timer_OutputComparePin_B:
			res = find_compareOutputMode_pgm(array_0_B_compareOutputMode, Timer_0_B_CompareOutputMode_size, array_0_B_compareOutputMode_index, Timer_0_B_CompareOutputMode_index_size, 1, timer_0_get_mode_bits(), value);
			if (res != UINT8_MAX)
				timer_0_set_compareOutputMode_B_bits(res);
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
	switch (timer) {
	case Timer_8_0:
		return(find_pair_pgm(array_clockSelect, Timer_ClockSelect_size, 0, timer_0_get_clockSelect_bits()));
		break;
	case Timer_16_0:
		return(find_pair_pgm(array_clockSelect, Timer_ClockSelect_size, 0, timer_1_get_clockSelect_bits()));
		break;
	default:
		break;
	}
	return(Timer_ClockSelect_None);
}

static uint8_t timer_0_get_mode_bits(void) {
	return(((TCCR0B & 0b00001000) >> 1) | (TCCR0A & 0b00000011));
}

static uint8_t timer_1_get_mode_bits(void) {
	return(((TCCR1B & 0b00011000) >> 1) | (TCCR1A & 0b00000011));
}

Timer_Mode_e timer_get_mode(Timer_Select_e const timer) {
	switch (timer) {
	case Timer_8_0:
		return(find_pair_pgm(array_0_mode, Timer_0_Mode_size, 0, timer_0_get_mode_bits()));
		break;
	case Timer_16_0:
		return(find_pair_pgm(array_1_mode, Timer_1_Mode_size, 0, timer_1_get_mode_bits()));
		break;
	default:
		break;
	}
	return(Timer_Mode_Normal);
}

static Timer_ModeGeneral_e timer_get_modeGeneral_m(uint8_t const mode) {
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

Timer_ModeGeneral_e timer_get_modeGeneral(Timer_Select_e const timer) {
	return(timer_get_modeGeneral_m(timer_get_mode(timer)));
}

static uint8_t timer_0_get_compareOutputMode_A_bits(void) {
	return((TCCR0A & 0b11000000) >> 6);
}

static uint8_t timer_0_get_compareOutputMode_B_bits(void) {
	return((TCCR0A & 0b00110000) >> 4);
}

Timer_CompareOutputMode_e timer_get_compareOutputMode(Timer_Select_e const timer, Timer_OutputComparePin_e const pin) {
	uint8_t res;
	switch (timer) {
	case Timer_8_0:
		switch (pin) {
		case Timer_OutputComparePin_A:
			res = find_compareOutputMode_pgm(array_0_A_compareOutputMode, Tiemr_0_A_CompareOutputMode_size, array_0_A_compareOutputMode_index, Timer_0_A_CompareOutputMode_index_size, 0, timer_0_get_mode_bits(), timer_0_get_compareOutputMode_A_bits());
			if (res != UINT8_MAX)
				return(res);
			break;
		case Timer_OutputComparePin_B:
			res = find_compareOutputMode_pgm(array_0_B_compareOutputMode, Timer_0_B_CompareOutputMode_size, array_0_B_compareOutputMode_index, Timer_0_B_CompareOutputMode_index_size, 0, timer_0_get_mode_bits(), timer_0_get_compareOutputMode_B_bits());
			if (res != UINT8_MAX)
				return(res);
			break;
		default:
			break;
		}
	default:
		break;
	}
	return(Timer_CompareOutputMode_Disconnected);
}
