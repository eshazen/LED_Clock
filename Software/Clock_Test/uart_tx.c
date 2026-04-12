//
// ChatGPT Software UART
//

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define TX_PIN PB0

// Bit delay for 1200 baud @ 1 MHz
static inline void uart_delay(void) {
    _delay_us(833);
}

// Initialize TX pin (idle = HIGH)
void uart_init(void) {
    DDRB |= (1 << TX_PIN);   // output
    PORTB |= (1 << TX_PIN);  // idle high
}

// Send one byte (8N1)
void uart_tx_byte(uint8_t data) {
    // Start bit (LOW)
    PORTB &= ~(1 << TX_PIN);
    uart_delay();

    // 8 data bits (LSB first)
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x01)
            PORTB |= (1 << TX_PIN);
        else
            PORTB &= ~(1 << TX_PIN);

        uart_delay();
        data >>= 1;
    }

    // Stop bit (HIGH)
    PORTB |= (1 << TX_PIN);
    uart_delay();
}

// Send string
void uart_tx_string(const char *s) {
    while (*s) {
        uart_tx_byte(*s++);
    }
}
