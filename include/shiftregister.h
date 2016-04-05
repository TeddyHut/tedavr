#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include <avr/io.h>
#include <stdlib.h>

#define SHIFTREGISTER_MASTERRESET_LINE 0
#define SHIFTREGISTER_OUTPUTENABLE_LINE 1
#define SHIFTREGISTER_SHIFTRCLOCK_LINE 2
#define SHIFTREGISTER_STORAGERCLOCK_LINE 3
#define SHIFTREGISTER_SERIALSEND_LINE 4
#define SHIFTREGISTER_SERIALRETURN_LINE 5

#define SHIFTREGISTER_ATTRIB_STORECONTENT 0
#define SHIFTREGISTER_ATTRIB_POPDATA 1

typedef struct Shiftregister {
	uint8_t attrib;
	uint8_t registerWidth;
	uint8_t portBit[3];
	volatile uint8_t* port[6];
	uint8_t* contentData;
}Shiftregister;

uint8_t _shiftregisterBV(uint8_t bit);

uint8_t shiftregister_calculateLineIndex_portBit(uint8_t line);
uint8_t shiftregister_calculateLineIndex_port(uint8_t line);
uint8_t shiftregister_calculateLineMask_portBit(uint8_t line);
uint8_t shiftregister_calculateLineMask_port(uint8_t line);
uint8_t shiftregister_calculateLineShift_portBit(uint8_t line);
uint8_t shiftregister_calculateLineShift_port(uint8_t line);

void shiftregisterSet_attrib_storeContent(Shiftregister* shiftregister,uint8_t state);
void shiftregisterSet_attrib_popData(Shiftregister* shiftregister,uint8_t state);
void shiftregisterSet_registerWidth(Shiftregister* shiftregister,uint8_t state);
void shiftregisterSet_portBit(Shiftregister* shiftregister,uint8_t line,uint8_t state);
void shiftregisterSet_port(Shiftregister* shiftregister,uint8_t line,volatile uint8_t* state);
void shiftregisterSet_contentData(Shiftregister* shiftregister,uint8_t index,uint8_t state);
void shiftregisterSet_contentDataBit(Shiftregister* shiftregister,uint8_t index,uint8_t bit,uint8_t state);

uint8_t shiftregisterGet_attrib_storeContent(Shiftregister* shiftregister);
uint8_t shiftregisterGet_attrib_popData(Shiftregister* shiftregister);
uint8_t shiftregisterGet_registerWidth(Shiftregister* shiftregister);
uint8_t shiftregisterGet_portBit(Shiftregister* shiftregister,uint8_t line);
volatile uint8_t* shiftregisterGet_port(Shiftregister* shiftregister,uint8_t line);
uint8_t shiftregisterGet_contentData(Shiftregister* shiftregister,uint8_t index);
uint8_t shiftregisterGet_contentDataBit(Shiftregister* shiftregister,uint8_t index,uint8_t bit);

void shiftregisterSet_line(Shiftregister* shiftregister,uint8_t line,volatile uint8_t* port,uint8_t bit);
uint8_t shiftregisterGet_line_portBit(Shiftregister* shiftregister,uint8_t line);
volatile uint8_t* shiftregisterGet_line_port(Shiftregister* shiftregister,uint8_t line);

void shiftregister_setLineState(Shiftregister* shiftregister,uint8_t line,uint8_t state);
uint8_t shiftregister_getLineState(Shiftregister* shiftregister,uint8_t line);

void shiftregister_push_bit(Shiftregister* shiftregister,uint8_t bit);
void shiftregister_push_byte(Shiftregister* shiftregister,uint8_t byte);
void shiftregister_push_buffer(Shiftregister* shiftregister,uint8_t bufferSize,void* buffer);
uint8_t shiftregister_push_pop_bit(Shiftregister* shiftregiser,uint8_t bit);
uint8_t shiftregister_push_pop_byte(Shiftregister* shiftregister,uint8_t byte);
void shiftregister_push_pop_buffer(Shiftregister* shiftregister,uint8_t bufferSize,void* outbuffer,void* inbuffer);

void shiftregister_latchRegisters(Shiftregister* shiftregister);
void shiftregister_resetRegisters(Shiftregister* shiftregister);
void shiftregister_disableOutputs(Shiftregister* shiftregister);
void shiftregister_enableOutputs(Shiftregister* shiftregister);

void shiftregister_allocateMemory(Shiftregister* shiftregister);
void shiftregister_deallocateMemory(Shiftregister* shiftregister);
void shiftregister_init(Shiftregister* shiftregister);
void shiftregister_clear(Shiftregister* shiftregister);
void shiftregister_autoBindToPort(Shiftregister* shiftregister,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit);

#endif //shiftRegister.h
