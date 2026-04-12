#include <stdint.h>

// Converts 0–255 into ASCII string
// buffer must be at least 4 bytes: "255\0"
void u8_to_str(uint8_t value, char *buffer) {
    char temp[3];
    uint8_t i = 0;

    // Generate digits in reverse order
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    // Reverse into output buffer
    uint8_t j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';
}
