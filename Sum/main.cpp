
#include <mpi/mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment. The two arguments to MPI Init are not
    // currently used by MPI implementations, but are there in case future
    // implementations might need the arguments.

    MPI_Init(&argc, &argv);

    int N;
    int elements_per_process;
    int n_elements_recieved;
    // np -> no. of processes
    // pid -> process id

    MPI_Status status;
    if(argc > 1)
    {
        N = atoi(argv[1]);
    }

//
//    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) {
        int index, i;
        elements_per_process = N / world_size;

        // check if more than 1 processes are run
        if (world_size > 1) {
            // distributes the portion of array
            // to child processes to calculate
            // their partial sums
            for (i = 1; i < world_size - 1; i++) {
                index = i * elements_per_process;

                MPI_Send(&elements_per_process,
                         1, MPI_INT, i, 0,
                         MPI_COMM_WORLD);
                MPI_Send(&index,
                         1,
                         MPI_INT, i, 0,
                         MPI_COMM_WORLD);
            }

            // last process adds remaining elements
            index = i * elements_per_process;
            int elements_left = N - index;

            MPI_Send(&elements_left,
                     1, MPI_INT,
                     i, 0,
                     MPI_COMM_WORLD);
            MPI_Send(&index,
                     1,
                     MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }

        // master process add its own sub array
        double sum = 0;
        //std::cout << elements_per_process << std::endl;
        for (i = 1; i < elements_per_process; i++) {
            double s = i;
            sum += 1 / s;
        }

        // collects partial sums from other processes
        double tmp = 0;
        for (i = 1; i < world_size; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE,
                     i, 0,
                     MPI_COMM_WORLD,
                     &status);
            int sender = status.MPI_SOURCE;

            sum += tmp;
        }

        double Nd = N;
        sum += 1/Nd;

        // prints the final sum of array
        printf("Sum of array is : %f\n", sum);
    }
        // slave processes
    else {
        MPI_Recv(&n_elements_recieved,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);

        // stores the received array segment
        // in local array a2
        int a2;
        MPI_Recv(&a2, 1,
                 MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);

        // calculates its partial sum
        double partial_sum = 0;
        for (int i = 0; i < n_elements_recieved; i++) {
            double s = a2 + i;
            partial_sum += 1 / s;

        }
        //std::cout << partial_sum << std::endl;
        // sends the partial sum to the root process
        MPI_Send(&partial_sum, 1, MPI_DOUBLE,
                 0, 0, MPI_COMM_WORLD);
    }

   MPI_Finalize();
}