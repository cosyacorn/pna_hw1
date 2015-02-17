typedef struct 
{
  int nx;
  int ny; 

  int nx_local;
  int ny_local; 

  int x_offset; // Array offsets
  int y_offset;
}
Grid; 

// -------------------------------------
Grid* init_grid(char *argv[]); 
void free_grid(Grid *g); 

