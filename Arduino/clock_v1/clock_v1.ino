//
// First attempt at clock display
//

#include <Arduino.h>
#include <ESP32Time.h>

#include "led_map.h"

ESP32Time rtc( -18000);


// display mapping for 1/6 sectors in dring[]
//
// bit
//   6 | w  w  w  w  w  w  w  w  w  w
//   5 | R              R
//   4 | G  G  G  G  G  G  G  G  G  G
//   3 | G  G  G  G  G  G  G  G  G  G
//   2 | B  B  B  B  B  B  B  B  B  B
//   1 | B  B  B  B  B  B  B  B  B  B
//   0 | B  B  B  B  B  B  B  B  B  B
//     +------------------------------
//pos    0  1  2  3  4  5  6  7  8  9
//
uint8_t dring[LED_NSEC][LED_NPOS];   // ring data for 6 sectors, 10 positions
uint8_t dled[8];	   // 7218 data for one sector

// Arduino pin numbers for 7218 WR for each sector
const int sec_wr[LED_NSEC] = { 2, 3, 4, 5, 6, 7};

// constant arrays with all LEDs lit and all blanked (note: D.P. is inverted)
static const uint8_t led_blank[] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
static const uint8_t led_light[] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f };

// Arduino pin numbers for 7218 data
const int LED_D0_PIN = A0;
const int LED_D1_PIN = A1;
const int LED_D2_PIN = A2;
const int LED_D3_PIN = A3;
const int LED_D4_PIN = A4;
const int LED_D5_PIN = A5;
const int LED_D6_PIN = A6;
const int LED_D7_PIN = A7;

// Arduino pin number for 7218 MODE
const int LED_MODE = 8;

// assert a data byte on the 7218 data bus
void led_data( uint8_t d) {
  digitalWrite( LED_D0_PIN, d & _BV(0)); // d
  digitalWrite( LED_D1_PIN, d & _BV(1)); // f
  digitalWrite( LED_D2_PIN, d & _BV(2)); // g
  digitalWrite( LED_D3_PIN, d & _BV(3)); // e
  digitalWrite( LED_D4_PIN, d & _BV(4)); // c
  digitalWrite( LED_D5_PIN, d & _BV(5)); // b
  digitalWrite( LED_D6_PIN, d & _BV(6)); // a
  digitalWrite( LED_D7_PIN, d & _BV(7)); // a  
}

// perform a write cycle to the 7218 with specified mode and sector
void led_write( int sec, uint8_t mode, uint8_t d) {
  digitalWrite( LED_MODE, mode);
  led_data( d);
  digitalWrite( sec_wr[sec], LOW);
  digitalWrite( sec_wr[sec], HIGH);
}

// 7218 commands
const uint8_t dpyblk = 0;	// blank the display
const uint8_t dpymod = 0xb0;	// no decode, data coming, not blanked
const uint8_t dpyhex = 0xd0;	// hex decode, data coming, not blanked

// update a 7218 from the specified data
void do7218( int sec, const uint8_t IX[8]) {
  led_write( sec, 1, dpymod);
  for( int i=0; i<8; i++)
    led_write( sec, 0, IX[i]);
}

// update the whole display from 60 bytes of data
void update_display( uint8_t dat[LED_NSEC][LED_NPOS]) {
  for( int i=0; i<LED_NSEC; i++) {
    update_sector( dat[i], dled);
    do7218( i, dled);
  }
}


// display the time
void display_time( int hr, int min, int sec, uint8_t (&dat)[LED_NSEC][LED_NPOS]) {
  uint8_t* pd = (uint8_t *)dat;
  memset( dat, 0, sizeof(dat));

  if( sec < 0 || sec > 59 || hr < 0 || hr > 12 || min < 0 || min > 59)
    return;

  pd[sec] |= 0x40;		// set second bit in outer ring
  pd[min] |= 0x18;		// set two bits in green rings
  if( hr > 12)
    hr -= 12;
  if( hr == 12)
    hr = 0;
  pd[hr] |= 7;			// set 3 bits in blue rings
  update_display( dat);
}


void setup() {
  pinMode( LED_BUILTIN, OUTPUT);
  pinMode( LED_D0_PIN, OUTPUT);
  pinMode( LED_D1_PIN, OUTPUT);
  pinMode( LED_D2_PIN, OUTPUT);
  pinMode( LED_D3_PIN, OUTPUT);
  pinMode( LED_D4_PIN, OUTPUT);
  pinMode( LED_D5_PIN, OUTPUT);
  pinMode( LED_D6_PIN, OUTPUT);
  pinMode( LED_D7_PIN, OUTPUT);
  pinMode( LED_MODE, OUTPUT);
  for( int i=0; i<LED_NSEC; i++) {
    pinMode( sec_wr[i], OUTPUT);
    digitalWrite( sec_wr[i], HIGH);
  }
  // blank the display for all sectors
  for( int i=0; i<LED_NSEC; i++)
    do7218( i, led_blank);

  // run a test sweep radar-style
  for( int n=0; n<5; n++) {
    memset( dring, 0, sizeof(dring));
    uint8_t *pd = (uint8_t *)dring;
    for( int s=0; s<LED_NSEC; s++) {
      for( int p=0; p<LED_NPOS; p++) {
	dring[s][p] = 0x7f;
	if( p>0 && s>0)
	  *pd = 0;
	++pd;
	update_display( dring);
	delay(10);
      }
    }
  }

  // should get the time from RTC etc
  rtc.setTime( 12, 25, 30, 24, 2, 2026);
}


void loop() {
  display_time( rtc.getHour(), rtc.getMinute(), rtc.getSecond(), dring);
  delay(200);
}
