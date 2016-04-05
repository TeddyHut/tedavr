#ifndef ACI_H
#define ACI_H

#include <avr/io.h>
#include <util/delay.h>

#define ACISLAVE_CLOCK_LINE 0
#define ACISLAVE_MOSI_LINE 1
#define ACISLAVE_MISO_LINE 2
#define ACISLAVE_REQUEST_LINE 3

#define ACIMASTER_CLOCK_LINE 8
#define ACIMASTER_MOSI_LINE 9
#define ACIMASTER_MISO_LINE 10

#define ACI_SLAVEWIDTH_0 0b00000000
#define ACI_SLAVEWIDTH_1 0b00000001
#define ACI_SLAVEWIDTH_2 0b00000011
#define ACI_SLAVEWIDTH_3 0b00000111
#define ACI_SLAVEWIDTH_4 0b00001111
#define ACI_SLAVEWIDTH_5 0b00011111
#define ACI_SLAVEWIDTH_6 0b00111111
#define ACI_SLAVEWIDTH_7 0b01111111
#define ACI_SLAVEWIDTH_8 0b11111111

#define ACI_REQUEST_NONE 0
#define ACI_REQUEST_SEND 1
#define ACI_REQUEST_SEND_RECEIVE 2

#define ACIMASTER_REQUEST_SEND 0
#define ACIMASTER_REQUEST_SEND_RECEIVE 1

typedef struct ACISlave {
	uint8_t portBit[2];
	volatile  uint8_t* port[4];
}ACISlave;

typedef struct ACIMaster {
	uint16_t clock_kHz;
	uint8_t slaveWidth;
	uint8_t portBit[6];
	volatile uint8_t* port[11];
}ACIMaster;

uint8_t _aciBV(uint8_t bit);
void aci_delay_us(uint16_t us);

uint8_t aci_calculateLineIndex_portBit(uint8_t line);
uint8_t aci_calculateLineIndex_port(uint8_t line);
uint8_t aci_calculateLineMask_portBit(uint8_t line);
uint8_t aci_calculateLineMask_port(uint8_t line);
uint8_t aci_calculateLineShift_portBit(uint8_t line);
uint8_t aci_calculateLineShift_port(uint8_t line);
uint8_t aci_calculateEncodedSlaveWidth(uint8_t slaveWidth);
uint8_t aci_calculateDecodedSlaveWidth(uint8_t slaveWidth);
uint16_t aci_calculateEncodedBufferSize(uint8_t master,uint8_t slave);
uint8_t aci_calculateDecodedBufferSize_master(uint16_t size);
uint8_t aci_calculateDecodedBufferSize_slave(uint16_t size);
uint16_t aci_calculateClockDelay_us(uint16_t kHz);

void aciSlaveSet_portBit(ACISlave* aciSlave,uint8_t line,uint8_t state);
void aciSlaveSet_port(ACISlave* aciSlave,uint8_t line,volatile uint8_t* state);
uint8_t aciSlaveGet_portBit(ACISlave* aciSlave,uint8_t line);
volatile uint8_t* aciSlaveGet_port(ACISlave* aciSlave,uint8_t line);

void aciSlaveSet_line(ACISlave* aciSlave,uint8_t line,volatile uint8_t* port,uint8_t bit);
uint8_t aciSlaveGet_line_portBit(ACISlave* aciSlave,uint8_t line);
volatile uint8_t* aciSlaveGet_line_port(ACISlave* aviSlave,uint8_t line);

void aciSlave_setLineState(ACISlave* aciSlave,uint8_t line,uint8_t state);
uint8_t aciSlave_getLineState(ACISlave* aciSlave,uint8_t line);

uint8_t aciSlave_checkForRequest(ACISlave* aciSlave);

void aciSlave_waitForClockHigh(ACISlave* aciSlave);
void aciSlave_waitForClockLow(ACISlave* aciSlave);

uint8_t aciSlave_r_byte(ACISlave* aciSlave);
void aciSlave_r_dataBufferSize(ACISlave* aciSlave,uint8_t* master);
void aciSlave_r_dataBuffer(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer);

uint8_t aciSlave_rs_byteReceive(ACISlave* aciSlave);
void aciSlave_rs_byteSend(ACISlave* aciSlave,uint8_t byte);
void aciSlave_rs_dataBufferSize(ACISlave* aciSlave,uint8_t* master,uint8_t* slave);
void aciSlave_rs_dataBufferReceive(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer);
void aciSlave_rs_dataBufferSend(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer);

void aciSlave_init(ACISlave* aciSlave);
volatile uint8_t aciSlave_DDR(ACISlave* aciSlave);
void aciSlave_clear(ACISlave* aciSlave);
void aciSlave_autoBindToPort(ACISlave* aciSlave,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit);

void aciMasterSet_clock_kHz(ACIMaster* aciMaster,uint16_t state);
void aciMasterSet_slaveWidth(ACIMaster* aciMaster,uint8_t state);
void aciMasterSet_portBit(ACIMaster* aciMaster,uint8_t line,uint8_t state);
void aciMasterSet_port(ACIMaster* aciMaster,uint8_t line,volatile uint8_t* state);
uint16_t aciMasterGet_clock_kHz(ACIMaster* aciMaster);
uint8_t aciMasterGet_slaveWidth(ACIMaster* aciMaster);
uint8_t aciMasterGet_portBit(ACIMaster* aciMaster,uint8_t line);
volatile uint8_t* aciMasterGet_port(ACIMaster* aciMaster,uint8_t line);

void aciMasterSet_line(ACIMaster* aciMaster,uint8_t line,volatile uint8_t* port,uint8_t bit);
uint8_t aciMasterGet_line_portBit(ACIMaster* aciMaster,uint8_t line);
volatile uint8_t* aciMasterGet_line_port(ACIMaster* aciMaster,uint8_t line);

void aciMaster_setLineState(ACIMaster* aciMaster,uint8_t line,uint8_t state);
uint8_t aciMaster_getLineState(ACIMaster* aciMaster,uint8_t line);

void aciMaster_request(ACIMaster* aciMaster,uint8_t slave,uint8_t type);
void aciMaster_unrequest(ACIMaster* aciMaster,uint8_t slave);
uint8_t aciMaster_checkForResponse(ACIMaster* aciMaster);

void aciMaster_clockDelay(ACIMaster* aciMaster,uint8_t divide);
void aciMaster_setClockHigh(ACIMaster* aciMaster);
void aciMaster_setClockLow(ACIMaster* aciMaster);

void aciMaster_s_byte(ACIMaster* aciMaster,uint8_t byte);
void aciMaster_s_dataBufferSize(ACIMaster* aciMaster,uint8_t bufferSize);
void aciMaster_s_dataBuffer(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer);
void aciMaster_s_endCommunication(ACIMaster* aciMaster,uint8_t slave);

void aciMaster_sr_byteSend(ACIMaster* aciMaster,uint8_t byte);
uint8_t aciMaster_sr_byteReceive(ACIMaster* aciMaster);
void aciMaster_sr_dataBufferSize(ACIMaster* aciMaster,uint8_t master,uint8_t slave);
void aciMaster_sr_dataBufferSend(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer);
void aciMaster_sr_dataBufferReceive(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer);
void aciMaster_sr_endCommunication(ACIMaster* aciMaster,uint8_t slave);

void aciMaster_init(ACIMaster* aciMaster);
volatile uint8_t aciMaster_DDR(ACIMaster* aciMaster);
void aciMaster_clear(ACIMaster* aciMaster);
void aciMaster_autoBindToPort(ACIMaster* aciMaster,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit,uint8_t slaveWidth);

#endif //aci.h
