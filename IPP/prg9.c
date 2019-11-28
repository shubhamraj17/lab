#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECLEN 100

int main(int argc, char* argv[]) {
	
	int num_procs, myid, len = VECLEN;
	double mysum = 0, allsum = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	if(myid == 0) printf("Process Started..\n");

	double a[len], b[len];
	
	for(int i=0;i<len;i++) {
		a[i] = 1;
		b[i] = a[i];
	}
	
	for(int i=0;i<len;i++) mysum += a[i] * b[i];
	
	printf("Partial Sum : %f ID : %d\n", mysum, myid);
	
	MPI_Reduce(&mysum, &allsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(myid==0) printf("Global Sum : %f\n", allsum);
	
	return 0;
}
