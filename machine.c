#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Machine.h" 

// ====================================================================
// ==================== A parallel printf function ====================
// ====================================================================

void pprintf(char *format, ...)
{
  va_list l;
  char* new_format; 

  va_start(l,format);

// RANK-ZERO ONLY
  if (format[0]=='%' && format[1]=='Z')
  {
    if (host.rank==0)
    {
      format+=2; 
      vprintf(format,l);
    }
  }
  else
  {
// GLOBAL

// problem with buffering -- make format and send to vprintf
    new_format = malloc(sizeof(char)*strlen(format)+6);

    sprintf(new_format,"P%d:%s",host.rank,format);
    vprintf(new_format,l);

    free(new_format); 
  }
  va_end(l);
}




void init_machine(int argc, char *argv[])
{
	  if (argc != 3) MPI_Abort(MPI_COMM_WORLD,1);

	//host.p_nx = atoi(argv[3]);
  	//host.p_ny = atoi(argv[4]);

	host.p_nx=4;
	host.p_ny=2;

  MPI_Comm_size(MPI_COMM_WORLD,&host.np);
  MPI_Comm_rank(MPI_COMM_WORLD,&host.rank);

  //host.p_x = host.rank % host.p_nx;
  //host.p_y = host.rank / host.p_nx;

  pprintf("%ZMachine geometry %d x %d\n",host.p_nx,host.p_ny);

// Work out neighbouring process ranks

	if(host.rank==0){
		host.p_x=2;
		host.p_y=0;
		host.neighbour[NORTH]=1;
		host.neighbour[SOUTH]=-1;
		host.neighbour[EAST]=-1;
		host.neighbour[WEST]=-1;
	}
	
	if(host.rank==1){
		host.p_x=2;
		host.p_y=1;
		host.neighbour[NORTH]=-1;
		host.neighbour[SOUTH]=0;
		host.neighbour[EAST]=-1;
		host.neighbour[WEST]=2;
	}

	if(host.rank==2){
		host.p_x=1;
		host.p_y=1;
		host.neighbour[NORTH]=-1;
		host.neighbour[SOUTH]=-1;
		host.neighbour[EAST]=1;
		host.neighbour[WEST]=3;
	}

	if(host.rank==3){
		host.p_x=0;
		host.p_y=1;
		host.neighbour[NORTH]=4;
		host.neighbour[SOUTH]=-1;
		host.neighbour[EAST]=2;
		host.neighbour[WEST]=-1;
	}

	if(host.rank==4){
		host.p_x=0;
		host.p_y=2;
		host.neighbour[NORTH]=5;
		host.neighbour[SOUTH]=3;
		host.neighbour[EAST]=-1;
		host.neighbour[WEST]=-1;
	}

	if(host.rank==5){
		host.p_x=0;
		host.p_y=3;
		host.neighbour[NORTH]=-1;
		host.neighbour[SOUTH]=4;
		host.neighbour[EAST]=6;
		host.neighbour[WEST]=-1;
	}

	if(host.rank==6){
		host.p_x=1;
		host.p_y=3;
		host.neighbour[NORTH]=-1;
		host.neighbour[SOUTH]=-1;
		host.neighbour[EAST]=7;
		host.neighbour[WEST]=5;
	}

	if(host.rank==7){
		host.p_x=2;
		host.p_y=3;
		host.neighbour[NORTH]=-1;
		host.neighbour[SOUTH]=-1;
		host.neighbour[EAST]=-1;
		host.neighbour[WEST]=6;
	}


/*  if (host.p_x == 0)
    host.neighbour[WEST]=-1;
  else 
    host.neighbour[WEST]=host.p_x-1+host.p_y*host.p_nx;

  if (host.p_x == host.p_nx-1)
    host.neighbour[EAST]=-1;
  else 
    host.neighbour[EAST]=host.p_x+1+host.p_y*host.p_nx;

  if (host.p_y == 0)
    host.neighbour[SOUTH]=-1;
  else 
    host.neighbour[SOUTH]=host.p_x+(host.p_y-1)*host.p_nx;

  if (host.p_y == host.p_ny-1)
    host.neighbour[NORTH]=-1;
  else 
    host.neighbour[NORTH]=host.p_x+(host.p_y+1)*host.p_nx;
*/
  pprintf(
"Grid co-ords are [%d,%d], neighbours: N=%d,S=%d,E=%d,W=%d\n",
     host.p_x,host.p_y,
     host.neighbour[NORTH], host.neighbour[SOUTH],
     host.neighbour[EAST ], host.neighbour[WEST ]
  );
}
