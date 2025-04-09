#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Comm intercomm;
    int errors[4];


    MPI_Comm_spawn("./worker", MPI_ARGV_NULL, 4, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, errors);

    for (int i = 0; i < 4; i++) {
        if (errors[i] != MPI_SUCCESS) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }



    MPI_Comm_disconnect(&intercomm);
    MPI_Finalize();
    return 0;
}