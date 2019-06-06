/*
 * uart.h
 *
 */

#ifndef UART_H_
#define UART_H_

#define UCBRF_NUM 10
#define UCBRS_NUM 0x00
#define UCBRW_NUM 0x01

#define UCBRF_PERIPHERAL_NUM 8
#define UCBRS_PERIPHERAL_NUM 0x55
#define UCBRW_PERIPHERAL_NUM 0x13

#define MAX_OUT 4096

void init_uart();
void write_uart(uint8_t data);
void uart_str(char *str);
void uart_newline();

#endif /* UART_H_ */
