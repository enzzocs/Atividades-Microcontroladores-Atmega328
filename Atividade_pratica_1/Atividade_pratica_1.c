/*
 * Atividade_pratica_1.c
 *
 * Created: 05/09/2017 14:51:30
 *  Author: Maria Negri
 */ 


#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned int funcao_decod(unsigned int cont);

// Macros funções (BIT)
#define setBit(var, bit)   (var= var|(1<<(bit)))
#define clrBit(var, bit)   (var= var & ~(1<<(bit)))
#define invertBit(var, bit)   (var= var ^(1<<(bit)))
#define testBitSet(var, bit)   (var & (1<<bit))
#define testBitClean(var, bit)  !(var & (1<<bit))

volatile unsigned int number = 1802;

volatile unsigned int aux1 = 1;
volatile unsigned int aux2 = 1;
volatile unsigned int aux3 = 1;
volatile unsigned int aux4 = 1;

volatile unsigned int led;

int main(void)
{
	unsigned int segments[4];
	unsigned int i = 0;


	// Display configuration
	DDRD = 0xFF;
	DDRC = 0x0F;
	PORTC = 0x0F;

	//Pull-up configuration
	setBit(PORTB, PB0);
	setBit(PORTB, PB1);
	setBit(PORTB, PB2);
	setBit(PORTB, PB3);
	
	// PCINT configuration for interruption
	setBit(PCICR, PCIE0);     	 // Chave para interrupção (PCINT0 -- PCINT7)
	setBit(PCMSK0, PCINT0);		//Chave individual PCINT0 (milhar)
	setBit(PCMSK0, PCINT1);		//Chave individual PCINT1 (centena)
	setBit(PCMSK0, PCINT2);		//Chave individual PCINT2 (dezena)
	setBit(PCMSK0, PCINT3);		//Chave individual PCINT3 (unidade)

	// Enables global interrupts
	sei();

	while(1) {
		
		segments[3] = funcao_decod((number / 1000) % 10);
		segments[2] = funcao_decod((number / 100) % 10);
		segments[1] = funcao_decod((number / 10) % 10);
		segments[0] = funcao_decod(number % 10);
		led = funcao_decod((number / 10000) % 10);
		
		
		////////////////////////////////////////////////////////////////////////////////
		// Realce de código – estouro cíclico do contador //////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		
		if (number > 9999){
			number = number%1000;
		}

		////////////////////////////////////////////////////////////////////////////////
		// Fim do Realce ///////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		
		
		
		setBit(DDRB, PB4); // PB4 como saida
		/// Teste do estouro cíclico do contador/////
		if ((number/10000) != 0){
			setBit(PORTB, PB4);
		}
		
		
		// print no display multiplexado //
		for(i = 0; i < 4; i++) {
			PORTC = 0x0F;
			PORTD = segments[i];
			clrBit(PORTC, PC0 + i);
			_delay_ms(5);
		}
	}
	return 0;
}

ISR(PCINT0_vect)
{
	if(testBitSet(PINB, PB3)&& (aux1!=1)) {
		number++;
		aux1=1;
	}
	if(testBitClean(PINB, PB3)) {
		aux1=0;
	}
	if(testBitSet(PINB, PB2)&& (aux2!=1)) {
		number += 10;
		aux2=1;
	}
	if(testBitClean(PINB, PB2)) {
		aux2=0;
	}
	if(testBitSet(PINB, PB1) && (aux3!=1)){
		number += 100;
		aux3 = 1;
	}
	if(testBitClean(PINB, PB1)){
		aux3 = 0;
	}
	if(testBitSet(PINB, PB0) && (aux4!=1)){
		number += 1000;
		aux4 = 1;
	}
	if(testBitClean(PINB, PB0)){
		aux4 = 0;
	}
}

unsigned int funcao_decod(unsigned int cont)
{
	switch (cont){
		case 0:
		return 0xC0;
		case 1:
		return 0xF9;
		case 2:
		return 0xA4;
		case 3:
		return 0xB0;
		case 4:
		return 0x99;
		case 5:
		return 0x92;
		case 6:
		return 0x83;
		case 7:
		return 0xF8;
		case 8:
		return 0x80;
		case 9:
		return 0x98;
		
	}
	return 0;
}