#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_File fh;
    MPI_Status status;
    char buf[50];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int len = sprintf(buf, "Rank = %d\n", rank);


    MPI_File_open(MPI_COMM_WORLD, "output.txt",
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &fh);


    MPI_Offset offset = (size - rank - 1) * len;


    MPI_File_write_at_all(fh, offset, buf, len, MPI_CHAR, &status);

    MPI_File_close(&fh);
    MPI_Finalize();
    return 0;
}
