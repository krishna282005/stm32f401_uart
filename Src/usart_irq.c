#include "usart.h"
#include "usart_regs.h"
#include "usart_config.h"

void USART1_IRQHandler(void)
{
    uint32_t status = USART1->SR;

    if (status & ORE)
    {
        rx_error = 1;
    }

    if ((status & RXNE) && (USART1->CR1 & RXNEIE))
    {
        uint8_t next_head = rx_head + 1;
        uint8_t data = USART1->DR;

        if (next_head >= RX_BUFFER_SIZE)
        {
            next_head = 0;
        }

        if (next_head == rx_tail)
        {
            rx_overflow = 1;
        }
        else
        {
            rx_buffer[rx_head] = data;
            rx_head = next_head;
        }
    }
    else if (status & ORE)
    {
        (void)USART1->DR;
    }

    if ((USART1->SR & TXE) && (USART1->CR1 & TXEIE))
    {
        if (tx_head == tx_tail)
        {
            uint8_t service_status = USART_tx_service();

            if (service_status == 1)
            {
            }
            else if (service_status == 2)
            {
            }
            else
            {
                USART1->CR1 &= ~TXEIE;
            }
        }
        else
        {
            uint8_t data = tx_buffer[tx_tail];

            USART1->DR = data;

            tx_tail++;

            if (tx_tail >= TX_BUFFER_SIZE)
            {
                tx_tail = 0;
            }
        }
    }
}
