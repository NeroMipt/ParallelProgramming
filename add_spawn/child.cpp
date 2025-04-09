#include <mpi/mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);
    if (parent == MPI_COMM_NULL) {
        fprintf(stderr, "Дочерний процесс не был создан через MPI_Comm_spawn\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello World от процесса %d из %d\n", rank, size);

    // Круговая пересылка токена
    int token;
    if (rank == 0) {
        token = 100;
        MPI_Send(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Процесс 0 получил токен %d от процесса %d\n", token, size - 1);
    } else {
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        token += 1;
        MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        printf("Процесс %d отправил токен %d процессу %d\n", rank, token, (rank + 1) % size);
    }

    MPI_Comm_disconnect(&parent);
    MPI_Finalize();
    return 0;
}