#ifndef USART_CONFIG_H
#define USART_CONFIG_H

#define APB2BUS_CLK 16000000U
#define BAUDRATE 115200U

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64
#define TX_MSG_QUEUE_SIZE 8

#define RXNE (1U << 5)
#define RXNEIE (1U << 5)
#define TXE (1U << 7)
#define TXEIE (1U << 7)

#define PC13 (1U << 5)
#define ORE (1U << 3)

#endif
