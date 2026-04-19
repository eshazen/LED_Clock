
Provide real-time clock support for various clocks (including the round LED_Clock).
Initialize and read a DS3231 RTC.

Look for button presses to update H, M, S at 5Hz.  When update seen,
reset the DS3231 and send a time string '[hh:mm:ss]' to serial port
at 1200 baud.

Send a time update every 10s regardless of setting.

