#include <stdint.h>
#include "crc.h"
#include "crc_regs.h"
#include "stm32f4xx_regs.h"

void CRC_init(void)
{
    RCC->AHB1ENR |= (1U << 12); // enable clk for crc
}

void CRC_reset(void)
{
    CRC->CR = (1U << 0); //reset high
}

uint32_t CRC_calculate(const uint8_t *data, uint32_t length)
{
    CRC_reset();

    uint32_t count = length/4;
    uint32_t remainder = length % 4;
    uint32_t i = 0;
    uint32_t word = 0;

    for (i = 0; i < count; i++)
    {
        word = ((uint32_t)data[0 + i*4] << 24)
             | ((uint32_t)data[1 + i*4] << 16)
             | ((uint32_t)data[2 + i*4] << 8)
             | ((uint32_t)data[3 + i*4] << 0);

        CRC->DR = word;
    }

    if (remainder != 0)
    {
        word = 0;

        if (remainder >= 1)
            word |= ((uint32_t)data[count*4 + 0] << 24);

        if (remainder >= 2)
            word |= ((uint32_t)data[count*4 + 1] << 16);

        if (remainder >= 3)
            word |= ((uint32_t)data[count*4 + 2] << 8);

        CRC->DR = word;
    }

    return CRC->DR;
}
