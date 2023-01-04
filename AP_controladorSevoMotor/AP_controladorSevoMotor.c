/*
 * AP_controladorSevoMotor.c
 *
 * Created: 19/10/2017 13:57:42
 *  Author: Enzzo Comassetto dos Santos
 */

#include <avr/io.h>
#include "ATmega328.h"
#include "lcd4f.h"

volatile adcChannel_t adcChannel;
uint8 igual = 0;
uint8 sinal = 0;
uint16 leituraADC;
uint16 leituraServo;
uint8 contador = 0;
int main(void)
{
	//Variable declaration
	attachLcd(display);
	int16 angulo = 0, angulo_servo = 0;
	int16 multiplicador = 1;
	uint8 numero = 0;
	uint8 flag_botao = 0;

	//LCD configuration
	lcdSetControlPort(&display, &DDRC, &PORTC, PC5, PC1, PC2);
	lcdSetDataPort(&display, &DDRB, &PORTB, &PINB, PB2);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);

	//TIMER1 configuration
	timer1Config(TIMER_B_MODE_CTC_ICR, TIMER_A_PRESCALER_64);
	timer1SetInputCaptureValue(2500);
	timer1SetCompareBValue(2500);
	timer1ClearCompareBInterruptRequest();

	//Timer0 configuration
	timer0SetCompareAValue(249);
	timer0ClearCompareAInterruptRequest();
	timer0ActivateCompareAInterrupt();

	//ADC configuration
	adcConfig(ADC_MODE_AUTO_TIMER1_COMPB, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_128);
	adcSelectChannel(ADC_CHANNEL_3);
	adcChannel = ADC_CHANNEL_3;
	adcEnable();
	adcActivateInterrupt();

	//SERVO
	setBit(DDRB, PB0);
	setBit(DDRB, PB1);
	clrBit(PORTB, PB0);
	clrBit(PORTB, PB1);

	printf("Controlador \nServo Motor\n"); //Mensagem Inicial
	_delay_ms(2000);
	lcdClearScreen(&display);

	sei();
	while(1) {
		igual = 0;
		multiplicador = 1;
		numero = 0;
		angulo = 0;
		while(igual != 1) { //Enquanto o igual não for pressionado pega os numeros digitados no teclado
			switch(leituraADC) {
			case 40 ... 112:
				igual = 1;				//acionamento da flag do igual
				break;
			case 113 ... 175:
				numero = 0;
				flag_botao = 1;
				if(angulo == 0) {
					flag_botao = 0;
				}
				break;

			case 227 ... 271:
				sinal = 1;					//acionamento da flag do sinal
				break;

			case 272 ... 314:
				numero = 3;
				flag_botao = 1;
				break;

			case 315 ... 352:
				numero = 2;
				flag_botao = 1;
				break;

			case 353 ... 385:
				numero = 1;
				flag_botao = 1;
				break;

			case 415 ... 442:
				numero = 6;
				flag_botao = 1;
				break;

			case 443 ... 468:
				numero = 5;
				flag_botao = 1;
				break;

			case 469 ... 491:
				numero = 4;
				flag_botao = 1;
				break;
			case 512 ... 532:
				numero = 9;
				flag_botao = 1;
				break;

			case 533 ... 551:
				numero = 8;
				flag_botao = 1;
				break;
			case 552 ... 791:
				numero = 7;
				flag_botao = 1;
				break;
			default:
				flag_botao = 0;
				sinal  = 0;
				break;
			}

			timer0Config(TIMER_A_MODE_CTC, TIMER_A_PRESCALER_1024); //Anti Repique
			timer0ActivateCompareAInterrupt();
			if(sinal == 1 && contador > 20) { //Nega o valor
				timer0Config(TIMER_A_CLOCK_DISABLE, TIMER_A_PRESCALER_OFF);
				timer0DeactivateCompareAInterrupt();
				contador = 0;
				angulo = angulo * (-1);
				sinal = 0;
			}
			if(flag_botao == 1 && contador > 20) { //Adciona o numero selecionado
				timer0Config(TIMER_A_CLOCK_DISABLE, TIMER_A_PRESCALER_OFF);
				timer0DeactivateCompareAInterrupt();
				contador = 0;
				flag_botao = 0;
				if(angulo >= 0) {
					angulo = angulo * multiplicador + numero;
				}
				if(angulo < 0) {
					angulo = angulo * multiplicador - numero;
				}
				if(angulo < 10 && angulo > -10) {
					multiplicador = multiplicador * 10;
				}

				if(angulo > 135 || angulo < -135) {//Verifica se o numero é valido
					lcdClearScreen(&display);
					printf("Angulo Invalido");
					_delay_ms(1000);
					lcdClearScreen(&display);
					igual = 0;
					sinal = 0;
					angulo = 0;
					numero = 0;
					multiplicador = 1;
				}
			}
			printf("Angulo : %d \n servo : %d\n", angulo, angulo_servo);
		}
		while(angulo_servo != angulo) {
			//converte o angulo do servo para graus
			angulo_servo = leituraServo * (-9);
			angulo_servo = angulo_servo / 34;
			angulo_servo = angulo_servo + 135;
			//Gira o motor na direção correta ao comparar o angulo digitado com o angulo do servo
			if(angulo_servo > angulo) {
				setBit(PORTB, PB1);
				clrBit(PORTB, PB0);
			}

			if(angulo_servo < angulo) {
				setBit(PORTB, PB0);
				clrBit(PORTB, PB1);
			}
			printf("Angulo : %d \n servo : %d\n", angulo, angulo_servo);
		}
		setBit(DDRB, PB0);
		setBit(DDRB, PB1);
		clrBit(PORTB, PB0);
		clrBit(PORTB, PB1);
		//TODO:: Please write your application code
	}
}

ISR(ADC_vect)
{
	switch(adcChannel) {
	case ADC_CHANNEL_3:
		leituraServo = ADC;
		timer1ClearCompareBInterruptRequest();
		adcSelectChannel(ADC_CHANNEL_4);
		adcChannel = ADC_CHANNEL_4;
		_delay_ms(1);
		adcStartConversion();
		break;
	case ADC_CHANNEL_4:
		leituraADC = ADC;
		adcSelectChannel(ADC_CHANNEL_3);
		adcChannel = ADC_CHANNEL_3;
		break;
	default:
		break;

	}
}

ISR(TIMER0_COMPA_vect)
{
	contador++;
}