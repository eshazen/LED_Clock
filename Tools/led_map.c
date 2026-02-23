#include <stdint.h>
#include <stdio.h>

// map LED locations in (ring, pos) to (byte, bit)
// where ring = 0..6
// and pos = 0..9 except in ring 5 where only 0, 5 are valid

// segment map:  7 6 5 4 3 2 1 0
//              dp a b c e g f d

typedef struct {
  int byt;
  int bit;
} a_tuple;

typedef a_tuple a_ring[10];

//                       POS:    0       1       2       3       4      5       6       7       8       9
const a_ring led_map[7] = { { {5, 6}, {5, 5}, {5, 4}, {5, 0}, {5, 3}, {5, 1}, {5, 2}, {5, 7}, {7, 3}, {7, 1} },    // ring 0
                            { {4, 6}, {4, 5}, {4, 4}, {4, 0}, {4, 3}, {4, 1}, {4, 2}, {4, 7}, {7, 4}, {7, 0} },    // ring 1
			    { {3, 6}, {3, 5}, {3, 4}, {3, 0}, {3, 3}, {3, 1}, {3, 2}, {3, 7}, {7, 6}, {7, 5} },    // ring 2
			    { {2, 6}, {2, 5}, {2, 4}, {2, 0}, {2, 3}, {2, 1}, {2, 2}, {2, 7}, {6, 2}, {6, 7} },    // ring 3
			    { {1, 6}, {1, 5}, {1, 4}, {1, 0}, {1, 3}, {1, 1}, {1, 2}, {1, 7}, {6, 0}, {6, 3} },    // ring 4
			    { {6, 6}, {9, 9}, {9, 9}, {9, 9}, {9, 9}, {6, 1}, {9, 9}, {9, 9}, {9, 9}, {9, 9} },    // ring 5
                            { {0, 6}, {0, 5}, {0, 4}, {0, 0}, {0, 3}, {0, 1}, {0, 2}, {0, 7}, {6, 5}, {6, 4} } };  // ring 6

// sector bytes.  bits 0..6 represent LEDs from inner to outer
uint8_t sector[10];

// LED driver data
uint8_t dled[8];

char segs[] = "dfgecbap---";

#define OVERWRITE_BIT(var, pos, val) \
    ( (var) = ( (var) & ~(1U << (pos)) ) | ( (!!(val)) << (pos) ) )

// update sector from array
void update_sector( uint8_t* s, uint8_t* d) {
  // set up LED data
  for( int ring=0; ring<7; ring++) {
    //    printf("Ring %d:\n", ring);
    for( int pos=0; pos<10; pos++) {
      int byt = led_map[ring][pos].byt;
      int bit = led_map[ring][pos].bit;
      //      printf( "  pos %d byt=%d seg=%c\n", pos, byt, segs[bit]);
      if( byt < 8 && bit < 8) {
	int val = !!(s[pos] & (1<<ring));
	OVERWRITE_BIT( s[byt], bit, val);
      }
    }
  }
}

int main( int argc, char *argv) {
  update_sector( sector, dled);
}
