/*
 * uart.c
 *
 */
#include "msp.h"
#include "uart.h"
#include "robot.h"

static volatile uint8_t output_flag = 0;

void EUSCIA0_IRQHandler(void){
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        uint8_t rx_character = EUSCI_A0->RXBUF; // character UART is receiving
        write_uart(rx_character); //echo rx_character
        if(rx_character == '\r'){
            uart_newline();
        }
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));    //If send buffer clear
        EUSCI_A2->TXBUF = rx_character;
        if(rx_character == '\r'){
            while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));    //If send buffer clear
            EUSCI_A2->TXBUF = '\n';
        }
    }
}

void EUSCIA2_IRQHandler(void){
    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG){
        uint8_t rx_character = EUSCI_A2->RXBUF;
        write_uart(rx_character);
        //uart_newline();
        switch(rx_character){
            case(1):
                forward();
                break;
            case(2):
                backward();
                break;
            case(3):
                turn_right();
                break;
            case(4):
                turn_left();
                break;
            case(5):
                stop();
                break;
            default:
                break;
        }
    }
}

void init_uart(){
    //Write to screen UART
    //=============================================================================================================
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     // Software reset enable
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_PEN        // Parity enable
                     | EUSCI_A_CTLW0_SPB        // Stop bit select
                     | EUSCI_A_CTLW0_MODE_0     // UART mode
                     | EUSCI_A_CTLW0_UCSSEL_2   // SMCLK
                     | EUSCI_A_CTLW0_SWRST;     // Keep Software reset enable

    // Baud Rate calculation
    EUSCI_A0->BRW = UCBRW_NUM;
    // UCBRS and UCBRF registers set and Oversampling mode enabled
    EUSCI_A0->MCTLW = (UCBRS_NUM << EUSCI_A_MCTLW_BRS_OFS) | (UCBRF_NUM << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    P1->SEL0 |= (BIT2 | BIT3);      // Select EUSCI_A0
    P1->SEL1 &= ~(BIT2 | BIT3);     // Tx and Rx

    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);   // Software reset off
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;              //CLear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;                  // Enable RX interrupt
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
    __enable_irq();

    //Read from Peripheral UART initialization
    //==============================================================================================================
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     // Software reset enable
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_MODE_0     // UART mode
                     | EUSCI_A_CTLW0_UCSSEL_2   // SMCLK
                     | EUSCI_A_CTLW0_SWRST;     // Keep Software reset enable

    EUSCI_A2->BRW = UCBRW_PERIPHERAL_NUM;
    EUSCI_A2->MCTLW = (UCBRS_PERIPHERAL_NUM << EUSCI_A_MCTLW_BRS_OFS) | (UCBRF_PERIPHERAL_NUM << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;
    P3->SEL0 |= (BIT2 | BIT3);      // Select EUSCI_A0
    P3->SEL1 &= ~(BIT2 | BIT3);     // Tx and Rx

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);   // Software reset off
    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;              //CLear eUSCI RX interrupt flag
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                  // Enable RX interrupt
    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);
}

void write_uart(uint8_t data){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));    //If send buffer clear
    EUSCI_A0->TXBUF = data;
}

void uart_newline(){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '\n';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '\r';
}

void uart_str(char *str){
    int i = 0;
    while(str[i] != '\0'){
        write_uart(str[i]);
        i++;
    }
    uart_newline();
}



