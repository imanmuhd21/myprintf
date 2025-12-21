# Custom Lightweight myprintf() (Bare-Metal STM32)

A custom lightweight `printf()`-like implementation built from scratch on the STM32F401RE using bare-metal programming (CMSIS only).
This project focuses on low-level UART driver design, interrupt-driven transmission, and ring buffer management, without relying on HAL, LL, or any third-party libraries.

## Features

* Custom lightweight `myprintf()`
* Supported format specifiers:

  * `%d` – integer
  * `%s` – string
  * `%c` – character
* Fully bare-metal (no HAL / LL)
* Custom GPIO and USART drivers
* Interrupt-driven UART transmission
* 16-byte ring buffer
* Variadic argument handling
* One-call initialization

## Project Structure

```
Driver Layer
├── gpio.c / gpio.h
├── usart.c / usart.h
├── systick.c / systick.h

Service Layer
├── ringbuffer.c / ringbuffer.h

Application Layer
├── myprintf.c / myprintf.h
└── main.c
```

## USART Transmission Design

* The CPU sends only the first character directly to the `TDR`
* Remaining bytes are transmitted via TXE interrupt
* A 16-byte ring buffer stores outgoing data
* When `TDR` transfers data to the shift register, `TXE = 1`
* With `TXEIE` enabled, the ISR:

  * Fetches the next byte from the ring buffer
  * Writes it to `TDR`
* CPU work after the first byte is only ring buffer writes

This design minimizes CPU usage and avoids blocking delays.

## Ring Buffer

* Fixed size: 16 bytes
* Uses `head` and `tail` indices
* Detects empty and full conditions
* Safely handles data between CPU and USART ISR

## myprintf() Implementation

* Implemented using variadic arguments (`stdarg.h`)
* Parses the format string character-by-character
* On `%` detection:

  * `%d` → integer to ASCII (USART driver)
  * `%s` → string transmit (USART driver)
  * `%c` → character transmit
* Non-format characters are transmitted directly

Formatting logic is handled in `myprintf.c`, while all transmission is delegated to `usart.c`.

## Initialization

### myprintf_init()

Initializes everything in one place:

* GPIO alternate function configuration
* USART configuration
* TX interrupt (`TXEIE`) enable
* NVIC setup
* Ring buffer initialization

After calling this function, the system is ready.

## Example Usage

```c
int main(void)
{
    myprintf_init();

    while (1)
    {
        myprintf("testing %d\r\n", 21);
        systick_delayS(2);
    }
}
```

## SysTick Delay

* Uses SysTick timer
* Delay is second-based

```c
systick_delayS(3); // 3 seconds delay
```

## Target Hardware

* MCU: STM32F401RE
* Core: ARM Cortex-M4
* Framework: CMSIS only
* Language: C (bare-metal)

## Purpose

This project demonstrates real embedded firmware design concepts such as:

* UART driver development
* Interrupt-driven communication
* Ring buffer synchronization
* Lightweight replacement for standard `printf()`
