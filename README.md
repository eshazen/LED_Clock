# LED Clock

Yet another LED pseudo-analog clock
<br>Boards and parts ordered

Looking to add RTC plus setting option.
<br>Only the UART is brought out, and it's UART0, which is shared
with the bootloader/kernel.  They an in principle be used as GPIO,
but serial messages come out during boot so not ideal for e.g. I2C.

There are two additional UARTs which can be mapped to any pin.

Probably the best bet long-term is an auxiliary module powered
by a 4-pin cable from the UART connector, with a keypad/LCD shield
and RTC module.

## 2026-01-21

`LED_Clock_sector` is the current design

LEDs are arranged in 6 sectors of 62 total LEDs, each driven by an
ICM7218A driver.  All placement/routing is duplicated radially.

## 2026-02-18

Thinking about a "mini" version (cheaper!).
The cost driver (half the price!) is the LED driver ($15 each, controls
64 LEDs).  An ATMega328 could do the same (possibly with transistors).
In principle it could handle a 12x10 (2x60) array using 22 I/Os.
This leaves exactly one for communication, and would require that the
SPI programming be shared with some LED drivers.  Maybe a better choice
would be e.g. AVR128DB32 ($2 each) with ~25 I/Os and UPDI.


