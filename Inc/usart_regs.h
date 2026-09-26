#ifndef USART_REGS_H
#define USART_REGS_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
}USART1_typedef;

#define USART1 ((USART1_typedef *)0x40011000U)

#endif
