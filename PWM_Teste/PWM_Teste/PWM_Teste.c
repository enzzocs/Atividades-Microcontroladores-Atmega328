/*
 * PWM_Teste.c
 *
 * Created: 18/09/2017 10:50:19
 *  Author: Enzzo Comassetto dos Santos
 */
//servo pwm motor (+90° a -90°)--- pwm--fast--t=20ms
//+90 = thigh = 2ms
//-90 = thigh = 1ms
//0 =	thigh = 1,5ms
# define F_CPU 1600000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	//Timer
	TCCR1A = 0b10000011;
	TCCR1B = 0b00011101;
	OCR1A = 312;
	DDRB |= 1 << PB1;
	while(1) {
		OCR0A = (OCR0A < 240) ? (OCR0A + 16) : 0;
		_delay_ms(1000);
	}
}