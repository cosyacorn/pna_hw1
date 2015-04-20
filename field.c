#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "Machine.h" 
#include "Grid.h" 
#include "Field.h" 

Field* init_field(Grid* g, double (*init)(int, int, Grid*))
{
	int size;
	int i,x,y;
	Field *f = malloc(sizeof(Field));

	// Malloc space for active data
	size = (g->nx_local+2) * (g->ny_local+2);
	f->active_data = malloc(sizeof(double) * size); 
	for (i=0;i<size;i++)
		f->active_data[i] = 0.0;

	// Assign pointers to access data
	f->value = malloc(sizeof(double *) * (g->nx_local+2) );
	for (x=0;x<g->nx_local+2;x++)
		f->value[x] = f->active_data + x * (g->ny_local+2) + 1;
	f->value++; // shift to start array at -1

	// Now decide if the four boundarys are active (need data updates from other
	// processes) or passive (are actual edges of the grid, so stay unchanged)
	for (i=0;i<4;i++) 
		if (host.neighbour[i]==-1) 
			f->boundary_type[i] = BOUNDARY_PASSIVE;
		else 
			f->boundary_type[i] = BOUNDARY_ACTIVE;

	pprintf("boundary types: N=%d,S=%d,E=%d,W=%d\n", f->boundary_type[NORTH], f->boundary_type[SOUTH], f->boundary_type[EAST], f->boundary_type[WEST]);


	// Load boundary data from initialiser function
	if (f->boundary_type[NORTH] == BOUNDARY_PASSIVE)
		for (x=0;x<g->nx_local;x++)
			f->value[x][g->ny_local] = init(NORTH,x+g->x_offset,g);
			//f->value[x][g->ny_local] = (4*f->value[x][g->ny_local-1];
	else 
		for (x=0;x<g->nx_local;x++)
			f->value[x][g->ny_local] = 0.0;


	if (f->boundary_type[SOUTH] == BOUNDARY_PASSIVE ){
		for (x=0;x<g->nx_local;x++){
			//if(host.rank==0){
				f->value[x][-1         ] = init(SOUTH,x+g->x_offset,g);
			//} else {
				//f->value[x][-1] = (4*f->value[x][0]-f->value[x][1])/3;
			//}
		}
	} else {
    		for (x=0;x<g->nx_local;x++)
			f->value[x][-1         ] = 0.0;
	}

	if (f->boundary_type[EAST ] == BOUNDARY_PASSIVE )
		for (y=0;y<g->ny_local;y++){ 
			//if(host.rank==7)
				 f->value[g->nx_local][y] = init(EAST ,y+g->y_offset,g);
			//else f->value[g->nx_local][y] = f->value[g->nx_local-1][y];
		}
	else 
		for (y=0;y<g->ny_local;y++)
			f->value[g->nx_local][y] = 0.0;


	if (f->boundary_type[WEST ] == BOUNDARY_PASSIVE)
		for (y=0;y<g->ny_local;y++)
			 f->value[-1         ][y] = init(WEST ,y+g->y_offset,g);
			//f->value[-1][y] = (4*f->value[0][y]-f->value[1][y])/3;
	else 
		for (y=0;y<g->ny_local;y++)
			f->value[-1         ][y] = 0.0;

	return f;
}

void free_field(Field* f)
{
  int k;
  free(f->active_data);
  // Undo shift of f->value pointer before freeing 
  f->value--;
  free(f->value);
  free(f);
}

void write_field(char* filestub, Field *f, Grid *g)
{
  int x,y;
  FILE* o;
  char *filename = malloc(sizeof(char) * (strlen(filestub)+10));

  sprintf(filename,"%s.%.3d",filestub,host.rank);
  o = fopen(filename,"w");

  // Write the active part, with offsets. Don't write process identities
  for (x=0;x<g->nx_local;x++)
    for (y=0;y<g->ny_local;y++)
      fprintf(o,"%d %d %lf\n",x+g->x_offset,y+g->y_offset,f->value[x][y]);

  fclose(o);
  free(filename);
}
