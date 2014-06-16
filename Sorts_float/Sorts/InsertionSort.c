
#include "InsertionSort.h"

void InsertionSort(float* arr, long arrSize) {
	float temp;
	long i, j;
	for(i = 1; i < arrSize; ++i) 
	{
		j = i;
		while(j >= 1 && arr[j - 1] > arr[j]) 
		{
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;       
			j--;
		}          
	}
}