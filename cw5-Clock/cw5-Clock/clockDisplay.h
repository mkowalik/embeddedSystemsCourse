/**
 * clockDisplay.h
 *
 * Created: 2014-12-02 14:10:41
 * Author: Adriana Borowa, Micha³ Kowalik
 */ 


#ifndef CLOCKDISPLAY_H_
#define CLOCKDISPLAY_H_

#include <avr/io.h>

#define DISPLAY_PORT PORTA
#define DISPLAY_DDR DDRA
#define DISPLAY_GRD_PORT PORTB
#define DISPLAY_GRD_DDR DDRB

/**
* Function needed to be executed before using clock library. Setup procedures.
*/
void setupClockDisplay();
/**
* Function which will be called ones when counter will achieve value of 1000sek.
*/
void setupFunctionToCallWhen1000(void (*foo)(void*), void* args);
/**
* Function which will display value of actual time which one place after dot - in deciseconds.
*/
void changeDisplayShowDeciSeconds(void* args);
/**
* Function which will display value of actual time in seconds - without dot.
*/
void changeDisplayShowSeconds(void* args);
/**
* Function which should be called every 0.1 second. It increments value of variable representing actual time.
*/
void increment_value(void* args);

#endif /* CLOCKDISPLAY_H_ */