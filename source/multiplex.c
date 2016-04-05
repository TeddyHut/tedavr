#include "../include/tedavr/multiplex.h"

uint8_t _multiplexBV(uint8_t bit) {
	return(1<<bit);
}

uint8_t multiplexer_calculateLineIndex_portBit(uint8_t line) {
	return(line/2);
}

uint8_t multiplexer_calculateLineIndex_port(uint8_t line) {
	return(line);
}

uint8_t multiplexer_calculateLineMask_portBit(uint8_t line) {
	if(line%2) {
		return(0xF0);
	}
	else {
		return(0x0F);
	}
	return(0);
}

uint8_t multiplexer_calculateLineMask_port(uint8_t line) {
	return(0xFF);
}

uint8_t multiplexer_calculateLineShift_portBit(uint8_t line) {
	if(line%2) {
		return(4);
	}
	else {
		return(0);
	}
	return(0);
}

uint8_t multiplexer_calculateLineShift_port(uint8_t line) {
	return(0);
}

uint8_t multiplexer_calculateEncodedBitWidth(uint8_t bitWidth) {
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

uint8_t multiplexer_calculateDecodedBitWidth(uint8_t bitWidth) {
	uint8_t a = 0;
	for(uint8_t i = 0;i < 8;i++) {
		if(bitWidth&_multiplexBV(i)) {
			a++;
		}
	}
	return(a);
}

void multiplexerSet_bitWidth(Multiplexer* multiplexer,uint8_t state) {
	multiplexer->bitWidth = state;
}

void multiplexerSet_portBit(Multiplexer* multiplexer,uint8_t line,uint8_t state) {
	multiplexer->portBit[multiplexer_calculateLineIndex_portBit(line)]&=~(multiplexer_calculateLineMask_portBit(line));
	multiplexer->portBit[multiplexer_calculateLineIndex_portBit(line)]|=(state<<multiplexer_calculateLineShift_portBit(line));
}

void multiplexerSet_port(Multiplexer* multiplexer,uint8_t line,volatile uint8_t* state) {
	multiplexer->port[line] = state;
}

uint8_t multiplexerGet_bitWidth(Multiplexer* multiplexer) {
	return(multiplexer->bitWidth);
}

uint8_t multiplexerGet_portBit(Multiplexer* multiplexer,uint8_t line) {
	return((multiplexer->portBit[multiplexer_calculateLineIndex_portBit(line)]&multiplexer_calculateLineMask_portBit(line))>>multiplexer_calculateLineShift_portBit(line));
}

volatile uint8_t* multiplexerGet_port(Multiplexer* multiplexer,uint8_t line) {
	return(multiplexer->port[line]);
}

void multiplexerSet_line(Multiplexer* multiplexer,uint8_t line,volatile uint8_t* port,uint8_t bit) {
	multiplexerSet_portBit(multiplexer,line,bit);
	multiplexerSet_port(multiplexer,line,port);
}

uint8_t multiplexerGet_line_portBit(Multiplexer* multiplexer,uint8_t line) {
	return(multiplexerGet_portBit(multiplexer,line));
}

volatile uint8_t* multiplexerGet_line_port(Multiplexer* multiplexer,uint8_t line) {
	return(multiplexerGet_port(multiplexer,line));
}

void multiplexer_setLineState(Multiplexer* multiplexer,uint8_t line,uint8_t state) {
	if(state) {
		*multiplexerGet_port(multiplexer,line)|=_multiplexBV(multiplexerGet_portBit(multiplexer,line));
	}
	else {
		*multiplexerGet_port(multiplexer,line)&=~(_multiplexBV(multiplexerGet_portBit(multiplexer,line)));
	}
}

void multiplexer_setDataLineState(Multiplexer* multiplexer,uint8_t state) {
	multiplexer_setLineState(multiplexer,MULTIPLEX_DATA_LINE,state);
}

void multiplexer_setChannel(Multiplexer* multiplexer,uint8_t channel) {
	for(uint8_t i = 0;i < multiplexer_calculateDecodedBitWidth(multiplexerGet_bitWidth(multiplexer));i++) {
		multiplexer_setLineState(multiplexer,i,channel&_multiplexBV(i));
	}
}

void multiplexer_clear(Multiplexer* multiplexer) {
	multiplexer->bitWidth = MULTIPLEX_BITWIDTH_0;
	for(uint8_t i = 0;i < 5;i++) {
		multiplexer->portBit[i] = 0;
	}
}

void multiplexer_autoBindToPort(Multiplexer* multiplexer,volatile uint8_t* port,uint8_t startBit,uint8_t bitWidth) {
   multiplexerSet_bitWidth(multiplexer,bitWidth);
	for(uint8_t i = 0;i < multiplexer_calculateDecodedBitWidth(bitWidth);i++) {
	   multiplexerSet_line(multiplexer,i,port,(i+startBit));
   }
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
