#define BOUNDARY_PASSIVE 0
#define BOUNDARY_ACTIVE 1

typedef struct
{
  double *active_data;  // Storage for the actively managed data-points
  double **value;	// Access pointers
  // double *boundary[4];
  int boundary_type[4];
}
Field;

Field* init_field(Grid* g, double (*init)(int, int, Grid*));
void free_field(Field* f);
void write_field(char *, Field*, Grid*);
