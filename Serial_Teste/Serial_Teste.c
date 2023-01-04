/*
 * Serial_Teste.c
 *
 * Created: 10/11/2017 08:12:18
 *  Author: csenz
 */


#include <avr/io.h>
#include "globaldefines.h"
#include "ATmega328.h"
//#include "twiMaster.h"
//#include "ds1307.h"
typedef union systemFlags_t {
	struct {
		uint8 serialSend : 1;
		uint8 updateData : 1
		uint8 unusedBits : 6;
	};
	uint8 allFlags;
};

volatile
int main(void)
{
	uint8 minuto = 44, hora = 8, dia = 10, semana = 5, mes = 11, ano = 17
	                                   //int data[6] = {12, 23, 8, 10, 11, 2017};
	                                   ds1307Timerabreviation aux8 = 0;

	//PCINT23
	pcint23ActivateInterrupt(PORT_INPUT_PULL_UP);
	pcint23_16ClearInterruptRequest();
	pcint23_16Enable();

	// USART configuration
	usartConfig(USART_MODE_ASYNCHRONOUS, USART_BAUD_9600, USART_DATA_BITS_8, USART_PARITY_NONE, USART_STOP_BIT_SINGLE);
	usartEnableReceiver();
	usartEnableTransmitter();
	usartStdio()
	usartActivateReceptionCompleteInterrupt();
	sei();
	//TWI Config
	twiMasterInit(10000);
	ds1307SetControl(DS1307_COUNTING_RESUME, DS1307_CLOCK_1HZ, DS1307_FORMAT_24);

	while(1) {
		if(systemFlags.updateData) {
			ds1307GetData(&ano, &mes, &dia, &semana);
			ds1307GetTime(&hora, &minuto, &segundos, &aux8);
			systemFlags.serialSend = 1;
			systemFlags.updateData = 0;

		}
		if(systemFlags.serialSend) {
			printf("%02d/%02d/%20%02d-%d-%02d:%02d:%02d")
		}
	}
}

USART_RECEIVER_BUFFER_FUNCTION_HANDLER;

ISR()
{
	systemFlags.serialSend = 1;
}