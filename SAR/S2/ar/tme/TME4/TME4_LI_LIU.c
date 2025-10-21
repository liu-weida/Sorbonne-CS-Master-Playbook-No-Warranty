#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#define REQUEST 1
#define ACK 2
#define FIN 3

#define REQUESTING 1
#define NOT_REQUESTING 2
#define CRITICAL_SECTION 3

#define MAX_CS 5

MPI_Status status;
int nb_proc;
int nback;
int rank;

int etat;
int horloge = 0;
int date_req;
int *file;

int nb_proc_fini = 0;

void Attendre_message(void);

void Request_CS(void) {
    etat = REQUESTING;
    horloge++;
    date_req = horloge;
    nback = 0;

    for (int i = 0; i < nb_proc; ++i) {
        if (i != rank) {
            //printf("%d envoyer request a %d \n",rank,i);
            MPI_Send(&horloge, 1, MPI_INT, i, REQUEST, MPI_COMM_WORLD);
            //sleep(1);
        }
    }

    while (etat != CRITICAL_SECTION) {
        Attendre_message();
    }
}

void Release_CS(void) {
    horloge++;
    for (int i = 0; i < nb_proc; i++) {
        if (file[i] != -1) {
            //printf("%d envoyer release a %d \n",rank,i);
            MPI_Send(&horloge, 1, MPI_INT, file[i], ACK, MPI_COMM_WORLD);
            file[i] = -1;
            //sleep(1);
        }
    }
    etat = NOT_REQUESTING;
}

void Attendre_message(void) {
    int clock;



    MPI_Recv(&clock, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    //printf("%d recu message de %d \n",rank,status.MPI_SOURCE);
    //sleep(1);
    horloge = (clock > horloge ? clock : horloge) + 1;

    switch (status.MPI_TAG) {
        case REQUEST:
            if ((etat == NOT_REQUESTING) ||
                (etat == REQUESTING && (date_req > clock || (date_req == clock && rank > status.MPI_SOURCE)))) {
                horloge++;
                MPI_Send(&horloge, 1, MPI_INT, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            } else {
                for (int i = 0; i < nb_proc; i++) {
                    if (file[i] == -1) {
                        file[i] = status.MPI_SOURCE;
                        break;
                    }
                }
            }
            break;

        case ACK:
            nback++;
            if (nback == nb_proc - 1) {
                etat = CRITICAL_SECTION;
            }
            break;

        case FIN:
            nb_proc_fini++;
            break;

        default:
            exit(1);
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (nb_proc < 2) {
        printf("Nombre de processus incorrect ! (Il doit >= 2)\n");
        MPI_Finalize();
        exit(1);
    }

    file = (int *)malloc(sizeof(int) * nb_proc);
    for (int i = 0; i < nb_proc; ++i) {
        file[i] = -1;
    }


    int cont_CS = 0;



    while (cont_CS < MAX_CS) {
        Request_CS();

        printf("Process %d entering critical section: %d\n", rank, ++cont_CS);
        fflush(stdout);
        sleep(1);
        printf("Process %d leaving critical section\n", rank);
        fflush(stdout);
        sleep(1);

        Release_CS();
    }

    /* traiter la terminaison de l'algorithme */
        horloge ++;
    for (int j = 0; j < nb_proc; ++j) {
        if(j != rank){
            MPI_Send(&horloge,1,MPI_INT,j,FIN,MPI_COMM_WORLD);
        }
    }

    nb_proc_fini ++;

    while (nb_proc_fini < nb_proc){
        Attendre_message();
    }


    /* terminer MPI */
    free(file);
    file = NULL;
    MPI_Finalize();
    return 0;

}



