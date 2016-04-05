#include "../include/tedavr/aciMaster.h"

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

void aciMasterSet_clock_kHz(ACIMaster* aciMaster,uint16_t state) {
	aciMaster->clock_kHz = state;
}

void aciMasterSet_slaveWidth(ACIMaster* aciMaster,uint8_t state) {
	aciMaster->slaveWidth = state;
}

void aciMasterSet_portBit(ACIMaster* aciMaster,uint8_t line,uint8_t state) {
	aciMaster->portBit[aci_calculateLineIndex_portBit(line)]&=~aci_calculateLineMask_portBit(line);
	aciMaster->portBit[aci_calculateLineIndex_portBit(line)]|=(state<<aci_calculateLineShift_portBit(line));
}

void aciMasterSet_port(ACIMaster* aciMaster,uint8_t line,volatile uint8_t* state) {
	aciMaster->port[line] = state;
}

uint16_t aciMasterGet_clock_kHz(ACIMaster* aciMaster) {
	return(aciMaster->clock_kHz);
}

uint8_t aciMasterGet_slaveWidth(ACIMaster* aciMaster) {
	return(aciMaster->slaveWidth);
}

uint8_t aciMasterGet_portBit(ACIMaster* aciMaster,uint8_t line) {
	return((aciMaster->portBit[aci_calculateLineIndex_portBit(line)]&aci_calculateLineMask_portBit(line))>>aci_calculateLineShift_portBit(line));
}

volatile uint8_t* aciMasterGet_port(ACIMaster* aciMaster,uint8_t line) {
	return(aciMaster->port[line]);
}

void aciMasterSet_line(ACIMaster* aciMaster,uint8_t line,volatile uint8_t* port,uint8_t portBit) {
	aciMasterSet_portBit(aciMaster,line,portBit);
	aciMasterSet_port(aciMaster,line,port);
}

uint8_t aciMasterGet_line_portBit(ACIMaster* aciMaster,uint8_t line) {
	return(aciMasterGet_portBit(aciMaster,line));
}

volatile uint8_t* aciMasterGet_line_port(ACIMaster* aciMaster,uint8_t line) {
	return(aciMasterGet_port(aciMaster,line));
}

void aciMaster_setLineState(ACIMaster* aciMaster,uint8_t line,uint8_t state) {
	if(state) {
		*aciMasterGet_port(aciMaster,line)|=_aciBV(aciMasterGet_portBit(aciMaster,line));
	}
	else {
		*aciMasterGet_port(aciMaster,line)&=~_aciBV(aciMasterGet_portBit(aciMaster,line));
	}
}

uint8_t aciMaster_getLineState(ACIMaster* aciMaster,uint8_t line) {
	return((*aciMasterGet_port(aciMaster,line)&_aciBV(aciMasterGet_portBit(aciMaster,line)))>>aciMasterGet_portBit(aciMaster,line));
}

void aciMaster_request(ACIMaster* aciMaster,uint8_t slave,uint8_t type) {
	if(type == ACIMASTER_REQUEST_SEND) {
		aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
	}
	if(type == ACIMASTER_REQUEST_SEND_RECEIVE) {
		aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,1);
	}
	aciMaster_setLineState(aciMaster,slave,1);
}

void aciMaster_unrequest(ACIMaster* aciMaster,uint8_t slave) {
	aciMaster_setLineState(aciMaster,slave,0);
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
}

uint8_t aciMaster_checkForResponse(ACIMaster* aciMaster) {
	return(aciMaster_getLineState(aciMaster,ACIMASTER_MISO_LINE));
}

void aciMaster_clockDelay(ACIMaster* aciMaster,uint8_t divide) {
	aci_delay_us(((aci_calculateClockDelay_us(aciMasterGet_clock_kHz(aciMaster)))/2)/divide);
}

void aciMaster_setClockHigh(ACIMaster* aciMaster) {
	aciMaster_setLineState(aciMaster,ACIMASTER_CLOCK_LINE,1);
}

void aciMaster_setClockLow(ACIMaster* aciMaster) {
	aciMaster_setLineState(aciMaster,ACIMASTER_CLOCK_LINE,0);
}

void aciMaster_s_byte(ACIMaster* aciMaster,uint8_t byte) {
	for(uint8_t i = 0;i < 8;i++) {
		aciMaster_clockDelay(aciMaster,2);
		aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,(byte&_aciBV(i)));
		aciMaster_clockDelay(aciMaster,2);
		aciMaster_setClockHigh(aciMaster);
		aciMaster_clockDelay(aciMaster,1);
		aciMaster_setClockLow(aciMaster);
	}
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
}

void aciMaster_s_dataBufferSize(ACIMaster* aciMaster,uint8_t bufferSize) {
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockHigh(aciMaster);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockLow(aciMaster);
	aciMaster_s_byte(aciMaster,bufferSize);
}

void aciMaster_s_dataBuffer(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer) {
	while(aciMaster_getLineState(aciMaster,ACIMASTER_MISO_LINE) == 0);
	for(uint8_t i = 0;i < bufferSize;i++) {
		aciMaster_s_byte(aciMaster,buffer[i]);
	}
}

