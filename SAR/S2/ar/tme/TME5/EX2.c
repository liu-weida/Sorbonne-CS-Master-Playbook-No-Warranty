#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_CHILDREN 2

//tap
#define wakeup 0
#define ident 1

struct LeaderInfo {
    int value;
    int rank;
};

int rank, size;
int leader_value;
int children[MAX_CHILDREN] = {-1, -1};
int parent;
int initiateur;


void InitializeTree(){
    srand(time(NULL) + rank);
    leader_value = rand() % 100;
    parent = rank > 0 ? (rank - 1) / MAX_CHILDREN : -1;
    for (int i = 0; i < MAX_CHILDREN; ++i) {
        int child = rank * MAX_CHILDREN + i + 1;
        if (child < size) {
            children[i] = child;
        }
    }

    printf("Process %d, Leader Value: %d, Parent: %d, Children: [%d, %d]\n",
           rank, leader_value, parent, children[0], children[1]);
    MPI_Barrier(MPI_COMM_WORLD);

    sleep(0.5);
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);



    InitializeTree();

    MPI_Status status;
    struct LeaderInfo local_leader_info = {leader_value, rank};

    srand(getpid());
    initiateur = rand()%2;

    if (initiateur) {
        if (children[0] != -1 || children[1] != -1) {
            if (parent != -1) {
                MPI_Send(NULL, 0, MPI_INT, parent, wakeup, MPI_COMM_WORLD);
            }
            for (int i = 0; i < MAX_CHILDREN; ++i) {
                if (children[i] != -1) {
                    MPI_Send(NULL, 0, MPI_INT, children[i], wakeup, MPI_COMM_WORLD);
                }
            }
        } else {
            MPI_Send(&local_leader_info, 2, MPI_INT, parent, ident, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, wakeup, MPI_COMM_WORLD, &status);
        if (children[0] == -1 && children[1] == -1) { //feuille
            MPI_Send(&local_leader_info, 2, MPI_INT, parent, ident, MPI_COMM_WORLD);

        } else {  //pas feuille
            for (int i = 0; i < MAX_CHILDREN; ++i) {
                if (children[i] != -1 && children[i] != status.MPI_SOURCE) {
                    MPI_Send(NULL, 0, MPI_INT, children[i], wakeup, MPI_COMM_WORLD);
                }
            }
            if (parent != -1 && parent != status.MPI_SOURCE) {
                MPI_Send(NULL, 0, MPI_INT, parent, wakeup, MPI_COMM_WORLD);
            }
        }
    }


    if (children[0] != -1 || children[1] != -1) { // pas feuille
        struct LeaderInfo received_info;
        for (int i = 0; i < MAX_CHILDREN; ++i) {
            if (children[i] != -1) {
                MPI_Recv(&received_info, 2, MPI_INT, children[i], ident, MPI_COMM_WORLD, &status);
                if (received_info.value < local_leader_info.value) {
                    local_leader_info = received_info;
                }
            }
        }

        if (parent != -1) {
            MPI_Send(&local_leader_info, 2, MPI_INT, parent, ident, MPI_COMM_WORLD);
        } else {
            printf("Process 0 : The winner is Process %d with the leader value: %d\n", local_leader_info.rank, local_leader_info.value);

            for (int i = 1; i < size; ++i) {
                MPI_Send(&local_leader_info, 2, MPI_INT, i, ident, MPI_COMM_WORLD);
            }
        }
    }

    if (rank != 0) {
        struct LeaderInfo winner_info;
        MPI_Recv(&winner_info, 2, MPI_INT, 0, ident, MPI_COMM_WORLD, &status);
        printf("Process %d : Leader's rank is %d, leader's number is %d\n", rank, winner_info.rank, winner_info.value);
    }

    MPI_Finalize();
    return 0;
}
