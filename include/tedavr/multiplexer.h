#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <avr/io.h>
#include <util/delay.h>

#define MULTIPLEX_BITWIDTH_0 0b00000000
#define MULTIPLEX_BITWIDTH_1 0b00000001
#define MULTIPLEX_BITWIDTH_2 0b00000011
#define MULTIPLEX_BITWIDTH_3 0b00000111
#define MULTIPLEX_BITWIDTH_4 0b00001111
#define MULTIPLEX_BITWIDTH_5 0b00011111
#define MULTIPLEX_BITWIDTH_6 0b00111111
#define MULTIPLEX_BITWIDTH_7 0b01111111
#define MULTIPLEX_BITWIDTH_8 0b11111111

#define MULTIPLEX_DATA_LINE 8

typedef struct Multiplexer {
	uint8_t bitWidth;
	uint8_t portBit[5];
	volatile uint8_t* port[9];
}Multiplexer;

uint8_t _multiplexBV(uint8_t bit);

uint8_t multiplexer_calculateLineIndex_portBit(uint8_t line);
uint8_t multiplexer_calculateLineIndex_port(uint8_t line);
uint8_t multiplexer_calculateLineMask_portBit(uint8_t line);
uint8_t multiplexer_calculateLineMask_port(uint8_t line);
uint8_t multiplexer_calculateLineShift_portBit(uint8_t line);
uint8_t multiplexer_calculateLineShift_port(uint8_t line);
uint8_t multiplexer_calculateEncodedBitWidth(uint8_t bitWidth);
uint8_t multiplexer_calculateDecodedBitWidth(uint8_t bitWidth);

void multiplexerSet_bitWidth(Multiplexer* multiplexer,uint8_t state);
void multiplexerSet_portBit(Multiplexer* multiplexer,uint8_t line,uint8_t state);
void multiplexerSet_port(Multiplexer* multiplexer,uint8_t line,volatile uint8_t* state);
uint8_t multiplexerGet_bitWidth(Multiplexer* multiplexer);
uint8_t multiplexerGet_portBit(Multiplexer* multiplexer,uint8_t line);
volatile uint8_t* multiplexerGet_port(Multiplexer* multiplexer,uint8_t line);

void multiplexerSet_line(Multiplexer* multiplexer,uint8_t line,volatile uint8_t* port,uint8_t bit);
uint8_t multiplexerGet_line_portBit(Multiplexer* multiplexer,uint8_t line);
volatile uint8_t* multiplexerGet_line_port(Multiplexer* multiplexer,uint8_t line);

void multiplexer_setLineState(Multiplexer* multiplexer,uint8_t line,uint8_t state);
void multiplexer_setDataLineState(Multiplexer* multiplexer,uint8_t state);
void multiplexer_setChannel(Multiplexer* multiplexer,uint8_t channel);

void multiplexer_clear(Multiplexer* multiplexer);
void multiplexer_autoBindToPort(Multiplexer* multiplexer,volatile uint8_t* port,uint8_t startBit,uint8_t bitWidth);

#endif
