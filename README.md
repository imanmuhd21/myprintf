Custom Lightweight printf() (Bare-Metal STM32F401RE)

A from-scratch, bare-metal printf()-like implementation for the STM32F401RE (ARM Cortex-M4), featuring interrupt-driven USART transmission, a ring buffer, and zero dependency on HAL / LL or third-party drivers.

This project focuses on low-level firmware design, direct register manipulation, and clean driver layering, closely resembling how production-grade embedded drivers are written.

✨ Highlights

✅ Bare-metal firmware (CMSIS only)

✅ Custom GPIO and USART drivers

✅ Lightweight printf() implementation

✅ Supports %d, %s, %c

✅ Interrupt-driven USART TX (TXE)

✅ First-character kick-start transmission design

✅ 16-byte ring buffer

✅ Layered architecture (Driver → Buffer → Application)

✅ Minimal main.c usage

✅ Designed for learning real embedded driver behavior

🧠 Design Philosophy

Instead of polling or blocking I/O, this project uses an interrupt-driven UART transmission model where:

The CPU writes only the first character to the USART

All subsequent characters are handled by the USART ISR

A ring buffer decouples the application from the hardware

The CPU remains free after enqueueing data

This mirrors how professional embedded serial drivers are implemented.

🏗️ Project Architecture

The firmware is organized into clear layers, each with a single responsibility.

Application Layer
 └── myprintf.c / myprintf.h

Buffer Layer
 └── ringbuffer.c / ringbuffer.h

Driver Layer
 ├── usart.c / usart.h
 └── gpio.c  / gpio.h

Utility
 └── systick.c / systick.h

🔌 Driver Layer
GPIO Driver (gpio.c / gpio.h)

Configures GPIO pins and alternate functions

Register-level implementation

Used for USART pin mapping

USART Driver (usart.c / usart.h) ⭐ Core of the Project

Handles all low-level serial communication, including:

USART peripheral configuration

TXE interrupt enabling/disabling

Writing bytes into the TDR register

Integer-to-ASCII conversion

String and character transmission

Character reception

Safe interaction with the ring buffer

⚡ Interrupt-Driven TX Design (Key Highlight)

This project uses a first-character kick-start mechanism for USART transmission.

How It Works
1️⃣ First Character (Kick-start)

TXE interrupt is temporarily disabled

CPU pushes the character into the ring buffer

If the buffer was empty:

The same character is also written into TDR

This clears TXE and starts transmission

TXE interrupt is re-enabled

2️⃣ Subsequent Characters

CPU only pushes characters into the ring buffer

CPU never writes to TDR again

3️⃣ ISR-Controlled Transmission

When hardware finishes shifting data:

TXE = 1

Since TXEIE is enabled:

USART interrupt triggers

ISR:

Fetches next byte from the ring buffer

Writes it into TDR

Continues until the buffer is empty

Why This Matters

✔ No polling

✔ Minimal CPU usage

✔ Deterministic TX behavior

✔ Clean ISR ↔ main synchronization

🔄 Buffer Layer
Ring Buffer (ringbuffer.c / ringbuffer.h)

Implements a 16-byte circular buffer

Manages:

head / tail movement

full and empty conditions

Provides controlled APIs:

put_ring_buff()

get_ring_buff()

Acts as the bridge between:

application code

USART ISR

🖨️ Custom printf() Layer
myprintf.c / myprintf.h

Lightweight printf() using variadic arguments

Parses format strings character-by-character

Supports:

%d → integer conversion (handled in USART driver)

%s → string transmission

%c → single character transmission

Non-format characters are transmitted directly

Fully reuses USART driver APIs

Initialization Abstraction

myprintf_init() performs all hardware setup:

GPIO configuration

USART configuration

Alternate function mapping

TX interrupt enabling

This keeps main.c clean and minimal.

⏱️ SysTick Delay Utility
systick.c / systick.h

Uses Cortex-M SysTick

Provides blocking, second-based delays

systick_delayS(2); // 2 seconds
systick_delayS(3); // 3 seconds

🚀 Example Usage
int main(void)
{
    myprintf_init();

    while (1)
    {
        myprintf("testing%d", 21);
        systick_delayS(2);
    }
}


All configuration is handled internally.
The superloop focuses only on application logic.

🧪 What This Project Demonstrates

Direct register-level programming

Interrupt-driven peripheral design

Ring buffer implementation

Variadic function handling

Layered firmware architecture

Realistic embedded driver patterns

📌 Target Platform

MCU: STM32F401RE

Core: ARM Cortex-M4

Language: C

Environment: Bare-metal (CMSIS only)

📚 Motivation

This project was built to deeply understand how printf() works in embedded systems, how UART drivers are designed internally, and how interrupt-driven communication is implemented without abstraction layers.
