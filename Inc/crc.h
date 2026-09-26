#ifndef CRC_H
#define CRC_H

#include <stdint.h>

void CRC_init(void);
void CRC_reset(void);
uint32_t CRC_calculate(const uint8_t *data, uint32_t length);

#endif
