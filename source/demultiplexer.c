#include "../include/tedavr/demultiplexer.h"

uint8_t _multiplexBV(uint8_t bit) {
	return(1<<bit);
}

uint8_t demultiplexer_calculateLineIndex_portBit(uint8_t line) {
	return(line/2);
}

uint8_t demultiplexer_calculateLineIndex_port(uint8_t line) {
	return(line);
}

uint8_t demultiplexer_calculateLineMask_portBit(uint8_t line) {
	if(line%2) {
		return(0xF0);
	}
	else {
		return(0x0F);
	}
	return(0);
}

uint8_t demultiplexer_calculateLineMask_port(uint8_t line) {
	return(0xFF);
}

uint8_t demultiplexer_calculateLineShift_portBit(uint8_t line) {
	if(line%2) {
		return(4);
	}
	else {
		return(0);
	}
	return(0);
}

uint8_t demultiplexer_calculateLineShift_port(uint8_t line) {
	return(0);
}

uint8_t demultiplexer_calcualteEncodedBitWidth(uint8_t bitWidth) {
	uint8_t a = 0;
	while(1) {
		if(bitWidth) {
			a|=_multiplexBV(bitWidth-1);
			bitWidth--;
		}
		else {
			break;
		}
	}
	return(a);
}

uint8_t demultiplexer_calculateDecodedBitWidth(uint8_t bitWidth) {
	uint8_t a = 0;
	for(uint8_t i = 0;i < 8;i++) {
		if(bitWidth&_multiplexBV(i)) {
			a++;
		}
	}
	return(a);
}

void demultiplexerSet_bitWidth(Demultiplexer* demultiplexer,uint8_t state) {
	demultiplexer->bitWidth = state;
}

void demultiplexerSet_portBit(Demultiplexer* demultiplexer,uint8_t line,uint8_t state) {
	demultiplexer->portBit[demultiplexer_calculateLineIndex_portBit(line)]&=~(demultiplexer_calculateLineMask_portBit(line));
	demultiplexer->portBit[demultiplexer_calculateLineIndex_portBit(line)]|=(state<<demultiplexer_calculateLineShift_portBit(line));
}

void demultiplexerSet_port(Demultiplexer* demultiplexer,uint8_t line,volatile uint8_t* state) {
	demultiplexer->port[line] = state;
}

uint8_t demultiplexerGet_bitWidth(Demultiplexer* demultiplexer) {
	return(demultiplexer->bitWidth);
}

uint8_t demultiplexerGet_portBit(Demultiplexer* demultiplexer,uint8_t line) {
	return((demultiplexer->portBit[demultiplexer_calculateLineIndex_portBit(line)]&demultiplexer_calculateLineMask_portBit(line))>>demultiplexer_calculateLineShift_portBit(line));
}

volatile uint8_t* demultiplexerGet_port(Demultiplexer* demultiplexer,uint8_t line) {
	return(demultiplexer->port[line]);
}

void demultiplexerSet_line(Demultiplexer* demultiplexer,uint8_t line,volatile uint8_t* port,uint8_t bit) {
	demultiplexerSet_portBit(demultiplexer,line,bit);
	demultiplexerSet_port(demultiplexer,line,port);
}

uint8_t demultiplexerGet_line_portBit(Demultiplexer* demultiplexer,uint8_t line) {
	return(demultiplexerGet_portBit(demultiplexer,line));
}

volatile uint8_t* demultiplexerGet_line_port(Demultiplexer* demultiplexer,uint8_t line) {
	return(demultiplexerGet_port(demultiplexer,line));
}

void demultiplexer_setLineState(Demultiplexer* demultiplexer,uint8_t line,uint8_t state) {
	if(state) {
		*demultiplexerGet_port(demultiplexer,line)|=_multiplexBV(demultiplexerGet_portBit(demultiplexer,line));
	}
	else {
		*demultiplexerGet_port(demultiplexer,line)&=~(_multiplexBV(demultiplexerGet_portBit(demultiplexer,line)));
	}
}

uint8_t demultiplexer_getDataLineState(Demultiplexer* demultiplexer) {
	return((*demultiplexerGet_port(demultiplexer,MULTIPLEX_DATA_LINE)&_multiplexBV(demultiplexerGet_portBit(demultiplexer,MULTIPLEX_DATA_LINE)))>>demultiplexerGet_portBit(demultiplexer,MULTIPLEX_DATA_LINE));
}

void demultiplexer_setChannel(Demultiplexer* demultiplexer,uint8_t channel) {
	for(uint8_t i = 0;i < demultiplexer_calculateDecodedBitWidth(demultiplexerGet_bitWidth(demultiplexer));i++) {
		demultiplexer_setLineState(demultiplexer,i,channel&_multiplexBV(i));
	}
}

void demultiplexer_clear(Demultiplexer* demultiplexer) {
	demultiplexer->bitWidth = MULTIPLEX_BITWIDTH_0;
	for(uint8_t i = 0;i < 5;i++) {
		demultiplexer->portBit[i] = 0;
	}
}

void demultiplexer_autoBindToPort(Demultiplexer* demultiplexer,volatile uint8_t* port,uint8_t startBit,uint8_t bitWidth) {
	demultiplexerSet_bitWidth(demultiplexer,bitWidth);
	for(uint8_t i = 0;i < demultiplexer_calculateDecodedBitWidth(bitWidth);i++) {
		demultiplexerSet_line(demultiplexer,i,port,(i+startBit));
	}
}
