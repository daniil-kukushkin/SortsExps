
#include "MergeSort.h"

int size;
/*
void make_merge(float* a, long l, long mid, long r) 
{
	long step;
	float* tmp = (float*) malloc (sizeof(float) * size);
	long i = l;
	long j = mid + 1;
	long count = 0;
	for(step = 0; step < r - l + 1; step++){
		if((j > r) || ((i <= mid) && (a[i] < a[j]))) {
			count += j - (mid + 1);
			tmp[step] = a[i];
			i++;
		}
		else{
			tmp[step] = a[j];
			j++;
		}
	}  
	for(step=0;step<r-l+1;++step){
		a[l+step]=tmp[step];
	}
	free(tmp);
}

int merge_sort(float* a, long l, long r) 
{
	long mid;
	if(l == r) 
		return 0;
	mid =(l+r)/2;
	merge_sort(a,l,mid);
	merge_sort(a,mid+1,r);
	make_merge(a,l,mid,r);
}*/

// Сортировка слиянием без рекурсии
void mergeSort (float* a, long num)
{
	int rght, wid, rend;
	int i,j,m,t,k, left;
	float* b = (float*) malloc (sizeof(float) * size);

	for (k=1; k < num; k *= 2 ) {       
		for (left=0; left+k < num; left += k*2 ) {
			rght = left + k;        
			rend = rght + k;
			if (rend > num) rend = num; 
			m = left; i = left; j = rght; 
			while (i < rght && j < rend) { 
				if (a[i] <= a[j]) {         
					b[m] = a[i]; i++;
				} else {
					b[m] = a[j]; j++;
				}
				m++;
			}
			while (i < rght) { 
				b[m]=a[i]; 
				i++; m++;
			}
			while (j < rend) { 
				b[m]=a[j]; 
				j++; m++;
			}
			for (m=left; m < rend; m++) { 
				a[m] = b[m]; 
			}
		}
	}
}

void MergeSort(float* arr, long arrSize)
{
	size = arrSize;
	mergeSort(arr, size); // без рекурсии
	//merge_sort(arr, 0, size-1); // с рекурсией
}