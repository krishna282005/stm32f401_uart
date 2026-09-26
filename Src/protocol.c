#include <stdint.h>
#include "crc.h"
#include "protocol.h"
#include "usart.h"

uint8_t protocol_send(const uint8_t *data, uint8_t length)
{
    if (length > MAX_PAYLOAD_SIZE)
    {
        return 0;
    }

    uint8_t frame[MAX_PAYLOAD_SIZE + 2 + FRAME_SIZE_CRC];

    frame[0] = FRAME_HEADER;
    frame[1] = length;

    for (uint8_t i = 0; i < length; i++)
    {
        frame[i + 2] = data[i];
    }

    uint32_t crc_out = CRC_calculate(frame, length + 2);

    frame[length + 2] = (uint8_t)(crc_out >> 24);
    frame[length + 3] = (uint8_t)(crc_out >> 16);
    frame[length + 4] = (uint8_t)(crc_out >> 8);
    frame[length + 5] = (uint8_t)(crc_out);

    for (uint16_t i = 0; i < length + 6; i++)
    {
        if (!USART_write_char(frame[i]))
        {
            return 0;
        }
    }

    return 1;
}
