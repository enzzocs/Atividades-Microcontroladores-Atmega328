/*
 * macros.c
 *
 * Created: 31/08/2017 15:16:22
 *  Author: Maria Negri
 */ 


#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define setBit(var, bit)		(var = var | (1<<bit))
#define clrBit(var, bit)		(var = var & ~(1<<bit))
#define invertBit(var, bit)		(var = var ^ (1<<bit))

#define testBitclr(var, bit)    !((var & (1 << bit)))	
#define testBitset(var, bit)	(var & (1 << bit))

unsigned char decode(unsigned char cont);


int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00;
	setBit(PORTD, 2);
	
	unsigned char cont;
	
	cont = '0';
	
    while(1)
    {
        //TODO:: Please write your application code 
		PORTD = decode(cont);
		_delay_ms(500);
		cont++;
		
		if(cont == 58){
			cont = '0';
		}
		
		
    }
}


unsigned char decode(unsigned char cont)
{
	switch (cont){
		case '0':
			return 0xC0;
		case '1':
			return 0xF9;
		case '2':
			return 0XA4;
		case '3':
			return 0XB0;
		case '4':
			return 0X99;
		case '5':
			return 0X92;
		case '6':
			return 0X83;
		case '7':
			return 0XF8;
		case '8':
			return 0X80;
		case '9':
			return 0X98;

	}
		
	
}

