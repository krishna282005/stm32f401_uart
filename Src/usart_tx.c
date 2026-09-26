#include <stddef.h>
#include <stdarg.h>

#include "usart.h"
#include "usart_regs.h"
#include "usart_config.h"

volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_buffer[TX_BUFFER_SIZE];

volatile uint8_t msg_head = 0;
volatile uint8_t msg_tail = 0;
char *msg_buffer[TX_MSG_QUEUE_SIZE];

char *current_msg = 0;

uint8_t USART_buffer_write(uint8_t data)
{
    uint8_t next_head = tx_head + 1;

    if (next_head >= TX_BUFFER_SIZE)
    {
        next_head = 0;
    }

    if (next_head == tx_tail)
    {
        return 0;
    }

    tx_buffer[tx_head] = data;
    tx_head = next_head;

    USART1->CR1 |= TXEIE;

    return 1;
}

uint8_t USART_write_char(uint8_t data)
{
    return USART_buffer_write(data);
}

uint8_t USART_write_string(char *msg_tx)
{
    uint8_t next_tail = msg_tail + 1;

    if (next_tail >= TX_MSG_QUEUE_SIZE)
    {
        next_tail = 0;
    }

    if (next_tail == msg_head)
    {
        return 0;
    }

    msg_buffer[msg_tail] = msg_tx;
    msg_tail = next_tail;

    USART1->CR1 |= TXEIE;

    return 1;
}

uint8_t USART_tx_service(void)
{
    if (current_msg == NULL)
    {
        if (msg_head == msg_tail)
        {
            return 0;
        }

        current_msg = msg_buffer[msg_head];
        msg_head += 1;
    }

    if (msg_head >= TX_MSG_QUEUE_SIZE)
    {
        msg_head = 0;
    }

    while (*current_msg != '\0')
    {
        if (!(USART_write_char(*current_msg)))
        {
            return 2;
        }

        current_msg += 1;
    }

    current_msg = NULL;
    return 1;
}

uint8_t USART_write_dec(uint32_t data)
{
    uint32_t temp = data;
    uint8_t length = 0;

    while (temp > 0)
    {
        temp = temp / 10;
        length += 1;
    }

    if (length == 0)
    {
        return USART_write_char('0');
    }

    uint8_t data_converted[length];

    for (uint8_t i = 0; data > 0; i++)
    {
        data_converted[length - i - 1] = (data % 10) + '0';
        data = data / 10;
    }

    uint8_t i = 0;

    while (i < length)
    {
        if (!USART_write_char(data_converted[i]))
        {
            return 0;
        }

        i += 1;
    }

    return 1;
}

uint8_t USART_write_int(int32_t data)
{
    if (data < 0)
    {
        uint32_t temp = (uint32_t)(data);

        if (!(USART_write_char('-')))
        {
            return 0;
        }

        temp = 0U - temp;
        return USART_write_dec(temp);
    }

    return USART_write_dec((uint32_t)data);
}

uint8_t USART_write_hex(uint32_t data)
{
    uint32_t digit;

    for (uint8_t i = 8; i > 0; i--)
    {
        digit = ((data >> 4 * (i - 1)) & 0xF);

        if (digit < 10)
        {
            if (!(USART_write_char(digit + '0')))
            {
                return 0;
            }
        }
        else
        {
            if (!(USART_write_char(digit - 10 + 'A')))
            {
                return 0;
            }
        }
    }

    return 1;
}

uint8_t USART_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;

            if (*format == '\0')
            {
                va_end(args);
                return 0;
            }

            switch (*format)
            {
                case 'd':
                {
                    int value = va_arg(args, int);
                    if (!(USART_write_int(value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case 'c':
                {
                    int value = va_arg(args, int);
                    if (!(USART_write_char((uint8_t)value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case 's':
                {
                    char *value = va_arg(args, char *);
                    if (!(USART_write_string(value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case 'u':
                {
                    uint32_t value = va_arg(args, uint32_t);
                    if (!(USART_write_dec(value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case 'i':
                {
                    int value = va_arg(args, int);
                    if (!(USART_write_int(value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case 'x':
                case 'X':
                {
                    uint32_t value = va_arg(args, uint32_t);
                    if (!(USART_write_hex(value)))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                case '%':
                {
                    if (!(USART_write_char('%')))
                    {
                        va_end(args);
                        return 0;
                    }
                    break;
                }

                default:
                    va_end(args);
                    return 0;
            }

            format++;
            continue;
        }

        if (!(USART_write_char(*format)))
        {
            va_end(args);
            return 0;
        }

        format++;
    }

    va_end(args);
    return 1;
}
