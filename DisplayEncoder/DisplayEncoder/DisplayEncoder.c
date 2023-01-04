/*
 * buttonMuxDisplay.c
 *
 * Created: 28/08/2017 10:40:52
 *  Author: Enzzo
 */

#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define setBit(var, bit)		(var = var | (1<<bit))
#define clrBit(var, bit)		(var = var & ~(1<<bit))
#define invertBit(var, bit)		(var = var ^ (1<<bit))

#define testBitClr(var, bit)    !((var & (1 << bit)))
#define testBitSet(var, bit)	(var & (1 << bit))

volatile unsigned int number = 1802;

volatile unsigned int atual = 22;
volatile unsigned int anterior = 22;

unsigned char decode(unsigned int cont);

int main(void)
{
	unsigned char segments[4];
	unsigned char i = 0;

	// Display configuration
	DDRD = 0xFF;
	DDRB = 0x0F;
	PORTB = 0x0F;

	// PCINT configuration
	setBit(PORTC, PC0);
	setBit(PORTC, PC1);
	setBit(PCICR, PCIE1);
	setBit(PCMSK1, PCINT8);
	setBit(PCMSK1, PCINT9);

	// Enables global interrupts
	sei();

	while(1) {
		segments[0] = decode(number % 10);
		segments[1] = decode((number / 10) % 10);
		segments[2] = decode((number / 100) % 10);
		segments[3] = decode((number / 1000) % 10);

		for(i = 0; i < 4; i++) {
			PORTB = 0x0F;
			PORTD = segments[i];
			clrBit(PORTB, (PB0 + i));
			_delay_ms(5);
		}
	}
	return 0;
}

ISR(PCINT1_vect)
{
	if(testBitSet(PINC, PC0) && testBitClr(PINC, PC1)) {
		atual = 0b10;
		if(anterior == 0b00) {
			number++;
		}
		if(anterior == 0b11) {
			number--;
		}
		anterior = atual;
	}

	if(testBitClr(PINC, PC0) && testBitClr(PINC, PC1)) {
		atual = 0b00;
		if(anterior == 0b01) {
			number++;
		}
		if(anterior == 0b10) {
			number--;
		}
		anterior = atual;
	}

	if(testBitClr(PINC, PC0) && testBitSet(PINC, PC1)) {
		atual = 0b01;
		if(anterior == 0b11) {
			number++;
		}
		if(anterior == 0b00) {
			number--;
		}
		anterior = atual;
	}
	if(testBitSet(PINC, PC0) && testBitSet(PINC, PC1)) {
		atual = 0b11;
		if(anterior == 0b10) {
			number++;
		}
		if(anterior == 0b01) {
			number--;
		}
		anterior = atual;
	}
	////////////////////////////////////////////////////////////////////////////////
	// Realce de código – estouro cíclico do contador //////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	if(number > 10000) {
		number -= 10000;
	}
	////////////////////////////////////////////////////////////////////////////////
	// Fim do Realce ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
}

unsigned char decode(unsigned int cont)
{
	switch(cont) {
	case 0:
		return 0xC0;
	case 1:
		return 0xF9;
	case 2:
		return 0XA4;
	case 3:
		return 0XB0;
	case 4:
		return 0X99;
	case 5:
		return 0X92;
	case 6:
		return 0X83;
	case 7:
		return 0XF8;
	case 8:
		return 0X80;
	case 9:
		return 0X98;
	default:
		return 0;

	}
}