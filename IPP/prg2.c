#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main() {
	int m, n, p, q, sum=0, i, j, k;
	int a[100][100], b[100][100], c[100][100] = {0};
	
	printf("Enter the dim of 1st matrix : ");
    scanf("%d%d", &m, &n);

	printf("Enter the elements of the first matrix \n");
	
	for(i=0 ; i<m ; i++) for(j=0 ; j<n ; j++) scanf("%d", &a[i][j]);
	
	printf("Enter the dim of 2nd matrix : ");
	scanf("%d%d", &p, &q);
	
	printf("Enter the elements of the second matrix \n");
	
	for(i=0 ; i<p ; i++) for(j=0 ; j<q ; j++) scanf("%d", &b[i][j]);
	
	printf("\n\nMultiplying....\n");
	
	double start = omp_get_wtime();
	
	#pragma omp parallel for private(i, j, k) shared(a, b) 
	for(i=0;i<m;i++) {
		 for(j=0;j<q;j++) {
		     for(k=0;k<p;k++) {
		         sum += a[i][k] * b[k][j];
		     }
		     c[i][j] = sum;
		     sum = 0;
		 }
	}
	
	double end = omp_get_wtime();
	
	printf("\nMultiplied Matrix\n");
	
	for(int i=0;i<m;i++) {
		for(int j=0;j<q;j++)
			printf("%d ", c[i][j]);
		printf("\n");
	}
	
	printf("Exec Time : %f\n", end-start);
	
	return 0;
}
