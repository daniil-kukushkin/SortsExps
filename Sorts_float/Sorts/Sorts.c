#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include <float.h>
#include "InsertionSort.h"
#include "MergeSort.h"

int floatCompare(const void* a,const void* b)
{
	if (*(float*)a > *(float*)b) 
		return 1;
	else 
		if (*(float*)a < *(float*)b) 
			return -1;
		else return 0;
}

int (*floatComparePtr)(const void* a, const void* b) = floatCompare;

int isCorrect(float* sortArr, int size, int seed)
{
	float *arr;
	int i;
	int correct = 1;
	
	arr = (float*) malloc (sizeof(float) * size);
	srand(seed);
	for(i=0; i<size; i++)
		arr[i] = ((rand() - RAND_MAX/2)/(float)RAND_MAX)*(float)FLT_MAX;

	qsort((void*)arr, size, sizeof(arr[0]), floatComparePtr );

	for(i=0; i<size; i++)
		if(arr[i] != sortArr[i])
		{
			correct = 0;
			break;
		}

	return correct;
	free((void*)arr);
}

float selectAndSort(float* arr, int countData, int sortNum, int seed)
{
	int i;
	LARGE_INTEGER sQP, fQP;
	LARGE_INTEGER start, finish, freq;

	srand(seed);
	for(i=0; i<countData; i++)
		arr[i] = ((rand() - RAND_MAX/2)/(float)RAND_MAX)*(float)FLT_MAX;

	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter( &start );

	switch(sortNum)
	{
	case 1:
		InsertionSort(arr, countData);
		break;
	case 2:
		MergeSort(arr, countData);
		break;
	case 3:
		qsort((void*)arr, countData, sizeof(arr[0]), floatComparePtr );
		break;
	default:
		return 1;
	}
	
	QueryPerformanceCounter( &finish );
	return (finish.QuadPart - start.QuadPart) / (float)freq.QuadPart; // time - время в секундах	
}

int main(int argc, char* argv[])
{
	float timeInSec;
	int arg1, arg2, arg3;
	int arrSize, seed, sortNum;
	float* array = NULL;

	if(argc < 4)
	{
		printf("Error!\n");
		printf("Usage: Sorts.exe <arg1> <arg2> <arg3>\n");
		printf("  <arg1> - data count\n");
		printf("  <arg2> - random seed\n");
		printf("  <arg3> - sort number:\n");
		printf("		1 - insertion sort\n");
		printf("		2 - merge sort\n");
		printf("		3 - std quick sort\n");
		return 0;
	}

	arg1 = atoi(argv[1]);
	arg2 = atoi(argv[2]);
	arg3 = atoi(argv[3]);

	arrSize = arg1;// = 20;
	seed = arg2;// = 124;
	sortNum = arg3;// = 1;
	if(arg1 <= 0)
		return 1;
	
	array = (float*) malloc (sizeof(float) * arrSize);
	timeInSec = selectAndSort(array, arrSize, sortNum, seed);
	printf("%e %e\n", timeInSec, array[arg1/13]);

	// Проверка на корректность
	/*
	if( isCorrect(array, arrSize, seed) )
		printf("correct \n");
	else
		printf("uncorrect \n");
	*/
	
	free((void*)array);
	getchar();
	return 0;
}