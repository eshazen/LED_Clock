//
// ChatGPT code to read/write the RTC
//


#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SDA_PIN PA6
#define SCL_PIN PA4

#define SDA_HIGH() DDRA &= ~(1 << SDA_PIN)   // input (pull-up handles high)
#define SDA_LOW()  do { DDRA |= (1 << SDA_PIN); PORTA &= ~(1 << SDA_PIN); } while(0)

#define SCL_HIGH() DDRA &= ~(1 << SCL_PIN)
#define SCL_LOW()  do { DDRA |= (1 << SCL_PIN); PORTA &= ~(1 << SCL_PIN); } while(0)

#define SDA_READ() (PINA & (1 << SDA_PIN))

#define DS3231_ADDR 0x68

// ---------------- I2C primitives ----------------

void i2c_delay() {
    _delay_us(5);
}

void i2c_start() {
    SDA_HIGH();
    SCL_HIGH();
    i2c_delay();

    SDA_LOW();
    i2c_delay();

    SCL_LOW();
}

void i2c_stop() {
    SDA_LOW();
    i2c_delay();

    SCL_HIGH();
    i2c_delay();

    SDA_HIGH();
    i2c_delay();
}

uint8_t i2c_write(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) SDA_HIGH();
        else SDA_LOW();

        SCL_HIGH();
        i2c_delay();
        SCL_LOW();
        i2c_delay();

        data <<= 1;
    }

    // ACK bit
    SDA_HIGH();
    SCL_HIGH();
    i2c_delay();

    uint8_t ack = !(SDA_READ());

    SCL_LOW();
    return ack;
}

uint8_t i2c_read(uint8_t ack) {
    uint8_t data = 0;

    SDA_HIGH(); // release line

    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;

        SCL_HIGH();
        i2c_delay();

        if (SDA_READ()) data |= 1;

        SCL_LOW();
        i2c_delay();
    }

    // send ACK/NACK
    if (ack) SDA_LOW();
    else SDA_HIGH();

    SCL_HIGH();
    i2c_delay();
    SCL_LOW();

    SDA_HIGH();

    return data;
}

// ---------------- BCD helpers ----------------

uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

uint8_t bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// ---------------- DS3231 functions ----------------

void ds3231_write_reg(uint8_t reg, uint8_t val) {
    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 0);
    i2c_write(reg);
    i2c_write(val);
    i2c_stop();
}

uint8_t ds3231_read_reg(uint8_t reg) {
    uint8_t val;

    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 0);
    i2c_write(reg);

    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 1);
    val = i2c_read(0);
    i2c_stop();

    return val;
}

// ---------------- Public API ----------------

// Set time: 24-hour format
void ds3231_set_time(uint8_t sec, uint8_t min, uint8_t hour,
                     uint8_t day, uint8_t date,
                     uint8_t month, uint8_t year) {

    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 0);
    i2c_write(0x00); // start at seconds register

    i2c_write(dec_to_bcd(sec));
    i2c_write(dec_to_bcd(min));
    i2c_write(dec_to_bcd(hour)); // 24h mode
    i2c_write(dec_to_bcd(day));
    i2c_write(dec_to_bcd(date));
    i2c_write(dec_to_bcd(month));
    i2c_write(dec_to_bcd(year));

    i2c_stop();
}

// Read time
void ds3231_get_time(uint8_t *sec, uint8_t *min, uint8_t *hour,
                     uint8_t *day, uint8_t *date,
                     uint8_t *month, uint8_t *year) {

    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 0);
    i2c_write(0x00);

    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 1);

    *sec   = bcd_to_dec(i2c_read(1) & 0x7F);
    *min   = bcd_to_dec(i2c_read(1));
    *hour  = bcd_to_dec(i2c_read(1) & 0x3F); // 24-hour
    *day   = bcd_to_dec(i2c_read(1));
    *date  = bcd_to_dec(i2c_read(1));
    *month = bcd_to_dec(i2c_read(1));
    *year  = bcd_to_dec(i2c_read(0));

    i2c_stop();
}
