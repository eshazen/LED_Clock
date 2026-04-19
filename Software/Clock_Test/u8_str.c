#include <stdint.h>
#include <string.h>

// convert 0-99 to string with leading zeroes
void u8_to_str2(uint8_t value, char *buffer) {
  if( value > 99)
    strcpy( buffer, "99");
  else {
    buffer[0] = value / 10 + '0';
    buffer[1] = value % 10 + '0';
  }
}

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
