#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define FRAME_HEADER 0xAA
#define FRAME_SIZE_CRC 4
#define MAX_PAYLOAD_SIZE 57

uint8_t protocol_send(const uint8_t *data, uint8_t length);

#endif
