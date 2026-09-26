#ifndef CRC_REGS_H
#define CRC_REGS_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t DR;
    volatile uint32_t IDR;
    volatile uint32_t CR;
}CRC_typedef;

#define CRC ((CRC_typedef *)0x40023000U)

#endif
