#include "../include/tedavr/aciSlave.h"

uint8_t _aciBV(uint8_t bit) {
	return(1<<bit);
}

void aci_delay_us(uint16_t us) {
	for(uint16_t i = 0;i < us;i++) {
		_delay_us(1);
	}
}

uint8_t aci_calculateLineIndex_portBit(uint8_t line) {
	return(line/2);
}

uint8_t aci_calculateLineIndex_port(uint8_t line) {
	return(line);
}

uint8_t aci_calculateLineMask_portBit(uint8_t line) {
	if(line%2) {
		return(0xF0);
	}
	else {
		return(0x0F);
	}
	return(0);
}

uint8_t aci_calculateLineMask_port(uint8_t line) {
	return(0xFF);
}

uint8_t aci_calculateLineShift_portBit(uint8_t line) {
	if(line%2) {
		return(4);
	}
	else {
		return(0);
	}
	return(0);
}

uint8_t aci_calculateLineShift_port(uint8_t line) {
	return(0);
}

uint8_t aci_calculateEncodedSlaveWidth(uint8_t slaveWidth) {
	uint8_t a = 0;
	while(1) {
		if(slaveWidth) {
			a|=_aciBV(slaveWidth-1);
			slaveWidth--;
		}
		else {
			break;
		}
	}
	return(a);
}

uint8_t aci_calculateDecodedSlaveWidth(uint8_t slaveWidth) {
	uint8_t a = 0;
	for(uint8_t i = 0;i < 8;i++) {
		if(slaveWidth&_aciBV(i)) {
			a++;
		}
	}
	return(a);
}

uint16_t aci_calculateEncodedBufferSize(uint8_t master,uint8_t slave) {
	uint16_t size = 0;
	size|=master;
	size|=(slave<<8);
	return(size);
}

uint8_t aci_calculateDecodedBufferSize_master(uint16_t size) {
	return((uint8_t)(size&0x00FF));
}

uint8_t aci_calculateDecodedBufferSize_slave(uint16_t size) {
	return((uint8_t)((size&0xFF00)>>8));
}

uint16_t aci_calculateClockDelay_us(uint16_t kHz) {
	return(1000/kHz);
}

void aciSlaveSet_portBit(ACISlave* aciSlave,uint8_t line,uint8_t state) {
	aciSlave->portBit[aci_calculateLineIndex_portBit(line)]&=~aci_calculateLineMask_portBit(line);
	aciSlave->portBit[aci_calculateLineIndex_portBit(line)]|=(state<<aci_calculateLineShift_portBit(line));
}

void aciSlaveSet_port(ACISlave* aciSlave,uint8_t line,volatile uint8_t* state) {
	aciSlave->port[line] = state;
}

uint8_t aciSlaveGet_portBit(ACISlave* aciSlave,uint8_t line) {
	return((aciSlave->portBit[aci_calculateLineIndex_portBit(line)]&aci_calculateLineMask_portBit(line))>>aci_calculateLineShift_portBit(line));
}

volatile uint8_t* aciSlaveGet_port(ACISlave* aciSlave,uint8_t line) {
	return(aciSlave->port[line]);
}

void aciSlaveSet_line(ACISlave* aciSlave,uint8_t line,volatile uint8_t* port,uint8_t bit) {
	aciSlaveSet_portBit(aciSlave,line,bit);
	aciSlaveSet_port(aciSlave,line,port);
}

uint8_t aciSlaveGet_line_portBit(ACISlave* aciSlave,uint8_t line) {
	return(aciSlaveGet_portBit(aciSlave,line));
}

volatile uint8_t* aciSlaveGet_line_port(ACISlave* aciSlave,uint8_t line) {
	return(aciSlaveGet_port(aciSlave,line));
}

void aciSlave_setLineState(ACISlave* aciSlave,uint8_t line,uint8_t state) {
	if(state) {
		*aciSlaveGet_port(aciSlave,line)|=_aciBV(aciSlaveGet_portBit(aciSlave,line));
	}
	else {
		*aciSlaveGet_port(aciSlave,line)&=~_aciBV(aciSlaveGet_portBit(aciSlave,line));
	}
}

uint8_t aciSlave_getLineState(ACISlave* aciSlave,uint8_t line) {
	return((*aciSlaveGet_port(aciSlave,line)&_aciBV(aciSlaveGet_portBit(aciSlave,line)))>>aciSlaveGet_portBit(aciSlave,line));
}

uint8_t aciSlave_checkForRequest(ACISlave* aciSlave) {
	if(aciSlave_getLineState(aciSlave,ACISLAVE_REQUEST_LINE)) {
		if(aciSlave_getLineState(aciSlave,ACISLAVE_MOSI_LINE)) {
			return(ACI_REQUEST_SEND_RECEIVE);
		}
		else {
			return(ACI_REQUEST_SEND);
		}
	}
	return(ACI_REQUEST_NONE);
}

