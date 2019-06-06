/**
 * main.c
 */

#include "msp.h"
#include "uart.h"
#include "robot.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	init_uart();
	init_servos();
	while(1){
	    //loop
	}
}
