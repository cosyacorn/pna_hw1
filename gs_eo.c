#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#include "Machine.h" 
Machine host; 

#include "Grid.h" 
#include "Field.h" 
#include "Comms.h" 

// =======================================

// Return Dirichlet data for boundary b (N,S,E,W) at location k
double boundary(int b, int k, Grid *g)
{
	double q;
	

	if (b==NORTH) 
		return 0.0;
    // { q = ((double) k) / ((double) g->nx); return q*q; }
	if (b==SOUTH) {
		if (host.rank==0) {
			return 1.0;
		} else {
			q = ((double) k) / ((double) g->nx);
			return 1.0 - cos(10 * M_PI * q);
			
		}
	}
    // { q = ((double) k) / ((double) g->nx); return 1.0 - sin(10.5 * M_PI * q);}
	if (b==EAST) {
		if( host.rank==7 ){
			return 0.0;
		} else {
			q = ((double) k) / ((double) g->ny);
			return cos(k)*sqrt(q);
		}
	}
	if (b==WEST) 
		return 0.0;
    // return 1.0;
}

// ------------------------------
double update(Field* phi, int eo, Grid* g)
{
  int x,y,p=eo;
  double diff,diff2_local=0.0,diff2_global;
  double omega=1.9; // Over-relax

// Update using GS iteration
	for (x=0;x<g->nx_local;x++){
		for (y=p;y<g->ny_local;y+=2){
			diff = phi->value[x][y];
			if(x==0 && host.neighbour[WEST]==-1) phi->value[x][y]=phi->value[1][y];
			else if(y==0 && host.neighbour[SOUTH]==-1 && host.rank!=0) phi->value[x][y]=phi->value[x][1];
			else if(x==g->nx_local-1 && host.neighbour[EAST]==-1 && host.rank!=7) phi->value[x][y]=phi->value[x-1][y];
			else if(y==g->ny_local-1 && host.neighbour[NORTH]==-1) phi->value[x][y]=phi->value[x][y-1];
			else
			phi->value[x][y] = (1.0-omega)*phi->value[x][y] + omega * 0.25 * (phi->value[x+1][y] + phi->value[x-1][y] + phi->value[x][y+1] + phi->value[x][y-1]);
			diff -= phi->value[x][y];
			diff2_local += diff*diff;
		}
		p=1-p; 
	}
  
  MPI_Allreduce(&diff2_local,&diff2_global,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
  
// Communicate boundaries in new
  send_boundary_data(phi,eo,g); 

  return diff2_global;
}

int main(int argc, char *argv[])
{
  Grid* g;
  Field* phi; 
  int i=0;
  double diff2;

  MPI_Init(&argc,&argv);
  init_machine(argc,argv); 

  g = init_grid(argv); 

// Only one field for GS algorithm
  phi = init_field(g,boundary); 

// iterate...
  do 
  {
    diff2 = update(phi,0,g);
    diff2 = update(phi,1,g);

  if (i%100==99) pprintf("%Z%d %e\n",i,diff2);
  // pprintf("%Z%d %e\n",i,diff2);
    i++;
  }
  while (diff2>1.0e-6);

  write_field("grid.out",phi,g);

  free_grid(g);
  free_field(phi);
  MPI_Finalize();
}
