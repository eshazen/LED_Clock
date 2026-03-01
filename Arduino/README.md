
The sketch `clock_v2` is the current working one
with set buttons.  Timekeeping just uses the
ESP32 timer interrupts... seems to be pretty stable.

`led_test` is useful for debugging LED issues.

`clock_ntp` is an attempt to get the time from NPT
servers but doesn't work reliably.
