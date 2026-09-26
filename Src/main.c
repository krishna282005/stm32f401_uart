#include <stdint.h>

#include "stm32f4xx_regs.h"
#include "usart.h"
#include "protocol.h"
#include "crc.h"

int main(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= (1U << 0);

    /* Configure PA9 and PA10 for USART1 alternate function mode */
    GPIOA->MODER &= ~(3U << (9 * 2));
    GPIOA->MODER |=  (2U << (9 * 2));

    GPIOA->MODER &= ~(3U << (10 * 2));
    GPIOA->MODER |=  (2U << (10 * 2));

    /* Select AF7 for PA9 (TX) and PA10 (RX) */
    GPIOA->AFRH &= ~(15U << 4);
    GPIOA->AFRH |=  (7U << 4);

    GPIOA->AFRH &= ~(15U << 8);
    GPIOA->AFRH |=  (7U << 8);

    /* Enable USART1 interrupt (IRQ 37 -> NVIC ISER1 bit 5) */
    NVIC_ISER1 |= (1U << 5);

    USART_init();
    CRC_init();

    /* Protocol frame demonstration */
    uint8_t data[] = {'H', 'E', 'L', 'L', 'O'};
    protocol_send(data, sizeof(data));

    while (1)
    {
    }
}
