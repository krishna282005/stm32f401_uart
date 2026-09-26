#include <stdint.h>
#include "usart.h"
#include "usart_regs.h"
#include "usart_config.h"

uint32_t calculate_baud(uint32_t baudrate, uint32_t bus_clk)
{
    uint32_t manti = (bus_clk/16)/baudrate;
    uint32_t frac = ((bus_clk/16 - manti*baudrate)*16)/baudrate;

    return (manti << 4)|frac;
}

void USART_init(void)
{
    USART1->BRR = calculate_baud(BAUDRATE, APB2BUS_CLK);

    USART1->CR1 = (1U << 13)
                | (1U << 3)
                | (1U << 2)
                | (1U << 5);
}
