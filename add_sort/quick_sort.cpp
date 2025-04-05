#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);
void quickSort(int *array, int low, int high)
{
    int i = low;
    int j = high;
    int pivot = array[(i + j) / 2];
    int temp;

    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    if (j > low)
        quickSort(array, low, j);
    if (i < high)
        quickSort(array, i, high);
}

int main(int argc, char** argv) {

    /********** Create and populate the array **********/
    int n = atoi(argv[1]);
    int *original_array = (int*)malloc(n * sizeof(int));

    int c;
    srand(time(NULL));

    for(c = 0; c < n; c++) {

        original_array[c] = rand() % n;

    }



    /********** Initialize MPI **********/
    int world_rank;
    int world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double start_sort = MPI_Wtime();

    /********** Divide the array in equal-sized chunks **********/
    int size = n/world_size;

    /********** Send each subarray to each process **********/
    int *sub_array = (int*)malloc(size * sizeof(int));
    MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

    /********** Perform the mergesort on each process **********/
    int *tmp_array = (int*)malloc(size * sizeof(int));
//    mergeSort(sub_array, tmp_array, 0, (size - 1));
    quickSort(sub_array, 0, (size-1));

    /********** Gather the sorted subarrays into one **********/
    int *sorted = NULL;
    if(world_rank == 0) {

        sorted = (int*)malloc(n * sizeof(int));

    }

    MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

    /********** Make the final mergeSort call **********/
    if(world_rank == 0) {

        int *other_array = (int*)malloc(n * sizeof(int));
        mergeSort(sorted, other_array, 0, (n - 1));

        /********** Display the sorted array **********/
        double end_sort = MPI_Wtime();
        printf("Elapsed time: %lf", end_sort - start_sort);
        /********** Clean up root **********/
        free(sorted);
        free(other_array);

    }

    /********** Clean up rest **********/
    free(original_array);
    free(sub_array);
    free(tmp_array);

    /********** Finalize MPI **********/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}

/********** Merge Function **********/
void merge(int *a, int *b, int l, int m, int r) {

    int h, i, j, k;
    h = l;
    i = l;
    j = m + 1;

    while((h <= m) && (j <= r)) {

        if(a[h] <= a[j]) {

            b[i] = a[h];
            h++;

        }

        else {

            b[i] = a[j];
            j++;

        }

        i++;

    }

    if(m < h) {

        for(k = j; k <= r; k++) {

            b[i] = a[k];
            i++;

        }

    }

    else {

        for(k = h; k <= m; k++) {

            b[i] = a[k];
            i++;

        }

    }

    for(k = l; k <= r; k++) {

        a[k] = b[k];

    }

}

/********** Recursive Merge Function **********/
void mergeSort(int *a, int *b, int l, int r) {

    int m;

    if(l < r) {

        m = (l + r)/2;

        mergeSort(a, b, l, m);
        mergeSort(a, b, (m + 1), r);
        merge(a, b, l, m, r);

    }

}
