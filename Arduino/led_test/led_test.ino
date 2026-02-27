
#include <Arduino.h>

#include "led_map.h"



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


void led_data( uint8_t d) {
  digitalWrite( LED_D0_PIN, d & _BV(0));
  digitalWrite( LED_D1_PIN, d & _BV(1));
  digitalWrite( LED_D2_PIN, d & _BV(2));
  digitalWrite( LED_D3_PIN, d & _BV(3));
  digitalWrite( LED_D4_PIN, d & _BV(4));
  digitalWrite( LED_D5_PIN, d & _BV(5));
  digitalWrite( LED_D6_PIN, d & _BV(6));
  digitalWrite( LED_D7_PIN, d & _BV(7));
}

// perform a write cycle to the 7218 with specified mode and sector
void led_write( int sec, uint8_t mode, uint8_t d) {
  digitalWrite( LED_MODE, mode);
  led_data( d);
  digitalWrite( sec_wr[sec], LOW);
  digitalWrite( sec_wr[sec], HIGH);
}


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


void setup() {
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
  

}

void loop() {

  uint8_t* p = (uint8_t *)dring;

  for( int i=0; i<60; i++) {
    p[i] = 0x7f;
    update_display( dring);
    delay(250);
    p[i] = 0;
    update_display( dring);
  }
  
}
