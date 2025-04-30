#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TESTS 100000
#define MESSAGE_SIZE 1  // 1 байт для измерения чистой задержки

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    if (world_size < 2) {
        fprintf(stderr, "Требуется как минимум 2 процесса для этого теста\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    char message[MESSAGE_SIZE];
    double total_time = 0.0;
    
    for (int i = 0; i < NUM_TESTS; i++) {
        if (world_rank == 0) {

            double start_time = MPI_Wtime();
            
            MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            double end_time = MPI_Wtime();
            total_time += (end_time - start_time);
        } else if (world_rank == 1) {

            MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    if (world_rank == 0) {
        double avg_latency = (total_time / (2 * NUM_TESTS)) * 1e6; // в микросекундах
        printf("Средняя задержка передачи: %.3f мкс\n", avg_latency);
    }
    
    MPI_Finalize();
    return 0;
}