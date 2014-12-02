/*
* clockDisplay.c
*
* Created: 2014-12-02 14:09:29
*  Author: Adriana Borowa, Micha³ Kowalik
*/

#include <avr/io.h>
#include "clockDisplay.h"

static uint8_t CYFRA[10] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};
static uint8_t DOT = 0b01111111;

volatile static uint32_t actual_time = 0;
volatile static uint8_t actual_display = 0;

void (*funToCallWhen1000)(void*) = 0;
void* argsToCallWhen1000 = 0;

void setupClockDisplay(){
	DISPLAY_DDR = 0xFF;
	DISPLAY_PORT = 0x0;
	DISPLAY_GRD_DDR = DISPLAY_GRD_DDR | 0x0F;
	DISPLAY_GRD_PORT = DISPLAY_GRD_PORT & 0xFF;
}

void increment_value(void* args){
	actual_time++;
}

void setupFunctionToCallWhen1000(void (*foo)(void*), void* args){
	funToCallWhen1000 = foo;
	argsToCallWhen1000 = args;
}

void changeDisplayShowSeconds(void* args){
	DISPLAY_GRD_PORT |= 0x0f;
	actual_display = (actual_display + 1) % 4;
	if (actual_display==0) DISPLAY_PORT = CYFRA[(actual_time/10)%10];
	else if (actual_display==1) DISPLAY_PORT = CYFRA[(actual_time/100)%10];
	else if (actual_display==2) DISPLAY_PORT = CYFRA[(actual_time/1000)%10];
	else if (actual_display==3) DISPLAY_PORT = CYFRA[(actual_time/10000)%10];
	DISPLAY_GRD_PORT &= (0xF0 | (~(1<<actual_display)));
}

void changeDisplayShowDeciSeconds(void* args){
	DISPLAY_GRD_PORT |= 0x0f;
	actual_display = (actual_display + 1) % 4;
	if (actual_display==0) DISPLAY_PORT = CYFRA[actual_time%10];
	else if (actual_display==1) { DISPLAY_PORT = CYFRA[(actual_time/10)%10] & DOT ; }
	else if (actual_display==2) DISPLAY_PORT = CYFRA[(actual_time/100)%10];
	else if (actual_display==3) DISPLAY_PORT = CYFRA[(actual_time/1000)%10];
	DISPLAY_GRD_PORT &= (0xF0 | (~(1<<actual_display)));
	
	if (actual_time >= 1000 && funToCallWhen1000!=0) funToCallWhen1000(argsToCallWhen1000);
	//changeThisTask(change_display_above_1000_sek, (void*)0);
}