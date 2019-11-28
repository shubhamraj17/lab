 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

void sineTable(int sineNum) {
	int i;
	double sines[sineNum];
	double pi=3.1415926;
	for(i=0;i<sineNum;i++) sines[i]=sin((double)(i)*pi/180);
	
	printf("\n");
	for(int i=0;i<sineNum;i++)
	{
		printf("%lf ",sines[i]);
	}
}

void primeTable(int primeNum) {
	int i, j, p;
	
	int prime, primes[primeNum];
	
	i = 2; p = 0;
	
	while(p < primeNum) {
		prime = 1;
		for(j=2 ; j<i ; j++)
			if(i % j == 0 ) {
				prime = 0;
				break;
			}
		
		if(prime) primes[p++] = i;
		
		i++;
	}
	
	for(int i=0;i<primeNum;i++) printf("%d ", primes[i]);
	
}  

int main() {
	int n;
	
	printf("Enter the number of values : ");
	scanf("%d", &n);
	
	int a[n];
	double b[n];
	
	for(int k=1 ; k<=8 ; k = pow(2,k)) {
		double start = omp_get_wtime();
		
		#pragma omp parallel sections num_threads(k)
		{
			#pragma omp section
				primeTable(n);
			
			#pragma omp section
				sineTable(n);
		}
		
		double end = omp_get_wtime();
		
		printf("Exec Time : %f\n", end - start);
	}
	
	return 0;
}
