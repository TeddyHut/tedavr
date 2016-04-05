#include "../include/tedavr/keypad.h"

void keypadInit(void) {
	DDRKP = 0b11110000;
}

char getPortKP(void) {
	char number = 0;
	if(bit_is_set(PINKP,0)) number+=1;
	if(bit_is_set(PINKP,1)) number+=2;
	if(bit_is_set(PINKP,2)) number+=4;
	if(bit_is_set(PINKP,3)) number+=8;
	if(bit_is_set(PINKP,4)) number+=16;
	if(bit_is_set(PINKP,5)) number+=32;
	if(bit_is_set(PINKP,6)) number+=64;
	if(bit_is_set(PINKP,7)) number+=128;
	return(number);
}

char getKeypad(void) {
	char side[2] = {0,0};
	PORTKP = 0xF0;
	if((getPortKP()&0b00001111)) {
		for(char i = 0;i < 4;i++) {
			if(bit_is_set(PINKP,i)) {
				side[0] = i;
				break;
			}
		}
		for(char i = 0;i < 4;i++) {
			PORTKP = (0xFF-(0x10<<i));
			if(bit_is_clear(PINKP,side[0])) {
				side[1] = i;
				break;
			}
		}
		return(0|(side[0]&0b00001111)|(side[1]<<4));
	}
	return(KEY_NONE);
}

char formatKeypad(char keypad) {
	if(keypad == KEY_NONE) {
		return(KEY_NONE);
	}
	char side[2];
	side[0] = keypad&0b00001111;
	side[1] = (keypad&0b11110000)>>4;
	if((side[0] == 0x0)&&(side[1] == 0x0)) return(KEY_AST);
	if((side[0] == 0x1)&&(side[1] == 0x0)) return(KEY_0);
	if((side[0] == 0x2)&&(side[1] == 0x0)) return(KEY_HSH);
	if((side[0] == 0x3)&&(side[1] == 0x0)) return(KEY_D);
	if((side[0] == 0x0)&&(side[1] == 0x1)) return(KEY_1);
	if((side[0] == 0x1)&&(side[1] == 0x1)) return(KEY_2);
	if((side[0] == 0x2)&&(side[1] == 0x1)) return(KEY_3);
	if((side[0] == 0x3)&&(side[1] == 0x1)) return(KEY_A);
	if((side[0] == 0x0)&&(side[1] == 0x2)) return(KEY_4);
	if((side[0] == 0x1)&&(side[1] == 0x2)) return(KEY_5);
	if((side[0] == 0x2)&&(side[1] == 0x2)) return(KEY_6);
	if((side[0] == 0x3)&&(side[1] == 0x2)) return(KEY_B);
	if((side[0] == 0x0)&&(side[1] == 0x3)) return(KEY_7);
	if((side[0] == 0x1)&&(side[1] == 0x3)) return(KEY_8);
	if((side[0] == 0x2)&&(side[1] == 0x3)) return(KEY_9);
	if((side[0] == 0x3)&&(side[1] == 0x3)) return(KEY_C);
	
	return(0);
}

char getKeypadChar(char key) {
	if(key == KEY_0) return('0');
	if(key == KEY_1) return('1');
	if(key == KEY_2) return('2');
	if(key == KEY_3) return('3');
	if(key == KEY_4) return('4');
	if(key == KEY_5) return('5');
	if(key == KEY_6) return('6');
	if(key == KEY_7) return('7');
	if(key == KEY_8) return('8');
	if(key == KEY_9) return('9');
	if(key == KEY_AST) return('*');
	if(key == KEY_HSH) return('#');
	if(key == KEY_A) return('A');
	if(key == KEY_B) return('B');
	if(key == KEY_C) return('C');
	if(key == KEY_D) return('D');
	return(KEY_NONE);
}