void aciSlave_waitForClockHigh(ACISlave* aciSlave) {
	while(1) {
		if(aciSlave_getLineState(aciSlave,ACISLAVE_CLOCK_LINE)) {
			break;
		}
	}
}

void aciSlave_waitForClockLow(ACISlave* aciSlave) {
	while(1) {
		if(aciSlave_getLineState(aciSlave,ACISLAVE_CLOCK_LINE) == 0) {
			break;
		}
	}
}

uint8_t aciSlave_r_byte(ACISlave* aciSlave) {
	uint8_t byte = 0;
	for(uint8_t i = 0;i < 8;i++) {
		aciSlave_waitForClockHigh(aciSlave);
		if(aciSlave_getLineState(aciSlave,ACISLAVE_MOSI_LINE)) {
			byte|=_aciBV(i);
		}
		aciSlave_waitForClockLow(aciSlave);
	}
	return(byte);
}

void aciSlave_r_dataBufferSize(ACISlave* aciSlave,uint8_t* master) {
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,1);
	aciSlave_waitForClockHigh(aciSlave);
	aciSlave_waitForClockLow(aciSlave);
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
	*master = aciSlave_r_byte(aciSlave);
}

void aciSlave_r_dataBuffer(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer) {
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,1);
	for(uint8_t i = 0;i < bufferSize;i++) {
		buffer[i] = aciSlave_r_byte(aciSlave);
	}
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
	aciSlave_waitForClockHigh(aciSlave);
	aciSlave_waitForClockLow(aciSlave);
}

uint8_t aciSlave_rs_byteReceive(ACISlave* aciSlave) {
	uint8_t byte = 0;
	for(uint8_t i = 0;i < 8;i++) {
		aciSlave_waitForClockHigh(aciSlave);
		if(aciSlave_getLineState(aciSlave,ACISLAVE_MOSI_LINE)) {
			byte|=_aciBV(i);
		}
		aciSlave_waitForClockLow(aciSlave);
	}
	return(byte);
}

void aciSlave_rs_byteSend(ACISlave* aciSlave,uint8_t byte) {
	for(uint8_t i = 0;i < 8;i++) {
		aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,(byte&_aciBV(i)));
		aciSlave_waitForClockHigh(aciSlave);
		aciSlave_waitForClockLow(aciSlave);
	}
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
}

void aciSlave_rs_dataBufferSize(ACISlave* aciSlave,uint8_t* master,uint8_t* slave) {
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,1);
	aciSlave_waitForClockHigh(aciSlave);
	aciSlave_waitForClockLow(aciSlave);
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
	*master = aciSlave_rs_byteReceive(aciSlave);
	*slave = aciSlave_rs_byteReceive(aciSlave);
}

void aciSlave_rs_dataBufferReceive(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer) {
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,1);
	for(uint8_t i = 0;i < bufferSize;i++) {
		buffer[i] = aciSlave_rs_byteReceive(aciSlave);
	}
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
}

void aciSlave_rs_dataBufferSend(ACISlave* aciSlave,uint8_t bufferSize,uint8_t* buffer) {
	for(uint8_t i = 0;i < bufferSize;i++) {
		aciSlave_rs_byteSend(aciSlave,buffer[i]);
	}
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
	aciSlave_waitForClockHigh(aciSlave);
	aciSlave_waitForClockLow(aciSlave);
}

void aciSlave_init(ACISlave* aciSlave) {
	aciSlave_setLineState(aciSlave,ACISLAVE_MISO_LINE,0);
}

volatile uint8_t aciSlave_DDR(ACISlave* aciSlave) {
	volatile uint8_t ddr = 0;
	ddr|=_aciBV(aciSlaveGet_portBit(aciSlave,ACISLAVE_MISO_LINE));
	return(ddr);
}

void aciSlave_clear(ACISlave* aciSlave) {
	for(uint8_t i = 0;i < 2;i++) {
		aciSlave->portBit[i] = 0;
	}
}

void aciSlave_autoBindToPort(ACISlave* aciSlave,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit) {
	aciSlaveSet_line(aciSlave,ACISLAVE_CLOCK_LINE,portRead,(startBit+0));
	aciSlaveSet_line(aciSlave,ACISLAVE_MOSI_LINE,portRead,(startBit+1));
	aciSlaveSet_line(aciSlave,ACISLAVE_MISO_LINE,portWrite,(startBit+2));
	aciSlaveSet_line(aciSlave,ACISLAVE_REQUEST_LINE,portRead,(startBit+3));
}
