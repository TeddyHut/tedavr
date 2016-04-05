#include "../include/tedavr/shiftregister.h"

uint8_t _shiftregisterBV(uint8_t bit) {
	return(1<<bit);
}

uint8_t shiftregister_calculateLineIndex_portBit(uint8_t line) {
	return(line/2);
}

uint8_t shiftregister_calculateLineIndex_port(uint8_t line) {
	return(line);
}

uint8_t shiftregister_calculateLineMask_portBit(uint8_t line) {
	if(line%2) {
		return(0xF0);
	}
	else {
		return(0x0F);
	}
	return(0);
}

uint8_t shiftregister_calculateLineMask_port(uint8_t line) {
	return(0xFF);
}

uint8_t shiftregister_calculateLineShift_portBit(uint8_t line) {
	if(line%2) {
		return(4);
	}
	else {
		return(0);
	}
	return(0);
}

uint8_t shiftregister_calculateLineShift_port(uint8_t line) {
	return(0);
}

void shiftregisterSet_attrib_storeContent(Shiftregister* shiftregister,uint8_t state) {
	if(state) {
		shiftregister->attrib|=_shiftregisterBV(SHIFTREGISTER_ATTRIB_STORECONTENT);
	}
	else {
		shiftregister->attrib&=~_shiftregisterBV(SHIFTREGISTER_ATTRIB_STORECONTENT);
	}
}

void shiftregisterSet_attrib_popData(Shiftregister* shiftregister,uint8_t state) {
	if(state) {
		shiftregister->attrib|=_shiftregisterBV(SHIFTREGISTER_ATTRIB_POPDATA);
	}
	else {
		shiftregister->attrib&=~_shiftregisterBV(SHIFTREGISTER_ATTRIB_POPDATA);
	}
}

void shiftregisterSet_registerWidth(Shiftregister* shiftregister,uint8_t state) {
	shiftregister->registerWidth = state;
}

void shiftregisterSet_portBit(Shiftregister* shiftregister,uint8_t line,uint8_t state) {
	shiftregister->portBit[shiftregister_calculateLineIndex_portBit(line)]&=~shiftregister_calculateLineMask_portBit(line);
	shiftregister->portBit[shiftregister_calculateLineIndex_portBit(line)]|=(state<<shiftregister_calculateLineShift_portBit(line));
}

void shiftregisterSet_port(Shiftregister* shiftregister,uint8_t line,volatile uint8_t* state) {
	shiftregister->port[line] = state;
}

void shiftregisterSet_contentData(Shiftregister* shiftregister,uint8_t index,uint8_t state) {
	shiftregister->contentData[index] = state;
}

void shiftregisterSet_contentDataBit(Shiftregister* shiftregister,uint8_t index,uint8_t bit,uint8_t state) {
	if(state) {
		shiftregister->contentData[index]|=_shiftregisterBV(bit);
	}
	else {
		shiftregister->contentData[index]&=~_shiftregisterBV(bit);
	}
}

uint8_t shiftregisterGet_attrib_storeContent(Shiftregister* shiftregister) {
	return(shiftregister->attrib&_shiftregisterBV(SHIFTREGISTER_ATTRIB_STORECONTENT));
}

uint8_t shiftregisterGet_attrib_popData(Shiftregister* shiftregister) {
	return(shiftregister->attrib&_shiftregisterBV(SHIFTREGISTER_ATTRIB_POPDATA));
}

uint8_t shiftregisterGet_registerWidth(Shiftregister* shiftregister) {
	return(shiftregister->registerWidth);
}

uint8_t shiftregisterGet_portBit(Shiftregister* shiftregister,uint8_t line) {
	return((shiftregister->portBit[shiftregister_calculateLineIndex_portBit(line)]&shiftregister_calculateLineMask_portBit(line))>>shiftregister_calculateLineShift_portBit(line));
}

