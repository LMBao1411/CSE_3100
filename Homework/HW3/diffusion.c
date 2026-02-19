#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int get_idx(int x, int y, int z, int n) {
	// We "slide" the whole box so that the most negative corner (-n, -n, -n) sits at (0, 0, 0)
	// To map a 3D cube to a 1D index, the index on the 1D array is defined as INDEX = (x) + (y * dim) + (z * dim * dim)
	// where dim is the width of one side of the cube 2n+1
	x += n;
	y += n;
	z += n;
	int dim = (2*n) + 1;
	return x + (y*dim) + (z*dim*dim); 
}

//TODO
//Implement the below function
//Simulate one particle moving n steps in random directions
//Use a random number generator to decide which way to go at every step
//When the particle stops at a final location, use the memory pointed to by grid to 
//record the number of particles that stop at this final location
//Feel free to declare, implement and use other functions when needed

void one_particle(int *grid, int n)
{
	int x=0, y=0, z=0;
	for (int i=0; i<n; i++) {
		int r = rand() % 6;
		if (r==0) {x--;}			// LEFT x
		else if (r==1) {x++;}		// RIGHT x
		else if (r==2) {y++;}		// UP y
		else if (r==3) {y--;}		// DOWN y
		else if (r==4) {z++;}		// UP z
		else if (r==5) {z--;}		// DOWN z
	}
	++grid[get_idx(x, y, z, n)];	// if a particle landed at this coord, increment by 1
}

//TODO
//Implement the following function
//This function returns the fraction of particles that lie within the distance
//r*n from the origin (including particles exactly r*n away)
//The distance used here is Euclidean distance
//Note: you will not have access to math.h when submitting on Mimir
double density(int *grid, int n, double r)
{
	// need 3 for loops (x times y times z times) to go through all points in the grid, at each point, compare its distance and the max distance r*n
	double max_dist = (r*n) * (r*n);
	double dist;
	int idx;
	int inside = 0, outside = 0;

	for (int x = -n; x <= n; x++) {
		for (int y =- n; y <= n; y++) {
			for (int z =- n; z <= n; z++) {
				dist = (x*x) + (y*y) + (z*z);
				idx = get_idx(x, y, z, n);
				if (dist <= max_dist) {
					inside += grid[idx];		// if this coord's distance is smaller the radius, add the amount of particles at this coord to 'inside'
				}
				else {
					outside += grid[idx];		// if this coord's distance is outside the radius, add the amount of particles at this coord to 'outside'
				}
			}
		}
	}

    return (double)inside / (inside + outside);
}

//use this function to print results
void print_result(int *grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

//TODO
//Finish the following function
//See the assignment decription on Piazza for more details
void diffusion(int n, int m)
{
	//fill in a few line of code below
	int size = 2*n + 1;
	int *grid = (int*)calloc(size*size*size, sizeof(int));			// (size x size x size) grid 3D cube
	if (grid == NULL) {
		printf("malloc failed\n"); 
		exit(1);
	}
	
	for(int i = 1; i<=m; i++) one_particle(grid, n);

	print_result(grid, n);
	//fill in some code below
	free(grid);
}

int main(int argc, char *argv[])
{
	
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <=50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}
