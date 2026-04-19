# LED_Clock

New version `clock_v3` expects time-set data at 1200 baud
on D12.  Set string format `[hh:mm:ss]`.
Runs with timer interrups between set commands

Previous sketch `clock_v2` uses
set buttons.  Timekeeping just uses the
ESP32 timer interrupts.  Loses ~1m per day.

`led_test` is useful for debugging LED issues.

`clock_ntp` is an attempt to get the time from NPT
servers but doesn't work reliably.
