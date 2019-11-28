#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int *a, int l, int m, int r) {
	int n1 = m-l+1, n2 = r-m, i, j, k;
	int L[n1], R[n2];
	
	for(i=0;i<n1;i++) L[i] = a[l+i];
	for(j=0;j<n2;j++) R[j] = a[m+1+j];
	
	i=0;j=0;k=l;
	
	while(i < n1 && j < n2) a[k++] = (L[i] < R[j]) ? L[i++] : R[j++];
	
	while(i < n1) a[k++] = L[i++];
	while(j < n2) a[k++] = R[j++];
}

void mergesort(int *a, int l, int r) {
	if(l < r) {
		int mid = l + (r-l)/2;
		
		#pragma omp task firstprivate(a, l, mid)
		mergesort(a, l, mid);
		
		#pragma omp task firstprivate(a, mid, r)		
		mergesort(a, mid+1, r);
		
		#pragma omp taskwait
		merge(a, l, mid, r);
	}
}

int main() {
	int n, a[100];
	
	printf("Enter the number of elements : ");
	scanf("%d", &n);
	
	printf("Enter the elements : ");
	for(int i=0;i<n;i++) scanf("%d", &a[i]);
	
	double time = omp_get_wtime();
	
	#pragma omp parallel num_threads(4)
		#pragma omp single
			mergesort(a, 0, n-1);
	
	time = omp_get_wtime() - time;
	
	printf("Sorted Array : ");
	for(int i=0;i<n;i++) printf("%d ", a[i]);
	
	printf("\nExec time : %lf\n", time);
	
	return 0;
}
