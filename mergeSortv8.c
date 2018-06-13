#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SEED 0
#define MIN_BLOCK_SIZE 30    //SIZE 1 --> R - L = 1 --> 2 array positions

inline void swapValues(int* a, int* b) { int c=*a; *a=*b; *b=c;}

inline void swapPointers(int** a, int** b) { int* c=*a; *a=*b; *b=c;}

/*inline void manualCopy(int* a, int* b, int l, int r){ //UTILTZADA PER DEBUGAR
	int i;
	for(i=0; i<r+1-l; i++)
		b[l+i] = a[l+i]; 
}*/

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]

inline void bubbleSort(int* arr, int l, int r){
   
   int i,j,aux;
   for(i=1; i<(r-l+1); i++)

     for(j=0; j<(r-l+1)-i; j++)

	  if(arr[l+j]>arr[l+j+1])
	     swapValues(&arr[l+j],&arr[l+j+1]);
}

void __attribute__ ((noinline)) insertionSort(int* arr, int l, int r){
	
	int j, temp;	
	for (int i = 0; i < (r-l+1); i++){
		j = i;
	
		while (j > 0 && arr[l+j] < arr[l+j-1]){
			  temp = arr[l+j];
			  arr[l+j] = arr[l+j-1];
			  arr[l+j-1] = temp;
			  j--;
		}
	}
}

void __attribute__ ((noinline)) merge(int* arr, int* arr_aux, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* Point to array's halves */
    int* L = arr_aux+l;
    int* R = arr_aux+m+1;

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        /*if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }*/
	arr[k] = (L[i] <= R[j]) ? L[i++] : R[j++];
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    if(i < n1)
	memcpy(arr+k, L+i, (n1-i)*sizeof(int));

    /* Copy the remaining elements of R[], if there
       are any */
    if(j < n2)
	memcpy(arr+k, R+j, (n2-j)*sizeof(int));
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int* arr, int* arr_aux, int l, int r, int depth)
{
    
    if (r - l > MIN_BLOCK_SIZE)
    {
	swapPointers(&arr, &arr_aux);
    	depth++;

        int m = l+(r-l)/2;

        mergeSort(arr, arr_aux, l, m, depth);

        mergeSort(arr, arr_aux, m+1, r, depth);

        merge(arr, arr_aux, l, m, r);

    }else{
	if(depth%2==0){
		insertionSort(arr, l, r);
		memcpy (arr_aux+l, arr+l, sizeof(int)*(r-l+1));
	}else{
		insertionSort(arr_aux, l, r);
		memcpy (arr+l, arr_aux+l, sizeof(int)*(r-l+1));
	}
	
     }
}
void mergeSortParallel(int* arr, int* arr_aux, int l, int r, int depth, int n_threads)
{
    n_threads/=2;
    if (r - l > MIN_BLOCK_SIZE)
    {
	if(n_threads<=0){ //if(r-l<1000){
	mergeSort(arr, arr_aux, l, r, depth);	
	return;
	}
	swapPointers(&arr, &arr_aux);
    	depth++;

        int m = l+(r-l)/2;

	#pragma omp task
        mergeSortParallel(arr, arr_aux, l, m, depth, n_threads);
	#pragma omp task
        mergeSortParallel(arr, arr_aux, m+1, r, depth, n_threads);
	#pragma omp taskwait
        merge(arr, arr_aux, l, m, r);
    }else{
	if(depth%2==0){
		insertionSort(arr, l, r);
		memcpy (arr_aux+l, arr+l, sizeof(int)*(r-l+1));
	}else{
		insertionSort(arr_aux, l, r);
		memcpy (arr+l, arr_aux+l, sizeof(int)*(r-l+1));
	}
	
     }
}

/* UTILITY FUNCTIONS */
void printArray(int* A, int size)
{
    int i;
    int i_jump = 1;
    if(size>10){
	i_jump = size/10;
    	i_jump = (i_jump == 1) ? 2 : i_jump; //Avoid printing N positions when 10<size<20
    }
    for (i=0; i < size; i+=i_jump)
        printf("[%d] %d\n", i, A[i]);

    if(i_jump>1 && size%2==0)
	printf("[%d] %d\n", size-1, A[size-1]);
}

inline void generateRandomValues(int* A, int size)
{
	srand(SEED);
	int i;
	for(i=0; i<size; i++)
		A[i] = rand();
}

void checkOrder (int *arr, int size){
	int i = 1, well_sorted = 1;
	while(well_sorted && i<size){
		well_sorted = (arr[i-1]<=arr[i]);
		i+=1;
	}
	printf("Well Sorted? %d\n", well_sorted);
}

/* Driver program to test above functions */
int main(int argc, char** argv)
{
    int arr_size = 100000000;
    int check_merge_sort = 1;
    int n_threads = 1;
    int printing_array = 0;

    if(argc>1) { arr_size = atoi(argv[1]);} 	        printf("[1] Array Size = %d\n", arr_size);
    if(argc>2) { check_merge_sort = atoi(argv[2]);}	printf("[2] Checking? = %d\n", check_merge_sort);
    if(argc>3) { n_threads = atoi(argv[3]);}		printf("[3] N_THREADS = %d\n", n_threads);
    if(argc>4) { printing_array = atoi(argv[4]);}	printf("[4] Printing array? = %d\n  (up to 11 (non consecutive) positions)\n\n", printing_array);


    int *arr = (int*)malloc(sizeof(int)*(arr_size));
    int *arr_aux = (int*)malloc(sizeof(int)*(arr_size));
    generateRandomValues(arr, arr_size);

    if(printing_array){
	    printf("Given array is \n");
	    printArray(arr, arr_size);
    }
    
    if(n_threads <2){ //Avoid pragma overhead
	mergeSort(arr, arr_aux, 0, arr_size - 1, 0);
    }else{
	    #pragma omp parallel num_threads(n_threads)
	    #pragma omp single
	    mergeSortParallel(arr, arr_aux, 0, arr_size - 1, 0, n_threads);
    }
    swapPointers(&arr, &arr_aux);


    if(printing_array){
	printf("\nSorted array is \n");
	printArray(arr, arr_size);
    }
    if(check_merge_sort)
	checkOrder(arr, arr_size);

    if(printing_array){
	printf("\nAux array is \n");
	printArray(arr_aux, arr_size);
    }
    if(check_merge_sort)
	checkOrder(arr_aux, arr_size);
    
    return 0;
}
