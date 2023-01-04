/*
 * PWM.c
 *
 * Created: 18/09/2017 10:52:06
 *  Author: Maria Negri
 */

#define F_CPU 1600000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	attachLcd(display);
	attachKeypad(keypad);
	uint8 tecla;
	uint8 vet[4];

	keypadSetPort(&keypad, &DDRD, &PORTD, &PIND, PD0, &DDRD, &PORTD, PD4);
	keypadSetKeys(&keypad, KEYPAD_4X4, 0X07, 0X04, 0X01, 0X0E,
	              0X08, 0X05, 0X02, 0X00,
	              0X09, 0X06, 0X03, 0X0F,
	              0X0A, 0X0B, 0X0C, 0X0D);
	keypadInit(&keypad, 10);

	lcdSetControlPort(&display, &DDRC, &PORTC, PC2, PC0);
	lcdSetDataPort(&display,  &DDRB, &PORTB, PB0);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);

	//Timer configutation
	TCCR1A = 0b10000110;
	TCCR1B = 0b00011011;
	ICR1 = 4999;
	DDRB |= 1 << PB1;

	printf("Servo Motor PWM\n");
	_delay_ms(2000);
	lcdClearScreen(&display);
	while(1) {
		//OCR0A = (OCR0A < 240)?(OCR0A + 16) : 0;
		OCR1A = 230; //-90 degree
		_delay_ms(8000);
		OCR1A = 500; //90 degree
		_delay_ms(8000);
		OCR1A = 375; //0 degree
		_delay_ms(8000);
		tecla = keypadRead(&keypad);
		if(tecla != 0xFF) {
			switch(tecla) {
			case 0:
				printf("0\n");
				break;
			case 1:
				printf("1\n");
				break;
			case 2:
				printf("2\n");
				break;
			case 3:
				printf("3\n");
				break;
			case 4:
				printf("4\n");
				break;
			case 5:
				printf("5\n");
				break;
			case 6:
				printf("6\n");
				break;
			case 7:
				printf("7\n");
				break;
			case 8:
				printf("8\n");
				break;
			case 9:
				printf("9\n");
				break;
			}
		}

	}
}