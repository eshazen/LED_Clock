#include <stdint.h>

void ds3231_get_time(uint8_t *sec, uint8_t *min, uint8_t *hour,
                     uint8_t *day, uint8_t *date,
                     uint8_t *month, uint8_t *year);

