#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//etat
#define non_candidat 0
#define candidat 1
#define elu 2
#define perdu 3

//message type
#define ELEC 0
#define LEADER 1



int main(int argc, char** argv) {
    int rank, nb_proc, etat, leaderRank, leaderNumber;
    int succ, msg[4];// type number rank leadernumber

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

    etat = non_candidat;
    leaderRank = -1;
    leaderNumber = -1;
    succ = (rank + 1) % nb_proc;

    srand(time(NULL) + getpid() + rank);
    int number = (rand() % 100) + 1;

    int initiateur = rand() % 2;
    if (initiateur) {
        etat = candidat;
        msg[0] = ELEC;
        msg[1] = number;
        msg[2] = rank;
        MPI_Send(msg, 3, MPI_INT, succ, 0, MPI_COMM_WORLD);
    }



    while (1) {

        MPI_Recv(msg, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // 接收时考虑4个整数
        if (msg[0] == ELEC) {
            if (number > msg[1] || (number == msg[1] && rank < msg[2])) {
                if (etat == non_candidat) {
                    etat = candidat;
                    msg[1] = number;
                    msg[2] = rank;
                    MPI_Send(msg, 3, MPI_INT, succ, 0, MPI_COMM_WORLD);
                }
            } else if (number < msg[1] || (number == msg[1] && rank > msg[2])) {
                etat = perdu;
                MPI_Send(msg, 3, MPI_INT, succ, 0, MPI_COMM_WORLD);
            } else if (number == msg[1] && rank == msg[2]) {
                etat = elu;
                msg[0] = LEADER;
                msg[3] = number;
                leaderRank = rank;
                leaderNumber = number;
                MPI_Send(msg, 4, MPI_INT, succ, 0, MPI_COMM_WORLD);
                break;
            }
        } else if (msg[0] == LEADER) {
            leaderRank = msg[2];
            leaderNumber = msg[3];
            if (rank != leaderRank) {
                MPI_Send(msg, 4, MPI_INT, succ, 0, MPI_COMM_WORLD);
            }
            break;
        }
    }

    printf("Process %d: Leader's rank is %d, leader's number is %d\n", rank, leaderRank, leaderNumber);

    MPI_Finalize();
    return 0;
}
