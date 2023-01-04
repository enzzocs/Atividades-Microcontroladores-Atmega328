/*
 * RTC_Serial.c
 *
 * Created: 10/11/2017 07:46:52
 *  Author: Aluno
 */ 


#include "globaldefines.h"
#include "atmega328.h"
#include "ds1307.h"


typedef volatile union systemFlags_t{
	uint8 allFlags;
	struct{
		uint8	intzero	: 1;
	};
} systemFlags_t;


systemFlags_t systemFlags;

int main(void)
{
	//variables
	uint8 segundos = 02;
	uint8 horas =8;
	uint8 minutos = 46;
	uint8 dia_semana = 5;
	uint8 dia = 11;
	uint8 mes = 9;
	uint16 ano = 2001;
	uint8 tipo;
	
	// USART configuration
	usartConfig(USART_MODE_ASYNCHRONOUS, USART_BAUD_9600, USART_DATA_BITS_8, USART_PARITY_NONE, USART_STOP_BIT_SINGLE);
	usartEnableTransmitter();
	usartEnableReceiver();
	usartActivateReceptionCompleteInterrupt();
	usartStdio();
	
	//twi
	twiMasterInit(100000);
	
	
	//int0 config
	int0Config(PORT_INPUT_PULL_UP, SENSE_FALLING_EDGE);
	int0ActivateInterrupt();
	
	systemFlags.allFlags = 0;
	sei();
	
	//DS1307 config
	ds1307SetControl(DS1307_COUNTING_RESUME, DS1307_CLOCK_1HZ, DS1307_FORMAT_24_HOURS);
	
    while(1){
	
		if (systemFlags.intzero){
			systemFlags.intzero=0;
			ds1307GetTime(&horas,&minutos,&segundos,&tipo);
			printf (" %d:%d ",minutos,segundos);
		}
    }
}

ISR (INT0_vect){
	systemFlags.intzero = 1;
}