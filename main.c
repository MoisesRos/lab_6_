/*
******************************************************************************
Universidad del Valle de Guatemala
Programación de Microcrontroladores
Proyecto: Lab 4
Archivo: main.asm
Hardware: ATMEGA328p
Author : Moises Rosales
**********
*/
#define F_CPU 16000000
//#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"
#include "ADC/ADC.h"
#include "Timer0/Timer0.h"
volatile uint8_t valorADC=0;
volatile uint8_t contador = 0;
volatile uint8_t PuertoADC=4;
volatile uint16_t top=0;


void init_pines(void){
		// Configurar los pines PC4, PC5 y A6 como entradas y habilitar resistencias de pull-up
		DDRC &= ~(1 << DDC4) & ~(1 << DDC5) & ~(1 << DDC6);
		PORTC |= (1 << PORTC4) | (1 << PORTC5)| (1 << PORTC6);
		
		DDRD |= (1 << DDD6);
	
}

uint16_t reescalar(uint8_t valor, uint8_t max_origen, uint16_t max_destino) {
	// Escalar el valor al rango 0 - max_destino
	uint16_t valor_reescalado = (float)valor / max_origen * max_destino;
	//INICIAR TIMER0 (DUTY CYCLE)
	init_Timer0(1,1024);
	return valor_reescalado;
}

int main() {
	CLKPR = CLKPCE;
	CLKPR = CLKPS0;
	contador = 0;
	init_pines();
	//frecuencia del adc -> 16M/128 = 125kHz
	init_ADC(0,0,128);
	
	top = 155;
	int preescaler1=1024;
	
	int preescaler2 = 1024;
	
	init_PWM1A(0,6,preescaler1, top);
	init_PWM2(0, 3, preescaler2);
	//init_Timer0(1,1024);
	OCR0A = 250;
	
	uint8_t valor = 0;
	//sei();
	


	while (1) {
		if (PuertoADC==4)
		{
			valor = readADC(4);
			uint16_t valor_reescalado = reescalar(valor, 255, 40);
			duty_cycle1A(valor_reescalado);
			PuertoADC++;
		} else if (PuertoADC==5)
		{
			valor = readADC(5);
			uint8_t valor_reescalado2 = reescalar(valor, 255, 40);
			duty_cycle2(valor_reescalado2);
			PuertoADC=4;
			} /*else{
			/*valor = readADC(6);
			uint8_t valor_reescalado3 = reescalar(valor, 255, 16);
			//INICIAR TIMER0 (DUTY CYCLE)
			if (contador > 2) {
				PORTD |= (1 << DDD6); // Alto
				//contador = 0;
				/*} else {
				PORTD &= ~(1 << DDD6); // Bajo
			}
			PuertoADC=4;
		}*/
	}

	return 0;
}


