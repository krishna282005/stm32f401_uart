#ifndef USART_H
#define USART_H

#include <stdint.h>

void USART_init(void);

uint32_t calculate_baud(uint32_t baudrate, uint32_t bus_clk);

uint8_t USART_read_char(uint8_t *data);
uint8_t USART_read_string(char *buffer, uint32_t size);

uint8_t USART_write_char(uint8_t data);
uint8_t USART_tx_service(void);
uint8_t USART_write_string(char *msg_tx);
uint8_t USART_write_dec(uint32_t data);
uint8_t USART_write_int(int32_t data);
uint8_t USART_write_hex(uint32_t data);
uint8_t USART_printf(const char *format,...);

uint8_t USART_overflow(void);

void USART1_IRQHandler(void);

extern volatile uint8_t tx_head;
extern volatile uint8_t tx_tail;
extern volatile uint8_t tx_buffer[];

extern volatile uint8_t rx_head;
extern volatile uint8_t rx_tail;
extern volatile uint8_t rx_buffer[];
extern volatile uint8_t rx_overflow;
extern volatile uint8_t rx_error;

#endif
