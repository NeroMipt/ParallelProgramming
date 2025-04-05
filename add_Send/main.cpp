//
// Created by egor on 3/9/25.
//
//
// Created by egor on 3/9/25.
//



#include <mpi/mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment. The two arguments to MPI Init are not
    // currently used by MPI implementations, but are there in case future
    // implementations might need the arguments.

    MPI_Status status;
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0)
    {

        int readyfg = 0;
        MPI_Send(&readyfg,
                 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD);
        double start_time = MPI_Wtime();
        std::cout << "start exp 0\n";
        MPI_Send(&readyfg,
                 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD);
        double end_time = MPI_Wtime();
        std::cout << (end_time - start_time) * 1e5;
    }
    if(world_rank == 1)
    {
        int ie;
        //sleep(2);
        MPI_Recv(&ie,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);
        std::cout << "start exp 1\n";
        double start_sleep = MPI_Wtime();
        double sleeping = 0.0;
        while(sleeping < 2.0)
        {
            sleeping = MPI_Wtime() - start_sleep;
        }
        std::cout << "end sleep 1\n";
        MPI_Recv(&ie,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);


    }






    // Finalize the MPI environment. No more MPI calls can be made after this
    MPI_Finalize();
}