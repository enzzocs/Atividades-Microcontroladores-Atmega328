/*
 * buttonMuxDisplay.c
 *
 * Created: 28/08/2017 10:40:52
 *  Author: Enzzo Comassetto dos Santos
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

volatile unsigned int flag_delay = 0;
volatile unsigned int estouro = 0;
volatile unsigned int aux = 0;
volatile unsigned int saiu = 0;

unsigned char decode(unsigned int cont);

int main(void)
{
	unsigned char segments[4];
	unsigned int i = 0;

	// Display configuration
	DDRD = 0xFF;
	DDRC = 0x0F;
	PORTC = 0x0F;

	// PCINT configuration
	setBit(PORTB, PB0);
	setBit(PCICR, PCIE0);
	setBit(PCMSK0, PCINT0);

	//Timer
	OCR1A = 31249;
	TCCR1B |= (1 << WGM12);  // Mode 4, CTC on OCR1A
	TIMSK1 |= (1 << OCIE1A); //Set interrupt on compare match, com o prescaler e 1024 contara 2s

	//delay
	TCCR0A |= (1 << WGM01);     // Set the Timer Mode to CTC
	OCR0A = 0xF9;               // Set the value that you want to count to
	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

	sei();         //enable interrupts

	TCCR0B |= (1 << CS02);      // set prescaler to 256 and start the timer

	while(1) {
		if(saiu == 1) {

			if((estouro == 0) && (aux <= 7813)) {
				number++;
			}
			if((estouro == 0) && (aux >= 7813)) {
				number += 10;
			}
			if(estouro == 1) {
				number = 0;
			}
			saiu = 0;
		}
		segments[0] = decode(number % 10);
		segments[1] = decode((number / 10) % 10);
		segments[2] = decode((number / 100) % 10);
		segments[3] = decode((number / 1000) % 10);

		////////////////////////////////////////////////////////////////////////////////
		// Realce de código – estouro cíclico do contador //////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		if(number > 10000) {
			number -= 10000;
		}
		////////////////////////////////////////////////////////////////////////////////
		// Fim do Realce ///////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////


		if(flag_delay) {
			flag_delay = 0;
			if(i > 3) {
				i = 0;
			}
			PORTC = 0x0F;
			PORTD = segments[i];
			clrBit(PORTC, (PC0 + i));
			i++;
		}
	}
	return 0;
}

ISR(PCINT0_vect)
{
	if(testBitClr(PINB, PB0)) { //Botão pressionado
		TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
		estouro = 0;
		saiu = 0;
	} else {						 //Botão solto
		TCCR1B &= 0x00;
		aux = TCNT1;
		TCNT1 = 0;
		saiu = 1;
	}
}

ISR(TIMER1_COMPA_vect)
{
	estouro = 1;
}
ISR(TIMER0_COMPA_vect)
{
	flag_delay = 1;
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
