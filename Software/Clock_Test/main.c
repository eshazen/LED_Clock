/*
 * Simple serial RTC firmware
 *
 * Read time from DS3231 module via I2C and send as [hh:mm:ss]
 * via software uart at 1200 baud on PB0.
 *
 * Set buttons are on PA0..PA2
 * PA0 - set hours
 * PA1 - set minutes
 * PA2 - reset seconds to 00
 */

#include <avr/io.h>
#include <util/delay.h>

#include "ds3231.h"
#include "uart_tx.h"
#include "u8_str.h"

uint8_t sec, min, hour, day, date, month, year;

char buff[10];

void send_time() {
    ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);
    uart_tx_byte( '[');
    u8_to_str2( hour, buff);
    uart_tx_string( buff);
    uart_tx_byte( ':');
    u8_to_str2( min, buff);
    uart_tx_string( buff);
    uart_tx_byte( ':');
    u8_to_str2( sec, buff);
    uart_tx_string( buff);
    uart_tx_byte( ']');
    uart_tx_string("\r\n");
}

// button scan delay
#define SCAN_MS 200

// number of scan periods between clock updates (1 min per update)
// #define UPDATE_TICKS (60000L/SCAN_MS)
#define UPDATE_TICKS 50

int main() {

  uint8_t b_set, tick, send_delay;

  uart_init();
  PORTA |= 7;			/* pull-ups on PA0..PA2 (buttons) */

  tick = 0;
  send_delay = 0;

  while(1) {
    // check for set buttons
    b_set = (PINA & 7) ^ 7;

    if( b_set) {		/* any set buttons pressed? */
      ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);

      if( b_set & 1) {		/* hour set */
	hour++;
	if( hour > 23)
	  hour -= 24;
      }

      if( b_set & 2) {		/* min set */
	min++;
	if( min > 59)
	  min = 0;
      }

      if( b_set & 4) {		/* sec set */
	sec = 0;
      }
      ds3231_set_time(sec, min, hour, day, date, month, year);
      send_time();
    }

    _delay_ms(SCAN_MS);		/* check buttons at 5Hz */

    ++tick;
    if( tick >= UPDATE_TICKS) {
      tick = 0;
      send_time();
    }
  }
}
