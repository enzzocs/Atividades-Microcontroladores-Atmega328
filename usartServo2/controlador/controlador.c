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
#include "ATmega328.h"
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

#define BAUD_RATE_COUNTER	103
#define ADC_SAMPLE_PERIOD	100  // ms

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union systemFlags_t{
	uint8 allFlags;
	struct{
		bool_t	transmit	: 1;
		uint8	unusedFlags	: 7;
	};
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

volatile uint16 rawAdc;
systemFlags_t systemFlags;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	int8 angle = 0;
	uint8 aux8 = 0;

	// Variable initialization
	systemFlags.allFlags = 0;
	
	// USART configuration
	usartConfig(USART_MODE_ASYNCHRONOUS, USART_BAUD_9600, USART_DATA_BITS_8, USART_PARITY_NONE, USART_STOP_BIT_SINGLE);
	usartEnableTransmitter();

	// TIMER1 configuration
	timer1SetCompareAValue(1561);
	timer1SetCompareBValue(1561);
	timer1Config(TIMER_B_MODE_CTC, TIMER_A_PRESCALER_1024);

	// ADC confoguration
	adcConfig(ADC_MODE_AUTO_TIMER1_COMPB, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_128);
	adcEnable();
	adcClearInterruptRequest();
	adcActivateInterrupt();
	timer1ClearCompareAInterruptRequest();

	// Enable Global Interrupts
	sei();

	while(1){
		if(systemFlags.transmit){
			// Convert adc to angle
			angle = (int8)(((rawAdc * 45) / 256) - 90);
			aux8 = 0;
//			if(angle < 0){
//				setBit(aux8, 7);		// Negative signal
//				aux8 |= abs(angle);
//			}else{
//				aux8 = angle;
//			}
			// Transmit data
			usartTransmit(angle);
			systemFlags.transmit = FALSE;
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(ADC_vect)
{
	rawAdc = ADC;
	timer1ClearCompareAInterruptRequest();
	systemFlags.transmit = TRUE;
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
