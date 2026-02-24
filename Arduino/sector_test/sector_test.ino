//
// sweep through one sector
//

#include <Arduino.h>

#include "led_map.h"

uint8_t dring[LED_NPOS];	// ring data for positions 0..9
uint8_t dled[8];	// 7218 data

const int sec_wr[] = { 2, 3, 4, 5, 6, 7};
#define LED_NSEC (sizeof(sec_wr)/sizeof(sec_wr[0]))

const int LED_D0_PIN = A0;
const int LED_D1_PIN = A1;
const int LED_D2_PIN = A2;
const int LED_D3_PIN = A3;
const int LED_D4_PIN = A4;
const int LED_D5_PIN = A5;
const int LED_D6_PIN = A6;
const int LED_D7_PIN = A7;

const int LED_MODE = 8;

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

void led_write( int sec, uint8_t mode, uint8_t d) {
  digitalWrite( LED_MODE, mode);
  led_data( d);
  digitalWrite( sec_wr[sec], LOW);
  digitalWrite( sec_wr[sec], HIGH);
}


const uint8_t dpyblk = 0;	// blank the display
const uint8_t dpymod = 0xb0;	// no decode, data coming, not blanked
const uint8_t dpyhex = 0xd0;	// hex decode, data coming, not blanked

void do7218( int sec, uint8_t IX[8]) {
  led_write( sec, 1, dpymod);
  for( int i=0; i<8; i++)
    led_write( sec, 0, IX[i]);
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
}

static uint8_t led_blank[] = { 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t led_light[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void loop() {

  digitalWrite( LED_BUILTIN, HIGH);
  // flash
  for( int i=0; i<5; i++) {
    int k = 0;
    //    for( int k=0; k<LED_NSEC; k++)
      do7218( k, led_blank);
    delay(100);
    //    for( int k=0; k<LED_NSEC; k++)
    do7218( k, led_light);
    digitalWrite( LED_BUILTIN, LOW);
    delay(100);
    digitalWrite( LED_BUILTIN, HIGH);
  }


 // chase
   memset( dring, 0, sizeof(dring));
   for( int pos=0; pos<LED_NPOS; pos++) {
     for( int ring=0; ring<LED_NRING; ring++) {
       OVERWRITE_BIT( dring[pos], ring, 1);
       update_sector( dring, dled);
       for( int s=0; s<LED_NSEC; s++)
 	do7218( s, dled);
       delay(50);
     }}

//  memset( dled, 0, sizeof(dled));
//
//  for( int w=0; w<8; w++) {
//    for( int b=0; b<8; b++) {
//      OVERWRITE_BIT( dled[w], b, 1);
//      do7218( dled);
//      delay(100);
//    }
//  }



}
