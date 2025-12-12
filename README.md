# myprintf
Custom bare-metal printf() for STM32 using CMSIS only. Includes a register-level UART driver with a 16-byte ring buffer and interrupt-driven TX. Supports %d, %s, %c with manual parsing and integer-to-ASCII conversion.
