/*
 * Blinky for ATTiny84 with LEDs on PB0, PB1
 *
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

int main() {

  uart_init();
  PORTA |= 7;			/* pull-ups on PA0..PA2 (buttons) */

  while(1) {
    send_time();

    if( !(PINA & 1)) {		/* hour set */
      ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);
      hour++;
      if( hour > 23)
	hour -= 24;
      ds3231_set_time(sec, min, hour, day, date, month, year);
    }

    if( !(PINA & 2)) {		/* min set */
      ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);
      min++;
      if( min > 59)
	min = 0;
      ds3231_set_time(sec, min, hour, day, date, month, year);
    }

    if( !(PINA & 4)) {		/* sec set */
      ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);
      sec++;
      if( sec > 59)
	sec = 0;
      ds3231_set_time(sec, min, hour, day, date, month, year);
    }
    

    _delay_ms(250);
  }
}
