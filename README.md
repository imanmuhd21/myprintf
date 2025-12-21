Below is a complete, unified GitHub project description, rewritten cleanly and professionally, fully based on all the details you provided, including the first-character kick-start TX design, ring buffer behavior, layering, and SysTick usage.
This is suitable for a README, technical review, or portfolio project.

Custom Lightweight printf() Implementation (Bare-Metal STM32F401RE)

This project is a custom lightweight printf()-like implementation developed entirely from scratch for the STM32F401RE (ARM Cortex-M4) microcontroller. It is written in bare-metal C, does not rely on HAL, LL, or any third-party drivers, and uses only CMSIS register-level access.

The implementation supports the format specifiers %d, %s, and %c, and transmits data over USART using an interrupt-driven design combined with a 16-byte ring buffer.

Project Architecture

The firmware is organized into clear layers to separate responsibilities and improve maintainability.

1. Driver Layer (Hardware Control)
GPIO Driver (gpio.c / gpio.h)

Configures GPIO pins and alternate functions for USART operation

Fully register-level implementation

No vendor abstraction layers

USART Driver (usart.c / usart.h)

This is the core of the project and handles all low-level serial communication, including:

USART peripheral configuration

TX interrupt (TXEIE) handling

Writing bytes into the TDR register

Integer-to-ASCII conversion

String and character transmission

Character reception

Interaction with the ring buffer

Interrupt-safe transmission control

Interrupt-Driven TX Design (Key Feature)

The USART TX implementation uses a first-character kick-start mechanism:

Only the first character is written by the CPU into both:

the ring buffer

and TDR

Writing to TDR clears TXE, starting the hardware transmission

TXEIE is enabled, allowing the USART to generate interrupts

All subsequent characters are fetched only by the ISR from the ring buffer

After the first character, the CPU’s only responsibility is buffering data

This design ensures:

No polling

Minimal CPU involvement

Fully asynchronous transmission

TX Flow Summary

CPU disables TXE interrupt

CPU receives a character

Character is written into the ring buffer

If the buffer was empty before insertion, the same character is also written into TDR

TXE interrupt is re-enabled

When TXE becomes set, the ISR:

fetches the next byte from the ring buffer

writes it into TDR

continues until the buffer is empty

2. Buffer Layer
Ring Buffer (ringbuffer.c / ringbuffer.h)

Implements a 16-byte circular buffer

Manages:

head and tail indexing

full and empty conditions

controlled read/write access

APIs such as:

put_ring_buff()

get_ring_buff()

Acts as a safe communication bridge between:

main application code

USART interrupt service routine

This layer fully controls how data is queued and drained during transmission.

3. Software / Application Layer
Custom printf() (myprintf.c / myprintf.h)

Implements a lightweight printf() using variadic arguments

Parses the format string character-by-character

On encountering %, checks for:

%d → integer conversion and transmission

%s → string transmission

%c → single character transmission

Non-format characters are transmitted directly as bytes

Fully reuses USART driver functionality

Initialization Abstraction

myprintf_init() performs all hardware setup, including:

GPIO configuration

USART configuration

Alternate function mapping

Interrupt enabling

This allows main.c to remain minimal and clean.

4. Timing Utility
SysTick (systick.c / systick.h)

Uses the Cortex-M SysTick timer

Provides second-based blocking delays

Example:

systick_delayS(2) → 2-second delay

systick_delayS(3) → 3-second delay

Example Usage
int main(void)
{
    myprintf_init();

    while (1)
    {
        myprintf("testing%d", 21);
        systick_delayS(2);
    }
}


All peripheral configuration is handled internally by myprintf_init().
After initialization, the superloop only focuses on application logic.

Key Features

Bare-metal STM32 firmware (no HAL / LL)

Custom GPIO and USART drivers

Interrupt-driven USART TX using TXE

First-character kick-start transmission design

Ring buffer–based asynchronous communication

Lightweight printf() with variadic arguments

Clear layered architecture

Designed for learning and demonstrating low-level firmware development

This project demonstrates direct register manipulation, interrupt handling, buffer management, and formatted output implementation on ARM Cortex-M, closely resembling real-world embedded driver design while remaining lightweight and educational.

If you want, I can:

Shorten this into a compact README

Rewrite it for a resume or interview explanation

Add a TX flow diagram description

Polish it for open-source presentation

Just tell me.
