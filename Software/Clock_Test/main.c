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

int main() {

  uart_init();

  while(1) {
    ds3231_get_time(&sec, &min, &hour, &day, &date, &month, &year);

    u8_to_str( hour, buff);
    uart_tx_string( buff);
    uart_tx_string( ":");
    u8_to_str( min, buff);
    uart_tx_string( ":");
    u8_to_str( sec, buff);
    uart_tx_string( buff);
    uart_tx_string("\r\n");

    _delay_ms(1000);
  }
}
