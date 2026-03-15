
#include <stdio.h>
#include <stdlib.h>

#define MMAX 60

#define wrap(n,m) ((n+m)%m)

void display( unsigned min) {
  //  printf("%d %d %d\n", (MMAX+(min-1)) % MMAX, (MMAX+min) % MMAX, (MMAX+(min+1)) % MMAX);
  printf("%d %d %d\n", wrap(min-1,MMAX), wrap(min,MMAX), wrap(min+1,MMAX));
}

int main( int argc, char *argv[]) {

  int a = atoi( argv[1]);
  display( a);

}
