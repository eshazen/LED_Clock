# LED Clock

Yet another LED pseudo-analog clock.
<br>6 concentric circles of 60 LEDs plus one of 12 LEDs.
<br>Arduino Nano ESP32.

Current code (`clock_v2`) just uses 1Hz timer interrupts
and provides hour / minute set buttons.

No ECOs on the PC board.  Forgot mounting holes though.
See `Case` folder for a two-part 3D printed wall hangar
with a little control panel for the set buttons.

Works OK but loses ~1min/day.  Need to work on the timekeeping.

<img src="images/front.jpg" width="400" alt="Front View">
