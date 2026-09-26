#include <stdint.h>

#include "usart.h"
#include "usart_regs.h"
#include "usart_config.h"

volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_overflow = 0;
volatile uint8_t rx_error = 0;

uint8_t USART_buffer_read(uint8_t *data)
{
    if (rx_head == rx_tail)
    {
        return 0;
    }

    *data = rx_buffer[rx_tail];

    rx_tail++;

    if (rx_tail >= RX_BUFFER_SIZE)
    {
        rx_tail = 0;
    }

    return 1;
}

uint8_t USART_read_char(uint8_t *data)
{
    return USART_buffer_read(data);
}

uint8_t USART_read_string(char *buffer, uint32_t size)
{
    static uint32_t index = 0;

    if (size == 0)
    {
        return 0;
    }

    uint8_t data;

    while (USART_read_char(&data))
    {
        if (data == '\r')
        {
            continue;
        }

        if (data == '\n')
        {
            buffer[index] = '\0';
            index = 0;
            return 1;
        }

        if (index < size - 1)
        {
            buffer[index] = data;
            index++;
        }
    }

    return 0;
}

uint8_t USART_overflow(void)
{
    if (rx_overflow)
    {
        rx_overflow = 0;
        return 1;
    }

    return 0;
}
