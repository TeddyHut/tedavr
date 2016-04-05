#include "../include/hd44780.h"

uint8_t _hd44780BV(uint8_t bit) {
	return(1<<bit);
}

uint8_t hd44780_calculateLineIndex_portBit(uint8_t line) {
	return(line/2);
}

uint8_t hd44780_calculateLineIndex_port(uint8_t line) {
	return(line);
}

uint8_t hd44780_calculateLineMask_portBit(uint8_t line) {
	if(line%2) {
		return(0xF0);
	}
	else {
		return(0x0F);
	}
	return(0);
}

uint8_t hd44780_calculateLineMask_port(uint8_t line) {
	return(0xFF);
}

uint8_t hd44780_calculateLineShift_portBit(uint8_t line) {
	if(line%2) {
		return(4);
	}
	else {
		return(0);
	}
	return(0);
}

uint8_t hd44780_calculateLineShift_port(uint8_t line) {
	return(0);
}

void hd44780Set_portBit(HD44780* hd44780,uint8_t line,uint8_t state) {
	hd44780->portBit[hd44780_calculateLineIndex_portBit(line)]&=~(hd44780_calculateLineMask_portBit(line));
	hd44780->portBit[hd44780_calculateLineIndex_portBit(line)]|=(state<<hd44780_calculateLineShift_portBit(line));
}

void hd44780Set_port(HD44780* hd44780,uint8_t line,volatile uint8_t* state) {
	hd44780->port[line] = state;
}

uint8_t hd44780Get_portBit(HD44780* hd44780,uint8_t line) {
	return((hd44780->portBit[hd44780_calculateLineIndex_portBit(line)]&hd44780_calculateLineMask_portBit(line))>>hd44780_calculateLineShift_portBit(line));
}

volatile uint8_t* hd44780Get_port(HD44780* hd44780,uint8_t line) {
	return(hd44780->port[line]);
}

void hd44780Set_line(HD44780* hd44780,uint8_t line,volatile uint8_t* port,uint8_t bit) {
	hd44780Set_portBit(hd44780,line,bit);
	hd44780Set_port(hd44780,line,port);
}

uint8_t hd44780Get_line_portBit(HD44780* hd44780,uint8_t line) {
	return(hd44780Get_portBit(hd44780,line));
}

volatile uint8_t* hd44780Get_line_port(HD44780* hd44780,uint8_t line) {
	return(hd44780Get_port(hd44780,line));
}

void hd44780_clear(HD44780* hd44780) {
	for(uint8_t i = 0;i < 3;i++) {
		hd44780->portBit[i] = 0;
	}
}

void hd44780_autoBindToPort(HD44780* hd44780,volatile uint8_t* port,uint8_t startBit) {
	for(uint8_t i = 0;i < 6;i++) {
		hd44780Set_line(hd44780,i,port,(i+startBit));
	}
}

void hd44780_setLineState(HD44780* hd44780,uint8_t line,uint8_t state) {
	if(state) *hd44780->port[line]|=_hd44780BV(hd44780Get_line_portBit(hd44780,line));
	else *hd44780->port[line]&=~(_hd44780BV(hd44780Get_line_portBit(hd44780,line)));
}

void hd44780_setOutput(HD44780* hd44780,uint8_t byte) {
	for(uint8_t i = 0;i < 6;i++) {
		hd44780_setLineState(hd44780,i,(byte&_hd44780BV(i))>>i);
	}
}

void hd44780_sendNibble(HD44780* hd44780,uint8_t registerSelect,uint8_t nibble) {
	nibble&=0x0F;
	registerSelect&=0x01;
	hd44780_setOutput(hd44780,(_hd44780BV(HD44780_ENABLE)|(registerSelect<<HD44780_REGISTER_SELECT)|nibble));
	_delay_ms(1);
	hd44780_setOutput(hd44780,((registerSelect<<HD44780_REGISTER_SELECT)|nibble));
	_delay_ms(1);
	hd44780_setOutput(hd44780,0);
}

void hd44780_sendByte(HD44780* hd44780,uint8_t registerSelect,uint8_t byte) {
	hd44780_sendNibble(hd44780,registerSelect,(byte&0xF0)>>4);
	hd44780_sendNibble(hd44780,registerSelect,byte&0x0F);
}

