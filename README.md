# STM32F401 Bare-Metal USART Driver

Register-level USART1 driver and binary protocol framing implementation for STM32F401CCUx, written in Embedded C without HAL.

## Current scope

- Register-level USART1 initialization
- Interrupt-driven TX using a circular byte buffer
- Interrupt-driven RX using a circular byte buffer
- TX message queue for strings
- Decimal, integer and hexadecimal output helpers
- `USART_printf()` support for common format specifiers
- RX overrun (`ORE`) detection
- STM32F4 hardware CRC peripheral
- Binary frame construction with CRC-32

## Target configuration

- MCU: STM32F401CCUx
- APB2 clock: 16 MHz
- USART1 baud rate: 115200
- USART1: PA9 (TX), PA10 (RX), AF7
- USART1 interrupt: IRQ 37

## Binary protocol

```text
+--------+--------+---------+----------+
| Header | Length | Payload | CRC-32   |
| 0xAA   | 0..57  | 0..57 B | 4 bytes  |
+--------+--------+---------+----------+
```

CRC bytes are transmitted MSB first.

The current payload limit is intentionally 57 bytes so the complete frame is at most 63 bytes, matching the usable capacity of the current 64-byte circular TX buffer.

Frames larger than this limit are intentionally deferred to a later revision.

## Hardware validation

A complete `HELLO` frame was verified over USART in hexadecimal form:

```text
AA 05 48 45 4C 4C 4F EB C7 70 FB
```

The STM32F4 hardware CRC peripheral was also verified independently during development.

## Project structure

```text
Inc/
    crc.h
    crc_regs.h
    protocol.h
    stm32f4xx_regs.h
    usart.h
    usart_config.h
    usart_regs.h

Src/
    crc.c
    main.c
    protocol.c
    syscalls.c
    sysmem.c
    usart.c
    usart_irq.c
    usart_rx.c
    usart_tx.c

Startup/
    startup_stm32f401ccux.s

STM32F401CCUX_FLASH.ld
.project
.cproject
```

## Build

Open the project in STM32CubeIDE and build for the STM32F401CCUx target.

This project intentionally uses direct register access rather than STM32 HAL.
