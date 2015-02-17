// #include <stdarg.h>

#include "mpi.h"

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

typedef struct 
{
  int np;
  int rank;
  int p_nx;
  int p_ny;
  int p_x;
  int p_y;

  int neighbour[4];
}
Machine; 

extern Machine host; 

void init_machine(int argc, char *argv[]); 
void pprintf(char *format, ...);

