/*
 * AP_buttonLcdBuzzer.c
 *
 * Created: 28/09/2017 19:02:41
 *  Author: Enzzo Comassetto dos Santos
 */

#include "globalDefines.h"
#include "lcd4d.h"
#include <avr/io.h>
#include <stdlib.h>

//Variaveis Globais
volatile unsigned int tst0 = 0;
volatile unsigned int tst1 = 0;
volatile unsigned int tst2 = 0;


volatile unsigned int dificuldade = 4;
volatile unsigned int count = 0;

volatile unsigned int botaoS = 0;
volatile unsigned int botaoI = 0;
volatile unsigned int botaoD = 0;

int main(void)
{
	//Variaveis
	unsigned int senha[4] = {1, 2, 3, 4};
	unsigned int tentativa[4] = {1, 1, 1, 1};
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int y = 0;
	unsigned int x = 0;
	unsigned int n = -1;
	unsigned int acertou = 1;
	unsigned int valido = 1;


	attachLcd(display);
	lcdSetControlPort(&display, &DDRC, &PORTC, PC2, PC0);
	lcdSetDataPort(&display, &DDRB, &PORTB, PB0);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);

	//Pull-up configuration
	setBit(PORTD, PD0);
	setBit(PORTD, PD1);
	setBit(PORTD, PD2);
	setBit(PCICR, PCIE2);
	setBit(PCMSK2, PCINT16);
	setBit(PCMSK2, PCINT17);
	setBit(PCMSK2, PCINT18);

	// timer interrupt
	TCCR0A |= (1 << WGM01);
	OCR0A = 0x09;
	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect
	count = 1;
	sei();
	TCCR0B |= (1 << CS02);

	while(1) {
		if(acertou == 1) {
			lcdClearScreen(&display);
			printf("Selecione a \ndificuldade!");
			_delay_ms(2000);
			lcdClearScreen(&display);
			while(botaoS != 1) { //Seleção Dificuldade
				if(botaoI == 1) {
					dificuldade ++;
					botaoI = 0;
					if(dificuldade == 10) {
						dificuldade = dificuldade - 6;
					}
					lcdClearScreen(&display);
				}
				printf("dificuldade :  %d", dificuldade);
			}
			botaoS = 0;
			lcdClearScreen(&display);
			tentativa[0] = 1;
			tentativa[1] = 1;
			tentativa[2] = 1;
			tentativa[3] = 1;
			n = -1;
			y = 0;
			x = 0;
			acertou = 0;
			srand(count);

////////////////////////////////////////////////////////////////////////////////
// Realce de código – geração de senha secreta /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
			while(i < 4) {
				senha[i] = 1 + (rand() % dificuldade);
				if(i == 1) {
					while(senha[i] == senha[i - 1]) {
						senha[i] = 1 + (rand() % dificuldade);
					}
				}
				if(i == 2) {
					while(senha[i] == senha[i - 1] || senha[i] == senha[i - 2]) {
						senha[i] = 1 + (rand() % dificuldade);
					}
				}
				if(i == 3) {
					while(senha[i] == senha[i - 1] || senha[i] == senha[i - 2] || senha[i] == senha[i - 3]) {
						senha[i] = 1 + (rand() % dificuldade);
					}
				}
				i++;
			}
////////////////////////////////////////////////////////////////////////////////
// Fim do Realce ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
		}


		i = 0;
		n++;
		while(i < 4) { //Seleção Senha
			if(botaoI == 1) {
				tentativa[i]++;
				if(tentativa[i] > dificuldade) {
					tentativa[i] = 1;
				}
				botaoI = 0;
			}
			if(botaoS == 1) {
				i++;
				botaoS = 0;
			}
			//lcdClearScreen(&display);
			printf("Senha: %d%d%d%d     ", tentativa[0], tentativa[1], tentativa[2], tentativa[3]);
			printf("\n%d---->%d.%d        \n", n, x, y);

			while(botaoD == 1) { //Debug
				printf("A Senha eh:\n%d%d%d%d\n", senha[0], senha[1], senha[2], senha[3]);
			}
			if(i == 4) { //Verificar se a senha digitada é valida
				i = 0;
				j = 0;
				while(i < 4) {
					j = i + 1;
					while(j < 4) {
						if(tentativa[i] == tentativa[j]) {
							valido = 0;
						}
						j++;
					}
					i++;
				}
				if(valido == 0) {
					lcdClearScreen(&display);
					printf("Senha: XXXX\n\n");
					_delay_ms(2000);
					i = 0;
					j = 0;
					valido = 1;
				} else {
					i = 4;
					j = 0;
				}
			}
		}
		lcdClearScreen(&display);
		i = 0;
		x = 0;
		j = 0;
		y = 0;
		while(j < 4) { //Verifica acertos
			while(i < 4) {
				if(senha[i] == tentativa[j]) {
					if(i == j) { //Caso a posição seja igual soma em y
						y++;
					} else { //Se o numero existir mas nao for na posição certa soma em x
						x++;
					}
				}
				i++;
			}
			i = 0;
			j++;
		}
		i = 0;
		j = 0;
		if(y == 4) {//Mensagem de Acerto
			lcdClearScreen(&display);
			printf("Parabens!\nSenha Correta\n");
			setBit(PORTC, PC3);
			_delay_ms(1000);
			clrBit(PORTC, PC3);
			_delay_ms(500);
			setBit(PORTC, PC3);
			_delay_ms(1000);
			clrBit(PORTC, PC3);
			acertou = 1;
		}
	}
}

ISR(PCINT2_vect)
{
	_delay_ms(25);  //antirepique
	if(isBitSet(PIND, PD0) && (tst0 == 1)) {

		botaoS = 1;
		tst0 = 0;
	}
	if(isBitClr(PIND, PD0)) {
		tst0 = 1;
	}
	if(isBitSet(PIND, PD1) && (tst1 == 1)) {
		botaoI = 1;
		tst1 = 0;
	}
	if(isBitClr(PIND, PD1)) {
		tst1 = 1;
	}

	if(isBitClr(PIND, PD2)) {
		tst2 = 1;
		botaoD = 1;
	}
	if(isBitSet(PIND, PD2) && (tst2 == 1)) {
		botaoD = 0;
		tst2 = 0;
	}
}

ISR(TIMER0_COMPA_vect)
{
	count++;

	if(count >= 255) {
		count = 1;
	}
}