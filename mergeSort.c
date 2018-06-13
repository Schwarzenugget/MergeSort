#include <stdlib.h>
#include <stdio.h>
 

#define SEED 0

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int* arr, int* arr_aux, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    //int L[n1], R[n2];
    int* L = malloc(sizeof(int)*n1);
    int* R = malloc(sizeof(int)*n2);
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(R); free(L);
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int* arr, int* arr_aux, int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
 
        merge(arr, l, m, r);
    }
}
 
/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int* A, int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

inline void generateRandomValues(int* A, int size)
{
	srand(SEED);
	int i;
	for(i=0; i<size; i++)
		A[i] = rand()%10;
}
 
/* Driver program to test above functions */
int main(int argc, char** argv)
{
    int arr_size = 10;
    int check_merge_sort = 0;
    int n_threads = 1;

    if(argc>1) { arr_size = atoi(argv[1]);}
    if(argc>2) { check_merge_sort = atoi(argv[2]);}
    if(argc>2) { n_threads = atoi(argv[3]);}

    int *arr = (int*)malloc(sizeof(int)*(arr_size));
    int *arr_aux = (int*)malloc(sizeof(int)*(arr_size));
    generateRandomValues(arr, arr_size);
 
    printf("Given array is \n");
    printArray(arr, arr_size);
 
    mergeSort(arr, 0, arr_size - 1);
 
    printf("\nSorted array is \n");
    printArray(arr, arr_size);
    return 0;
}
