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
#include "ff.h"
#include <string.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	FATFS sdCard;
	FRESULT res;
	FIL file;
	int16 aux16 = 0;
	char string[201];

	// Variable initialization
	for(aux16 = 0; aux16 < 201; aux16++) {
		string[aux16] = 'A';
	}
	string[200] = '\0';

	// USART configuration
	usartConfig(USART_MODE_ASYNCHRONOUS, USART_BAUD_57600, USART_DATA_BITS_8, USART_PARITY_NONE, USART_STOP_BIT_SINGLE);
	usartEnableTransmitter();
	usartStdio();
	printf("USART OK\r\r");

	// SD card mount
	res = f_mount(0, &sdCard);
	if(res == FR_OK) {
		printf("Cartao montado!\r");
	}
	res = f_open(&file, "maria.bru", FA_WRITE | FA_CREATE_ALWAYS);
	if(res == FR_OK) {
		printf("Arquivo criado!\r");
	}

//	printf("Foram escritos %d bytes no arquivo!\r", aux16);
	//f_close(&file);

	// Enable Global Interrupts
	sei();

	DDRC = 0xFF;

	while(1) {
		setBit(PORTC, PC0);
		aux16 = f_printf(&file, "%s", string);
		clrBit(PORTC, PC0);
		setBit(PORTC, PC1);
//sprintf(string, )
		aux16 = f_puts(string, &file);
		clrBit(PORTC, PC1);
		setBit(PORTC, PC2);
		res = f_write(&file, string, strlen(string), (unsigned int *)&aux16);
		clrBit(PORTC, PC2);
		setBit(PORTC, PC3);
		for(aux16 = 0; aux16 < 200; aux16++) {
			f_putc(string[aux16], &file);
		}
		clrBit(PORTC, PC3);
		f_sync(&file);
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------