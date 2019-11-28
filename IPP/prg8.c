#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>

void ring_io ( int p, int id );

int main ( int argc, char *argv[]) {
  int error, id, p;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &id);
  MPI_Comm_size ( MPI_COMM_WORLD, &p);

  if (id == 0) printf ( "The number of processes is %d\n", p);
  
  ring_io (p, id);

  MPI_Finalize();
  
  return 0;
}

void ring_io ( int p, int id ) {
  int dest, i, j, n, source, n_test_num = 5, test, test_num = 10;
  int n_test[5] = { 100, 1000, 10000, 100000, 1000000 };
  MPI_Status status;
  double tave, tmax, tmin, wtime;
  double *x;

  if ( id == 0 ) printf ( "         N           T min           T ave           T max\n\n" );
	/* Choose message size */
  for ( i = 0; i < n_test_num; i++ ) {    
    n = n_test[i];

    x = (double *) malloc (n*sizeof(double));
	/*
	  Process 0 sends very first message, 
	  then waits to receive the "echo" that has gone around the world.
	*/
    if ( id == 0 ) {
    	dest = 1;
    	source = p - 1;

   	tave = 0;
      tmin = LONG_MAX;
      tmax = 0.0;
      
      for ( test = 1; test <= test_num; test++ ) {
		/* Just in case, set the entries of X in a way that identifies which iteration of the test is being carried out */
        for (j = 0; j < n; j++) x[j] = (double) (test + j);
		  
        wtime = MPI_Wtime();
        MPI_Send (x, n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        MPI_Recv (x, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
        wtime = MPI_Wtime() - wtime;
        
        tave = tave + wtime;
        
        if (wtime < tmin) tmin = wtime;
        if (tmax < wtime) tmax = wtime;
      }

      tave = tave / ( double ) ( test_num );

      printf ( "  %8d  %14.6g  %14.6g  %14.6g\n", n, tmin, tave, tmax );
    } else {						/* Worker ID must receive first from ID-1, then send to ID+1 */
      dest = (id + 1) % p;
      source = id - 1;
      for ( test = 1; test <= test_num; test++ ) {
        MPI_Recv ( x, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
        MPI_Send ( x, n, MPI_DOUBLE, dest,   0, MPI_COMM_WORLD);
      }
    }
    free ( x );
  }
}

//mpicc 8.c
//mpirun -np 4 ./a.out
