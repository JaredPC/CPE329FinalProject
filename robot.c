/*
 * robot.c
 *
 */

#include "msp.h"
#include "robot.h"

void init_servos(){
    //Initialize Servo Pins
    P2->SEL0 |= (BIT7 | BIT6);
    P2->SEL1 &= ~(BIT7 | BIT6);
    P2->DIR  |= (BIT7 | BIT6);

    // Config TimerA0 for reset/set output mode and at 20ms period
    TIMER_A0->CCR[0] = 60000;   //Set for 20ms duty cycle
    TIMER_A0->CCR[3] = CW;    // Set L servo to run CW
    TIMER_A0->CCR[4] = CW;    // Set R servo to run CW
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__STOP;
    TIMER_A0->R = 0;
}

void turn_right(){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[3] = CW;    // Set L servo to run CW
    TIMER_A0->CCR[4] = CW;    // Set R servo to run CW
}

void turn_left(){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[3] = CCW;    // Set L servo to run CCW
    TIMER_A0->CCR[4] = CCW;    // Set R servo to run CCW
}

void forward(){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[3] = CW;    // Set L servo to run CW
    TIMER_A0->CCR[4] = CCW;    // Set R servo to run CCW
}

void backward(){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[3] = CCW;    // Set L servo to run CCW
    TIMER_A0->CCR[4] = CW;    // Set R servo to run CW
}

void stop(){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__STOP;
    TIMER_A0->CCR[3] = STOP;    // Set L servo to STOP
    TIMER_A0->CCR[4] = STOP;    // Set R servo to STOP
}
