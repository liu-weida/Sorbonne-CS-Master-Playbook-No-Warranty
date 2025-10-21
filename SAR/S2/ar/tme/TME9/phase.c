#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT    0
#define NB_SITE 6

#define DIAMETRE  5

void simulateur(void) {
    int i;

    /* nb_voisins_in[i] est le nombre de voisins entrants du site i */
    /* nb_voisins_out[i] est le nombre de voisins sortants du site i */
    int nb_voisins_in[NB_SITE+1] = {-1, 2, 1, 1, 2, 1, 1};
    int nb_voisins_out[NB_SITE+1] = {-1, 2, 1, 1, 1, 2, 1};

    int min_local[NB_SITE+1] = {-1, 4, 7, 1, 6, 2, 9};

    /* liste des voisins entrants */
    int voisins_in[NB_SITE+1][2] = {{-1, -1},
                                    {4, 5}, {1, -1}, {1, -1},
                                    {3, 5}, {6, -1}, {2, -1}};

    /* liste des voisins sortants */
    int voisins_out[NB_SITE+1][2] = {{-1, -1},
                                     {2, 3}, {6, -1}, {4, -1},
                                     {1, -1}, {1, 4}, {5,-1}};

    for(i=1; i<=NB_SITE; i++){
        MPI_Send(&nb_voisins_in[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(&nb_voisins_out[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(voisins_in[i], nb_voisins_in[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(voisins_out[i], nb_voisins_out[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
    }
}

//printf("123");
void calcul_min(int rang){
    int nb_voisins_in;
    int nb_voisins_out;
    int min_local;
    int* voisins_in;
    int* voisins_out;
    int* received_counts;
    int sent_counts = 0;
    MPI_Status status;
    MPI_Recv(&nb_voisins_in, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&nb_voisins_out, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    voisins_in = (int*) calloc(nb_voisins_in, sizeof(int));
    voisins_out = (int*) calloc(nb_voisins_out, sizeof(int));
    received_counts = (int*) calloc(nb_voisins_in, sizeof(int));
    MPI_Recv(voisins_in, nb_voisins_in, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(voisins_out, nb_voisins_out, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&min_local, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
/*
    printf("Process %d received data:\n", rang);
    printf(" - Number of incoming neighbors: %d\n", nb_voisins_in);
    printf(" - Number of outgoing neighbors: %d\n", nb_voisins_out);
    printf(" - Local minimum: %d\n", min_local);
    printf(" - Incoming neighbors:");
    for (int i = 0; i < nb_voisins_in; i++) {
        printf(" %d", voisins_in[i]);
    }
    printf("\n - Outgoing neighbors:");
    for (int i = 0; i < nb_voisins_out; i++) {
        printf(" %d", voisins_out[i]);
    }
    printf("\n");
*/
    int done = 0;
    while (!done) {
        if(sent_counts>=DIAMETRE){
            int testend = 1;
            for(int i = 0;i<nb_voisins_in;i++){
                if(received_counts[i] < DIAMETRE){
                    testend = 0;
                }
            }
            if (testend)
            {
                done = 1;
                break;
            }
        }
        int testsent = 1;
        for(int i = 0;i<nb_voisins_in;i++){
            if(sent_counts > received_counts[i]){
                testsent = 0;
                break;
            }
        }
        if (testsent) {
            for (int i = 0; i < nb_voisins_out; i++) {
                MPI_Send(&min_local, 1, MPI_INT, voisins_out[i], 0, MPI_COMM_WORLD);
                printf("Process %d send message: %d to process %d\n", rang, min_local, voisins_out[i]);
            }
            sent_counts++;
            printf("process %d count++ :%d\n",rang,sent_counts);
            continue;
        }

        int message;
        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (message < min_local) {
            min_local = message;
        }
        for (int i = 0; i < nb_voisins_in; i++) {
            if (status.MPI_SOURCE == voisins_in[i]) {
                received_counts[i]++;
                break;
            }
        }

        printf("Process %d received message: %d from process %d\n", rang, message, status.MPI_SOURCE);
        printf("%d Updated local minimum: %d\n",rang, min_local);
    }
    free(voisins_in);
    free(voisins_out);
    free(received_counts);
    printf("Process %d termine, min_local = %d\n",rang,min_local);
}

/******************************************************************************/

int main (int argc, char* argv[]) {
    int nb_proc,rang;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

    if (nb_proc != NB_SITE+1) {
        printf("Nombre de processus incorrect !\n");
        MPI_Finalize();
        exit(2);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    if (rang == 0) {
        simulateur();
    } else {
        calcul_min(rang);
    }

    MPI_Finalize();
    return 0;
}