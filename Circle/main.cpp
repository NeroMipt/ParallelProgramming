//
// Created by egor on 3/9/25.
//



#include <mpi/mpi.h>
#include <stdio.h>

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
        int i = 0;
        int ie = 0;
        std::cout << "rank: 0, var: " << i << std::endl;
        MPI_Send(&i,
                 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD);
        MPI_Recv(&ie,
                 1, MPI_INT, world_size - 1, 0,
                 MPI_COMM_WORLD,
                 &status);
        std::cout << "res: " << ie << std::endl;
    }else
    {
        int dest = 0;
        int is = 0;
        MPI_Recv(&is,
                 1, MPI_INT, world_rank - 1, 0,
                 MPI_COMM_WORLD,
                 &status);
        is += 1;
        std::cout << "rank: " << world_rank << " var: " << is << std::endl;
        if(world_rank == world_size - 1)
            dest = 0;
        else dest = world_rank + 1;

        MPI_Send(&is,
                 1, MPI_INT, dest, 0,
                 MPI_COMM_WORLD);
    }



    // Finalize the MPI environment. No more MPI calls can be made after this
    MPI_Finalize();
}