void hd44780_turnDisplayOn(HD44780* hd44780,uint8_t cursor) {
	hd44780_sendNibble(hd44780,0,0b0000);
	hd44780_sendNibble(hd44780,0,(0b1100|cursor));
	_delay_ms(5);
}

void hd44780_turnDisplayOff(HD44780* hd44780) {
	hd44780_sendNibble(hd44780,0,0b0000);
	hd44780_sendNibble(hd44780,0,0b1000);
	_delay_ms(5);
}

void hd44780_setShift(HD44780* hd44780,uint8_t shift) {
	hd44780_sendNibble(hd44780,0,0b0000);
	hd44780_sendNibble(hd44780,0,(0b0100|shift));
}

void hd44780_clearDisplay(HD44780* hd44780) {
	hd44780_sendNibble(hd44780,0,0b0000);
	hd44780_sendNibble(hd44780,0,0b0001);
}

void hd44780_setPosition(HD44780* hd44780,uint8_t pos) {
	pos&=0b01111111;
	hd44780_sendByte(hd44780,0,(0b10000000|pos));
}

void hd44780_init(HD44780* hd44780) {
	_delay_ms(15);
	hd44780_sendNibble(hd44780,0,0b0010); //4-Bit mode
	_delay_ms(5);
	hd44780_sendNibble(hd44780,0,0b0010);
	hd44780_sendNibble(hd44780,0,0b1000); //FunctionSet
}

uint8_t hd44780_calculateCursor(uint8_t on,uint8_t blink) {
	on&=0x01;
	blink&=0x01;
	return((on<<HD44780_CURSOR_ON)|(blink<<HD44780_CURSOR_BLINK));
}

uint8_t hd44780_calculateShift(uint8_t cursor,uint8_t display) {
	display&=0x01;
	cursor&=0x01;
	return((display<<HD44780_SHIFT_DISPLAY)|(cursor<<HD44780_SHIFT_CURSOR));
}

void hd44780_putText(HD44780* hd44780,char* text) {
	for(uint8_t i = 0;text[i] != '\0';i++) {
		hd44780_sendByte(hd44780,1,text[i]);
	}
}

void hd44780_putNumberi8(HD44780* hd44780,int8_t number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0'; 
	}
	sprintf(text,"%d",number);
	hd44780_putText(hd44780,text);
}

void hd44780_putNumberui8(HD44780* hd44780,uint8_t number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0';
	}
	sprintf(text,"%u",number);
	hd44780_putText(hd44780,text);
}

void hd44780_putNumberi16(HD44780* hd44780,int16_t number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0';
	}
	sprintf(text,"%d",number);
	hd44780_putText(hd44780,text);
}

void hd44780_putNumberui16(HD44780* hd44780,uint16_t number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0';
	}
	sprintf(text,"%u",number);
	hd44780_putText(hd44780,text);
}

void hd44780_putNumberf(HD44780* hd44780,float number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0';
	}
	sprintf(text,"%f",(double)number);
	hd44780_putText(hd44780,text);
}

void hd44780_putNumberd(HD44780* hd44780,double number) {
	char text[16];
	for(uint8_t i = 0;i < 16;i++) {
		text[i] = '\0';
	}
	sprintf(text,"%f",(double)number);
	hd44780_putText(hd44780,text);
}

void hd44780_putBinary(HD44780* hd44780,void* data,uint8_t size) {
	if(size == sizeof(uint8_t)) {
		uint8_t* binary = (uint8_t*)data;
		for(uint8_t i = 7;i < 8;i--) {
			hd44780_sendNibble(hd44780,1,0x03);
			hd44780_sendNibble(hd44780,1,(*binary&(1<<i))>>i);
		}
	}
	if(size == sizeof(uint16_t)) {
		uint16_t* binary = (uint16_t*)data;
		for(uint8_t i = 15;i < 16;i--) {
			hd44780_sendNibble(hd44780,1,0x03);
			hd44780_sendNibble(hd44780,1,(*binary&(1<<i))>>i);
		}
	}
}
