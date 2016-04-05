#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

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

typedef struct Demultiplexer {
	uint8_t bitWidth;
	uint8_t portBit[5];
	volatile uint8_t* port[9];
}Demultiplexer;

uint8_t _multiplexBV(uint8_t bit);

uint8_t demultiplexer_calculateLineIndex_portBit(uint8_t line);
uint8_t demultiplexer_calculateLineIndex_port(uint8_t line);
uint8_t demultiplexer_calculateLineMask_portBit(uint8_t line);
uint8_t demultiplexer_calculateLineMask_port(uint8_t line);
uint8_t demultiplexer_calculateLineShift_portBit(uint8_t line);
uint8_t demultiplexer_calculateLineShift_port(uint8_t line);
uint8_t demultiplexer_calculateEncodedBitWidth(uint8_t bitWidth);
uint8_t demultiplexer_calculateDecodedBitWidth(uint8_t bitWidth);

void demultiplexerSet_bitWidth(Demultiplexer* demultiplexer,uint8_t state);
void demultiplexerSet_portBit(Demultiplexer* demultiplexer,uint8_t line,uint8_t state);
void demultiplexerSet_port(Demultiplexer* demultiplexer,uint8_t line,volatile uint8_t* state);
uint8_t demultiplexerGet_bitWidth(Demultiplexer* demultiplexer);
uint8_t demultiplexerGet_portBit(Demultiplexer* demultiplexer,uint8_t line);
volatile uint8_t* demultiplexerGet_port(Demultiplexer* demultiplexer,uint8_t line);

void demultiplexerSet_line(Demultiplexer* demultiplexer,uint8_t line,volatile uint8_t* port,uint8_t bit);
uint8_t demultiplexerGet_line_portBit(Demultiplexer* demultiplexer,uint8_t line);
volatile uint8_t* demultiplexerGet_line_port(Demultiplexer* demultiplexer,uint8_t line);

void demultiplexer_setLineState(Demultiplexer* demultiplexer,uint8_t line,uint8_t state);
uint8_t demultiplexer_getDataLineState(Demultiplexer* demultiplexer);
void demultiplexer_setChannel(Demultiplexer* demultiplexer,uint8_t channel);

void demultiplexer_clear(Demultiplexer* demultiplexer);
void demultiplexer_autoBindToPort(Demultiplexer* demultiplexer,volatile uint8_t* port,uint8_t startBit,uint8_t bitWidth);

#endif
