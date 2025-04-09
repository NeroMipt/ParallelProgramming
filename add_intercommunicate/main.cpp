#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, N;
    double local_sum = 0.0;
    double *total_sum = NULL;
    MPI_Win win;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (argc != 2) {
            printf("Usage: %s N\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        N = atoi(argv[1]);
    }


    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);


    int chunk = N / size;
    int start = rank * chunk + 1;
    int end = (rank == size - 1) ? N : start + chunk - 1;


    for (int i = start; i <= end; i++) {
        local_sum += 1.0 / i;
    }


    if (rank == 0) {
        MPI_Win_allocate(sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &total_sum, &win);
        *total_sum = 0.0;
    } else {
        MPI_Win_allocate(0, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &total_sum, &win);
    }

    MPI_Win_fence(0, win);


    MPI_Accumulate(&local_sum, 1, MPI_DOUBLE, 0, 0, 1, MPI_DOUBLE, MPI_SUM, win);

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("Sum = %f\n", *total_sum);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}