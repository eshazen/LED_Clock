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



