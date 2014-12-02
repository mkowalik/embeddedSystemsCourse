/*
 * main.c - example use of task_manager library
 *
 * Created: 2014-11-11 17:27:14
 *  Author: Patrycja Cielecka, Micha³ Kowalik, Igor Sieradzki
 */ 

#include <avr/io.h>
#include "taskManager.h"

/**
* scheduling functions, call at every interrupt
*/ 
ISR(TIMER0_COMP_vect){
	schedule();						
}

volatile int task1int = 0;
volatile int task2int = 0;

/**
* Dummy task functions #1
*/
void task1(void* args){				
	task1int++;
}

/**
* Dummy task functions #2
*/
void task2(void* args){				
	task2int+=2;
}

/**
* Setup atmega32's clock for interrupts every 1ms
*/
void setupTimer(){
	
	TCCR0 |= (1<<WGM01) | (0<<WGM00);	// set clock type as CTC
	OCR0 = 250;							// set Output Compare Register - together with prescaler this will give us 
											// interrupt every 1ms
	sei();								// turn interrupts on
	TIMSK |= (1<<OCIE0);				// set interrupts co compare
	TCCR0 |= (1<<CS00) | (1<<CS01 );	// set clock prescaler at 64*250 = 16,000; 16MHz * 16,000 = 1KHZ;
	
}



int main(void)
{
	setupTimer();							
	addTask(0, 5, task1, (void*)0);
	addTask(1, 10, task2, (void*)0);
	execute();
}