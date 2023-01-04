/* -----------------------------------------------------------------------------
 * Project:     Project Name
 * File:        fileName.c
 * Author:      Project Author
 * Created:		2017-03-16
 * Modified:	2017-03-16
 * Version:     1.0
 * Purpose:     What this project does.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include <avr/io.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union systemFlags_t{
	uint8 allFlags;
	struct{
		bool_t	dataReady		: 1;
		uint8	packageIndex	: 4;
		uint8	unusedFlags		: 3;
	};
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

systemFlags_t systemFlags;
volatile int8 data = 0;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	int16 aux16 = 0;

	// Variable initialization
	systemFlags.allFlags = 0;
	
	// USART bitbang
	setBit(PORTB, PB2);					// Pull-Up
	setMask(MCUCR, 0x02, ISC00);		// Falling edge
	setBit(GIFR, INTF0);				// Clear INT0 previous requests
	setBit(GIMSK, INT0);				// Enable INT0

	// TIMER1 configuration
	setMask(TCCR1A, 0x02, COM1A0);
	setMask(TCCR1A, 0x02, WGM10);
	setMask(TCCR1B, 0x03, WGM12);
	setMask(TCCR1B, 0x03, CS10);
	OCR1A = 250;
	ICR1 = 5000;

	// Enable Global Interrupts
	sei();
DDRA = 0xFF;
	while(1){
		if(systemFlags.dataReady){
			aux16 = data;
			aux16 += 90;
			aux16 *= 25;
			aux16 /= 18;
			aux16 += 250;
			OCR1A = (uint16)aux16;
			systemFlags.dataReady = FALSE;
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(INT0_vect)
{
	// Timer para 52us
	setMask(TCCR0A, 0x02, WGM00);
	setMask(TCCR0B, 0x03, CS00);
	OCR0A = 13;
	TCNT0 = 0;
	setBit(TIFR0, OCF0A);
	setBit(TIMSK0, OCIE0A);
	// Desabilita INT0
	clrBit(GIMSK, INT0);
	// Zera index
	systemFlags.packageIndex = 0;
	data = 0;
}

ISR(TIM0_COMPA_vect)
{
	switch(systemFlags.packageIndex){
	case 0:		// Read start bit
		if(isBitClr(PINB, PB2)){
			OCR0A = 26;
		}else{
			// Cancela recepção;
		}
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:	// Read data
		if(isBitSet(PINB, PB2)){
			setBit(data, systemFlags.packageIndex - 1);
		}
		break;
	case 9:	// Read stop
		if(isBitSet(PINB, PB2)){
			TCCR0B = 0;			// Desliga timer
			setBit(GIFR, INTF0);
			setBit(GIMSK, INT0);
			systemFlags.dataReady = TRUE;
			}else{
			// Cancela recepção;
		}
		break;
	}
	systemFlags.packageIndex++;
}


// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
