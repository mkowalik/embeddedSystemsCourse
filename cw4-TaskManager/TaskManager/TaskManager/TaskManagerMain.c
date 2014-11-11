/*
 * TaskManager.c
 *
 * Created: 2014-11-11 17:27:14
 *  Author: Kowalik
 */ 


#include <avr/io.h>
#include "TaskManager.h"

ISR(TIMER0_COMP_vect){
	schedule();
}

volatile int task1int = 0;
volatile int task2int = 0;

void task1(void* args){
	task1int++;
}

void task2(void* args){
	task2int+=2;
}

int main(void)
{
	setupTimer();
	addTask(0, 5, task1, (void*)0);
	addTask(1, 10, task2, (void*)0);
	execute();
}