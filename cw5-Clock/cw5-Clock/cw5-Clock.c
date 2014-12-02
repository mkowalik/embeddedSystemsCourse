/*
* cw5_Timer.c
*
* Created: 2014-11-26 17:21:05
*  Author: Adriana Borowa, Micha³ Kowalik
*/

#include <avr/io.h>
#include "taskManager.h"
#include "clockDisplay.h"

ISR(TIMER0_COMP_vect){
	schedule();
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

typedef void (*toCallFunction)(void (*)(void*), void*); //pointer to function typed as setupFunctionToCallWhen1000 from clockDisplay.h
typedef void (*toSetupFunction)(void*); //pointer to function typed as changeThisTask from taskManager.h

/**
* This function is called from clock function when time reaches 1000sek and it's needed to turn off dot after seconds value. This function calls calls function changeThisTask from taskManager.h with argument change_display_above_1000_sek from clockDisplay.h, which changes function executing every 4ms from changeDisplayShowDeciSeconds to changeDisplayAbove1000sek.
*/
void changeDisplayingFunction(void* args){
	toCallFunction foo = *((toCallFunction*) args);
	toSetupFunction fooSet = *((toSetupFunction*)(args + sizeof(toCallFunction)));
	void* argsSet = (void*)0;
	foo(fooSet, argsSet);
}

int main(void)
{
	setupTimer();
	setupClockDisplay();
	void* argsChangingFunction[] = {changeThisTask, changeDisplayShowSeconds};
	setupFunctionToCallWhen1000(changeDisplayingFunction, (void*)argsChangingFunction);
	addTask(0, 100, increment_value, (void*)0);
	addTask(1, 4, changeDisplayShowDeciSeconds, (void*)0);
	execute();
}