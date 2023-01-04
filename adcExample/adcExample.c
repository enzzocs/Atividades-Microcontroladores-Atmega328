/*
 * adcExample.c
 *
 * Created: 25/09/2017 10:24:21
 *  Author: csenz
 */


#include <avr/io.h>
#include "globalDefines.h"
#include "ATmega328.h"
#include "lcd4d.h"

#define DEBUG_DDR DDRD
#define DEBUG_PORT PORTD
#define DEBUG_BIT0 PD0
#define DEBUG_BIT1 PD1

typedef volatile union systemFlags_t {
	struct {
		uint8	conversionReady : 1;
		uint8	unusedFlags     : 7;
	};
	uint8 allFlags;
} systemFlags_t;

systemFlags_t systemFlags;
volatile uint16 rawADC;

int main(void)
{
	//Variable Declaration
	attachLcd(display);

	systemFlags.allFlags = 0;

	//DEBUG configuration
	setMask(DEBUG_DDR, 0x03, DEBUG_BIT0);
	clrMask(DEBUG_PORT, 0x03, DEBUG_BIT0);

	//LCD Configuration
	lcdSetControlPort(&display, &DDRB, &PORTB, PB4, PB5);
	lcdSetDataPort(&display, &DDRB, &PORTB, PB0);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);
	printf("adcExample v 1.0\n\n");
	_delay_ms(2000);
	lcdClearScreen(&display);

	//ADC Configuration
	adcConfig(ADC_MODE_AUTO_TIMER1_COMPB, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_128);
	adcSelectChannel(ADC_CHANNEL_0);
	adcDisableDigitalInput(ADC_CHANNEL_0);
	//adcDataPresentation(ADC_LEFT_ADJUSTED);
	adcEnable();
	adcActivateInterrupt();


	//Timer1
	timer1Config(TIMER_B_MODE_CTC_ICR, TIMER_A_PRESCALER_64);
	timer1SetInputCaptureValue(25000);
	timer1SetCompareBValue(25000);
	timer1ClearCompareBInterruptRequest();


	sei();
	while(1) {
		if(systemFlags.conversionReady) {
			printf("ADC = %d\n\n", ADC);
			systemFlags.conversionReady = 0;
		}

		//TODO:: Please write your application code
	}
}

ISR(ADC_vect)
{
	rawADC = ADC;
	timer1ClearCompareBInterruptRequest();
	systemFlags.conversionReady = 1;
}