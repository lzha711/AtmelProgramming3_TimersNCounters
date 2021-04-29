/*
 * MyAtmelTimersNcounters.c
 
 * Timers and counters practice
 * CTC mode on using compare output pin OC0A (8bit) and OC1A (16bit)
   or CTC mode using interrupt settings to toggle other pins
 * Created: 4/28/2021 10:38:16 AM
 * Author : lzha711
 */ 
#ifndef F_CPU 
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define CLEAR_BIT(byte, bit) (byte &= ~(1 << bit))

void Setup_8bitTN(void){ // 8-bit counter TCNT0 set up
	// maximum period an 8 bit counter can achieve is 1/1.907
	TCCR0A = 0b01000010; //COM0A1:0 = 01, toggle OC0A on compare match, WGM01:0 = 10, CTC mode, output OC0A toggles when TCNT0 matches
	                     //OCRA
	TCCR0B = 0b00000101; // WGM02 = 0, CTC mode. CS02:00=100, clk_t0 = clk_io/1024	
	OCR0A = 0xFF; //set top value of timer as 255
	TIMSK0 = 0b00000010; // set bit OCIE0A, timer/counter0 output compare match A interrupt enable
	SET_BIT(DDRD, PD6); //set PD6/OC0A as output, square wave will be observed at PD6
}

void Setup_16bitTN(void){ // 16-bit counter TCNT1 set up
	// maximum period an 16 bit counter can achieve is 134.2s
	TCCR1A = 0b01000000; //CTC mode, toggle OC1A on compare match
	TCCR1B = 0b00001101; //CTC mode, clk_t0 = clk_io/1024
	OCR1AH = 0b00000011;  
	OCR1AL = 0b11010000; // calculate to 2s period. 
	TIMSK1 = 0b00000010; // set bit OCIE1A, timer/counter1 output compare match A interrupt enable
	SET_BIT(DDRB, PB1); //set PB1/OC1A as output, square wave will be observed at PB1
}

int main(void)
{
    Setup_8bitTN();
	Setup_16bitTN();
	SET_BIT(DDRC, PC0);
	SET_BIT(DDRC, PC1);
	sei(); //enable interrupt
    
	while (1) 
    {		
		//do nothing
    }
}

ISR(TIMER0_COMPA_vect) { // if timer 0 output compare match A is triggered
	PORTC ^= (1 << PC0); //toggle
}

ISR(TIMER1_COMPA_vect) { // if timer 1 output compare match A is triggered
	PORTC ^= (1 << PC1); //toggle
}
