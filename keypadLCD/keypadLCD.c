/*
 * GccApplication1.c
 *
 * Created: 04/09/2017 10:39:48
 *  Author: Enzzo COmassetto dos Santos
 */

#include "globalDefines.h"
#include "keypad.h"
#include "lcd4d.h"



int main(void)
{
	attachLcd(display);
	attachKeypad(keypad);
	uint8 tecla;
	uint8 senha[4] = {1, 2, 3, 4};
	uint8 senha2[4] = {0, 0, 0, 0};
	int i = 0;
	int d = 0;

	keypadSetPort(&keypad, &DDRD, &PORTD, &PIND, PD0, &DDRD, &PORTD, PD4);
	keypadSetKeys(&keypad, KEYPAD_4X4,
	              0x07, 0x04, 0x01, 0x0E,
	              0x08, 0x05, 0x02, 0x00,
	              0x09, 0x06, 0x03, 0x0F,
	              0x0A, 0x0B, 0x0C, 0x0D);
	keypadInit(&keypad, 10);

	lcdSetControlPort(&display, &DDRC, &PORTC, PC2, PC0);
	lcdSetDataPort(&display, &DDRB, &PORTB, PB0);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);
	printf("   cofre ACME   \n1.0  04/09/2017");
	_delay_ms(1000);
	lcdClearScreen(&display);

	while(1) {

		tecla = keypadRead(&keypad);

		if(tecla != 0xFF) {
			switch(tecla) {
			case 0:
				printf("* ");
				senha2[i] = 0;
				i++;
				break;
			case 1:
				printf("* ");
				senha2[i] = 1;
				i++;
				break;
			case 2:
				printf("* ");
				senha2[i] = 2;
				i++;
				break;
			case 3:
				printf("* ");
				senha2[i] = 3;
				i++;
				break;
			case 4:
				printf("* ");
				senha2[i] = 4;
				i++;
				break;
			case 5:
				printf("* ");
				senha2[i] = 5;
				i++;
				break;
			case 6:
				printf("* ");
				senha2[i] = 6;
				i++;
				break;
			case 7:
				printf("* ");
				senha2[i] = 7;
				i++;
				break;
			case 8:
				printf("* ");
				senha2[i] = 8;
				i++;
				break;
			case 9:
				printf("* ");
				senha2[i] = 9;
				i++;
				break;
			case 0x0E:
				lcdClearScreen(&display);
				i = 0;
				d = 0;
				break;
			case 0x0F:
				i = 0;
				while(i < 4) {
					if(senha2[i] == senha[i]) {
						d++;
					}

					printf("%u", senha2[i]);
					i++;
					//printf("%i", d);
				}

				if(d == 4) {
					printf("\nSenha Correta");

				}
				if(d != 4) {
					printf("\nSenha Incorreta\n");

				}
				d = 0;
				break;
			}
		}
	}
}