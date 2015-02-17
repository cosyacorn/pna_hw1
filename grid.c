#include <stdlib.h> 
#include "Machine.h" 
#include "Grid.h" 

// -------------------------------------

void check_grid(Grid* g)
{

// Check machine size compatible...
  if (host.p_nx * host.p_ny != host.np)
  {
    pprintf("%ZIncompatible grid parallelisation: %d x %d on %d processes\n",
        host.p_nx,host.p_ny,host.np);
    MPI_Abort(MPI_COMM_WORLD,1); 
  }

// Check local geometry fits easily
  g->nx_local = g->nx / host.p_nx;
  if (g->nx_local * host.p_nx != g->nx)
  {
    pprintf("%ZGrid mismatch in X-dir\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }
  if (g->nx_local % 2 == 1)
  {
    pprintf("%ZLocal X grid size must be even\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }

  g->ny_local = g->ny / host.p_ny;
  if (g->ny_local * host.p_ny != g->ny)
  {
    pprintf("%ZGrid mismatch in Y-dir\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }
  if (g->ny_local % 2 == 1)
  {
    pprintf("%ZLocal Y grid size must be even\n");
    MPI_Abort(MPI_COMM_WORLD,1); 
  }

  g->x_offset = host.p_x * g->nx_local;
  g->y_offset = host.p_y * g->ny_local;

  // pprintf("Offsets: %d %d\n",g->x_offset,g->y_offset);


  pprintf("%ZRunning Gauss-Seidel solver on %d processes\n",host.np);
  pprintf("%ZGrid size = %d x %d\n",g->nx,g->ny);

}


Grid* init_grid(char *argv[])
{
  Grid* g = malloc(sizeof(Grid)); 
  g->nx = atoi(argv[1]);
  g->ny = atoi(argv[2]);


  check_grid(g);
  return g;
}

void free_grid(Grid *g)
{
}
// =======================================

