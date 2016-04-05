#include <avr/io.h>
#include <util/delay.h>

void keypadInit(void);
char getKeypad(void);
char getPortKP(void);
char formatKeypad(char keypad);
char getKeypadChar(char key);

#define PORTKP PORTB
#define PINKP PINB
#define DDRKP DDRB

#define KP_0 bit_is_set(PINKP,0)
#define KP_1 bit_is_set(PINKP,1)
#define KP_2 bit_is_set(PINKP,2)
#define KP_3 bit_is_set(PINKP,3)

#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9
#define KEY_AST 10
#define KEY_HSH 11
#define KEY_A 12
#define KEY_B 13
#define KEY_C 14
#define KEY_D 15
#define KEY_NONE 255