volatile uint8_t* shiftregisterGet_port(Shiftregister* shiftregister,uint8_t line) {
	return(shiftregister->port[line]);
}

uint8_t shiftregisterGet_contentData(Shiftregister* shiftregister,uint8_t index) {
	return(shiftregister->contentData[index]);
}

uint8_t shiftregisterGet_contentDataBit(Shiftregister* shiftregister,uint8_t index,uint8_t bit) {
	return(shiftregister->contentData[index]&_shiftregisterBV(bit));
}

void shiftregisterSet_line(Shiftregister* shiftregister,uint8_t line,volatile uint8_t* port,uint8_t bit) {
	shiftregisterSet_portBit(shiftregister,line,bit);
	shiftregisterSet_port(shiftregister,line,port);
}

uint8_t shiftregisterGet_line_portBit(Shiftregister* shiftregister,uint8_t line) {
	return(shiftregisterGet_portBit(shiftregister,line));
}

volatile uint8_t* shiftregisterGet_line_port(Shiftregister* shiftregister,uint8_t line) {
	return(shiftregisterGet_port(shiftregister,line));
}

void shiftregister_setLineState(Shiftregister* shiftregister,uint8_t line,uint8_t state) {
	if(state) {
		*shiftregisterGet_port(shiftregister,line)|=_shiftregisterBV(shiftregisterGet_portBit(shiftregister,line));
	}
	else {
		*shiftregisterGet_port(shiftregister,line)&=~_shiftregisterBV(shiftregisterGet_portBit(shiftregister,line));
	}
}

uint8_t shiftregister_getLineState(Shiftregister* shiftregister,uint8_t line) {
	return((*shiftregisterGet_port(shiftregister,line)&_shiftregisterBV(shiftregisterGet_portBit(shiftregister,line)))>>shiftregisterGet_portBit(shiftregister,line));
}

void shiftregister_push_bit(Shiftregister* shiftregister,uint8_t bit) {
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SERIALSEND_LINE,bit);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SHIFTRCLOCK_LINE,1);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SERIALSEND_LINE,0);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SHIFTRCLOCK_LINE,0);
	if(shiftregisterGet_attrib_storeContent(shiftregister)) {
		uint8_t carry[2];
		carry[1] = bit;
		for(uint8_t i = 0;i < shiftregisterGet_registerWidth(shiftregister);i++) {
			carry[0] = shiftregisterGet_contentDataBit(shiftregister,i,7);
			shiftregisterSet_contentData(shiftregister,i,shiftregisterGet_contentData(shiftregister,i)<<1);
			shiftregisterSet_contentDataBit(shiftregister,i,0,carry[1]);
			carry[1] = carry[0];
		}
	}
}

void shiftregister_push_byte(Shiftregister* shiftregister,uint8_t byte) {
	for(uint8_t i = 8;i > 0;i--) {
		shiftregister_push_bit(shiftregister,byte&_shiftregisterBV(i-1));
	}
}

void shiftregister_push_buffer(Shiftregister* shiftregister,uint8_t bufferSize,void* buffer) {
	for(uint8_t i = 0;i < bufferSize;i++) {
		shiftregister_push_byte(shiftregister,*((uint8_t*)(buffer+i)));
	}
}

uint8_t shiftregister_push_pop_bit(Shiftregister* shiftregister,uint8_t bit) {
	uint8_t rtn = 0;
	rtn = shiftregister_getLineState(shiftregister,SHIFTREGISTER_SERIALRETURN_LINE);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SERIALSEND_LINE,bit);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SHIFTRCLOCK_LINE,1);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SERIALSEND_LINE,0);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_SHIFTRCLOCK_LINE,0);
	if(shiftregisterGet_attrib_storeContent(shiftregister)) {
		uint8_t carry[2];
		carry[1] = bit;
		for(uint8_t i = 0;i < shiftregisterGet_registerWidth(shiftregister);i++) {
			carry[0] = shiftregisterGet_contentDataBit(shiftregister,i,7);
			shiftregisterSet_contentData(shiftregister,i,shiftregisterGet_contentData(shiftregister,i)<<1);
			shiftregisterSet_contentDataBit(shiftregister,i,0,carry[1]);
			carry[1] = carry[0];
		}
	}
	return(rtn);
}

