#include <iostream>
#include <fstream>
#include <cmath>
#include <mpi/mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double T = 1.0;
    const double X = 1.0;

    const double tau = 0.1;
    const double h = 0.2;

    const int K = int(T / tau);
    const int M = int(X / h);


    int local_M = M / size;
    int start_j = rank * local_M + 1;
    int end_j = (rank == size - 1) ? M : start_j + local_M - 1;

    double Cell[K+1][M+2];


    for (int i = 0; i <= K; i++)
        for (int j = 0; j <= M+1; j++)
            Cell[i][j] = 0.0;

    double start_sort = MPI_Wtime();
    for (int j = start_j; j <= end_j; j++) {
        Cell[0][j] = std::cos(M_PI * h * j);
    }
    for (int i = 0; i < K; i++) {
        if (rank == 0) {
            Cell[i][1] = std::exp(-1 * i * tau);
        }
    }


    for (int i = 0; i < K; i++) {

        if (rank != 0) {
            MPI_Send(&Cell[i][start_j], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&Cell[i][start_j - 1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank != size - 1) {
            MPI_Recv(&Cell[i][end_j + 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&Cell[i][end_j], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        }

        for (int j = start_j; j <= end_j; j++) {
            Cell[i+1][j] = Cell[i][j] + tau * (
                    i * tau + h * (j - 1) +
                    tau * (Cell[i][j+1] - 2 * Cell[i][j] + Cell[i][j-1]) / (2 * h * h) -
                    (Cell[i][j+1] - Cell[i][j-1]) / (2 * h)
            );
        }
    }


    if (rank == 0) {
        std::ofstream out("data_par");
        for (int i = 0; i < K; i++) {

            for (int r = 1; r < size; r++) {
                int recv_start = r * local_M + 1;
                int recv_end = (r == size - 1) ? M : recv_start + local_M - 1;
                MPI_Recv(&Cell[i][recv_start], recv_end - recv_start + 1, MPI_DOUBLE, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            double end_sort = MPI_Wtime();
            printf("Elapsed time: %lf \n", end_sort - start_sort);
            for (int j = 1; j <= M; j++) {
                out << Cell[i][j] << "\t";
//                std::cout << Cell[i][j] << " ";
            }
            out << "\n";
//            std::cout << std::endl;
        }
    } else {
        for (int i = 0; i < K; i++) {
            MPI_Send(&Cell[i][start_j], end_j - start_j + 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
