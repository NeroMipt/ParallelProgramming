#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

int main() {
// Obtain an intercommunicator to the parent MPI job
    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);
// Get child rank
    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
// Check if this process is a spawned one and if so get parent CPU rank
    if (parent != MPI_COMM_NULL) {
        int parent_id;
        MPI_Bcast(&parent_id, 1, MPI_INT, 0, parent);
        std::cout << "Child " << myid << " of Parent " << parent_id << std::endl;
    }
}