uint8_t shiftregister_push_pop_byte(Shiftregister* shiftregister,uint8_t byte) {
	uint8_t rtn = 0;
	for(uint8_t i = 8;i > 0;i--) {
		rtn|=(shiftregister_push_pop_bit(shiftregister,(byte&_shiftregisterBV(i-1))>>(i-1)))<<(i-1);
	}
	return(rtn);
}

void shiftregister_push_pop_buffer(Shiftregister* shiftregister,uint8_t bufferSize,void* outbuffer,void* inbuffer) {
	uint8_t* buffer = (uint8_t*)inbuffer;
	for(uint8_t i = 0;i < bufferSize;i++) {
		buffer[i] = shiftregister_push_pop_byte(shiftregister,*((uint8_t*)(outbuffer+i)));
	}
	inbuffer = (void*)buffer;
}

void shiftregister_latchRegisters(Shiftregister* shiftregister) {
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_STORAGERCLOCK_LINE,1);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_STORAGERCLOCK_LINE,0);
}

void shiftregister_resetRegisters(Shiftregister* shiftregister) {
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_MASTERRESET_LINE,0);
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_MASTERRESET_LINE,1);
	if(shiftregisterGet_attrib_storeContent(shiftregister)) {
		for(uint8_t i = 0;i < shiftregisterGet_registerWidth(shiftregister);i++) {
			shiftregisterSet_contentData(shiftregister,i,0);
		}
	}
}

void shiftregister_disableOuputs(Shiftregister* shiftregister) {
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_OUTPUTENABLE_LINE,1);
}

void shiftregister_enableOutputs(Shiftregister* shiftregister) {
	shiftregister_setLineState(shiftregister,SHIFTREGISTER_OUTPUTENABLE_LINE,0);
}

void shiftregister_allocateMemory(Shiftregister* shiftregister) {
	shiftregister->contentData = malloc(shiftregisterGet_registerWidth(shiftregister));
}

void shiftregister_deallocateMemory(Shiftregister* shiftregister) {
	free(shiftregister->contentData);
}

void shiftregister_init(Shiftregister* shiftregister) {
	shiftregister_allocateMemory(shiftregister);
	shiftregister_resetRegisters(shiftregister);
	shiftregister_latchRegisters(shiftregister);
	shiftregister_enableOutputs(shiftregister);
}

void shiftregister_clear(Shiftregister* shiftregister) {
	for(uint8_t i = 0;i < 6;i++) {
		shiftregisterSet_portBit(shiftregister,i,0);
	}
	shiftregisterSet_registerWidth(shiftregister,0);
}

void shiftregister_autoBindToPort(Shiftregister* shiftregister,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit) {
	shiftregisterSet_line(shiftregister,SHIFTREGISTER_MASTERRESET_LINE,portWrite,(startBit+0));
	shiftregisterSet_line(shiftregister,SHIFTREGISTER_OUTPUTENABLE_LINE,portWrite,(startBit+1));
	shiftregisterSet_line(shiftregister,SHIFTREGISTER_SHIFTRCLOCK_LINE,portWrite,(startBit+2));
	shiftregisterSet_line(shiftregister,SHIFTREGISTER_STORAGERCLOCK_LINE,portWrite,(startBit+3));
	shiftregisterSet_line(shiftregister,SHIFTREGISTER_SERIALSEND_LINE,portWrite,(startBit+4));
	if(shiftregisterGet_attrib_popData(shiftregister)) {
		shiftregisterSet_line(shiftregister,SHIFTREGISTER_SERIALRETURN_LINE,portRead,(startBit+5));
	}
}
