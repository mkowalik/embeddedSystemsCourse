/*
 * TaskManager.c
 *
 * Created: 2014-11-11 18:00:13
 *  Author: Kowalik
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TaskManager.h"

typedef struct {
	void (*foo)(void*); //function intended to be executed
	void* args; //arguments of function foo
	uint8_t interval;//interval of continuous executions of function
	uint16_t counter;//counter of executions
} task;

static task tasks[MAX_NUMBER_OF_TASKS];

void schedule(){
	
	for (int i=0; i<MAX_NUMBER_OF_TASKS; i++){
		tasks[i].counter++;
	}	
	
}

void addTask(uint8_t numberOfTask, uint8_t interval, void (*foo)(void*), void* args){
	
	tasks[numberOfTask].foo = foo;
	tasks[numberOfTask].args = args;
	tasks[numberOfTask].interval = interval;
	tasks[numberOfTask].counter = 0;
	
}

void setupTimer(){
	
	TCCR0 |= (1<<WGM01) | (0<<WGM00); //tryb pracy na CTC
	OCR0 = 250; //ustawiamy Output Compare Register - granica do ktorej liczmy
	sei(); //w³aczenie przerwan
	TIMSK |= (1<<OCIE0); //wlaczamy przerwanie przy porownaniu
	TCCR0 |= (1<<CS00) | (1<<CS01 ); //prescaller 64
	
}

void execute(){
	
	setupTimer();
	
	while (1){
		for (int i=0; i<MAX_NUMBER_OF_TASKS; i++){
			if (tasks[i].interval>0 && tasks[i].counter>=tasks[i].interval){
				tasks[i].counter-=tasks[i].interval;
				tasks[i].foo(tasks[i].args);
				i=0;
			}
		}
	}
	
}