void aciMaster_s_endCommunication(ACIMaster* aciMaster,uint8_t slave) {
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
	aciMaster_setLineState(aciMaster,slave,0);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockHigh(aciMaster);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockLow(aciMaster);
}

void aciMaster_sr_byteSend(ACIMaster* aciMaster,uint8_t byte) {
	for(uint8_t i = 0;i < 8;i++) {
		aciMaster_clockDelay(aciMaster,2);
		aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,(byte&_aciBV(i)));
		aciMaster_clockDelay(aciMaster,2);
		aciMaster_setClockHigh(aciMaster);
		aciMaster_clockDelay(aciMaster,1);
		aciMaster_setClockLow(aciMaster);
	}
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
}

uint8_t aciMaster_sr_byteReceive(ACIMaster* aciMaster) {
	uint8_t byte = 0;
	for(uint8_t i = 0;i < 8;i++) {
		aciMaster_clockDelay(aciMaster,1);
		aciMaster_setClockHigh(aciMaster);
		aciMaster_clockDelay(aciMaster,2);
		if(aciMaster_getLineState(aciMaster,ACIMASTER_MISO_LINE)) {
			byte|=_aciBV(i);
		}
		aciMaster_clockDelay(aciMaster,2);
		aciMaster_setClockLow(aciMaster);
	}
	return(byte);
}

void aciMaster_sr_dataBufferSize(ACIMaster* aciMaster,uint8_t master,uint8_t slave) {
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockHigh(aciMaster);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockLow(aciMaster);
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
	aciMaster_sr_byteSend(aciMaster,master);
	aciMaster_sr_byteSend(aciMaster,slave);
}

void aciMaster_sr_dataBufferSend(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer) {
	while(aciMaster_getLineState(aciMaster,ACIMASTER_MISO_LINE) == 0);
	for(uint8_t i = 0;i < bufferSize;i++) {
		aciMaster_sr_byteSend(aciMaster,buffer[i]);
	}
}

void aciMaster_sr_dataBufferReceive(ACIMaster* aciMaster,uint8_t bufferSize,uint8_t* buffer) {
	for(uint8_t i = 0;i < bufferSize;i++) {
		buffer[i] = aciMaster_sr_byteReceive(aciMaster);
	}
}

void aciMaster_sr_endCommunication(ACIMaster* aciMaster,uint8_t slave) {
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
	aciMaster_setLineState(aciMaster,slave,0);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockHigh(aciMaster);
	aciMaster_clockDelay(aciMaster,1);
	aciMaster_setClockLow(aciMaster);
}

void aciMaster_init(ACIMaster* aciMaster) {
	for(uint8_t i = 0;i < 8;i++) {
		aciMaster_setLineState(aciMaster,i,0);
	}
	aciMaster_setLineState(aciMaster,ACIMASTER_CLOCK_LINE,0);
	aciMaster_setLineState(aciMaster,ACIMASTER_MOSI_LINE,0);
}

volatile uint8_t aciMaster_DDR(ACIMaster* aciMaster) {
	volatile uint8_t ddr = 0;
	ddr|=(_aciBV(aciMasterGet_portBit(aciMaster,ACIMASTER_CLOCK_LINE)));
	ddr|=(_aciBV(aciMasterGet_portBit(aciMaster,ACIMASTER_MOSI_LINE)));
	ddr&=~(_aciBV(aciMasterGet_portBit(aciMaster,ACIMASTER_MISO_LINE)));
	for(uint8_t i = 0;i < aci_calculateDecodedSlaveWidth(aciMasterGet_slaveWidth(aciMaster));i++) {
		ddr|=(_aciBV(aciMasterGet_portBit(aciMaster,i)));
	}
	return(ddr);
}

void aciMaster_clear(ACIMaster* aciMaster) {
	aciMaster->clock_kHz = 0;
	aciMaster->slaveWidth = 0;
	for(uint8_t i = 0;i < 6;i++) {
		aciMaster->portBit[i] = 0;
	}
}

void aciMaster_autoBindToPort(ACIMaster* aciMaster,volatile uint8_t* portWrite,volatile uint8_t* portRead,uint8_t startBit,uint8_t slaveWidth) {
	aciMasterSet_slaveWidth(aciMaster,slaveWidth);
	aciMasterSet_line(aciMaster,ACIMASTER_CLOCK_LINE,portWrite,(startBit+0));
	aciMasterSet_line(aciMaster,ACIMASTER_MOSI_LINE,portWrite,(startBit+1));
	aciMasterSet_line(aciMaster,ACIMASTER_MISO_LINE,portRead,(startBit+2));
	if(startBit < 5) {
		for(uint8_t i = 0;(i+startBit < 8)&&(i < aci_calculateDecodedSlaveWidth(slaveWidth));i++) {
			aciMasterSet_line(aciMaster,i,portWrite,(startBit+i+3));
		}
	}
}
