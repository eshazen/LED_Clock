
#include <Arduino.h>

const int LED_D0_PIN = 2;
const int LED_D1_PIN = 3;
const int LED_D2_PIN = 4;
const int LED_D3_PIN = 5;
const int LED_D4_PIN = 6;
const int LED_D5_PIN = 7;
const int LED_D6_PIN = 8;
const int LED_D7_PIN = 9;

const int LED_MODE = 23;
const int LED_WRITE = 24;

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

void led_write( uint8_t mode, uint8_t d) {
  digitalWrite( LED_MODE, mode);
  led_data( d);
  digitalWrite( LED_WRITE, LOW);
  digitalWrite( LED_WRITE, HIGH);
}


const uint8_t dpyblk = 0;	// blank the display
const uint8_t dpymod = 0xb0;	// no decode, data coming, not blanked
const uint8_t dpyhex = 0xd0;	// hex decode, data coming, not blanked

void do7218( uint8_t IX[8]) {
  led_write( 1, dpymod);
  for( int i=0; i<8; i++)
    led_write( 0, IX[i]);
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
  pinMode( LED_WRITE, OUTPUT);
  digitalWrite( LED_WRITE, HIGH);
}

static uint8_t led_blank[] = { 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t led_light[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void loop() {

  do7218( led_blank);
  delay(1000);
  do7218( led_light);
  delay(1000);
  
}
