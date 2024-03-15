/*Programmer: Joshua Jaggernauth
Date: 03/15/2024
Project: Lab Assignment 8 (Merge Sort)
*/
//This code implements the merge sort algorithm in C for sorting integer arrays read from input files. 
//It tracks extra memory allocation during sorting, using custom memory management functions. 
//The sorted arrays and runtime statistics are printed for each input file processed.

#include <time.h>

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

//Global variable to track extra memory allocated
int extraMemoryAllocated;

//Function to allocate memory with tracking
void * Alloc(size_t sz) {
  extraMemoryAllocated += sz;
  size_t * ret = malloc(sizeof(size_t) + sz);
  * ret = sz;
  printf("Extra memory allocated, size: %ld\n", sz);
  return & ret[1];
}

//Function to deallocate memory with tracking
void DeAlloc(void * ptr) {
  size_t * pSz = (size_t * ) ptr - 1;
  extraMemoryAllocated -= * pSz;
  printf("Extra memory deallocated, size: %ld\n", * pSz);
  free((size_t * ) ptr - 1);
}

//Function to get the size of allocated memory
size_t Size(void * ptr) {
  return ((size_t * ) ptr)[-1];
}

//Merge function for merge sort
void merge(int * arr, int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  //Create temporary arrays
  int * L = (int * ) Alloc(n1 * sizeof(int));
  int * R = (int * ) Alloc(n2 * sizeof(int));

  //Copy data to temp arrays L[] and R[]
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  //Merge the temp arrays back into arr[l..r]
  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  //Copy the remaining elements of L[], if any
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  //Copy the remaining elements of R[], if any
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  //Free temporary arrays
  DeAlloc(L);
  DeAlloc(R);
}

//Recursive function for merge sort
void mergeSort(int * arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    //Sort first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    //Merge the sorted halves
    merge(arr, left, mid, right);
  }
}

//Function to parse input file to an integer array
int parseData(char * inputFileName, int ** ppData) {
  FILE * inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n, * data;
  * ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", & dataSz);
    * ppData = (int * ) malloc(sizeof(int) * dataSz);
    if ( * ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      fscanf(inFile, "%d ", & n);
      data = * ppData + i;
      * data = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

//Function to print first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");
  for (i = 0; i < 100; ++i) {
    printf("%d ", pData[i]);
  }
  printf("\n\t");

  for (i = sz; i < dataSz; ++i) {
    printf("%d ", pData[i]);
  }
  printf("\n\n");
}

int main(void) {
  clock_t start, end;
  int i;
  double cpu_time_used;
  char * fileNames[] = {
    "input1.txt",
    "input2.txt",
    "input3.txt",
    "input4.txt"
  };

  for (i = 0; i < 4; ++i) {
    int * pDataSrc, * pDataCopy;
    int dataSz = parseData(fileNames[i], & pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int * ) malloc(sizeof(int) * dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n", dataSz);
    printf("---------------------------\n");

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    free(pDataCopy);
    free(pDataSrc);
  }

}