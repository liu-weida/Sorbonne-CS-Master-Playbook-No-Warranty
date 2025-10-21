#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAGINIT    0
#define NB_SITE 6
#define TAGREQ 1
#define TAGMIN 2
#define TAGANN 3

void simulateur(void) {
    int i, initiateur;

    printf("\nAlgorithme de l'echo\n");

    int nb_voisins[NB_SITE+1] = {-1, 3, 3, 2, 3, 5, 2};
    int min_local[NB_SITE+1] = {-1, 12, 11, 8, 14, 5, 17};

    int voisins[NB_SITE+1][5] = {{-1, -1, -1, -1, -1},
                                 {2, 5, 3, -1, -1},
                                 {4, 1, 5, -1, -1},
                                 {1, 5, -1, -1, -1},
                                 {6, 2, 5, -1, -1},
                                 {1, 2, 6, 4, 3},
                                 {4, 5, -1, -1, -1}};

    for(i=1; i<=NB_SITE; i++){
        MPI_Send(&nb_voisins[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(voisins[i], nb_voisins[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
        MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
    }

    srand(time(0));
    initiateur = rand()%NB_SITE+1;

    MPI_Send(&initiateur, 1, MPI_INT, initiateur, TAGINIT, MPI_COMM_WORLD);
}

void calcul_min(int rang) {
    int nb_voisins, *voisins, min_local, i, recu, j;
    int pere, initiateur;
    MPI_Status status;


    initiateur = 0;
    pere = 0;


    MPI_Recv(&nb_voisins, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);

    voisins = (int *) malloc(nb_voisins * sizeof(int));

    MPI_Recv(voisins, nb_voisins, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
    MPI_Recv(&min_local, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
    MPI_Recv(&recu, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    pere = status.MPI_SOURCE;

    if (status.MPI_TAG == TAGINIT)
        initiateur = 1;
    else{
        if(recu < min_local)
            min_local = recu;
    }



    for(i=0; i<nb_voisins; i++)
        if(voisins[i] != pere)  // envoyer a sauf pere
            MPI_Send(&min_local, 1, MPI_INT, voisins[i], TAGREQ, MPI_COMM_WORLD);


    for(i=0; i<nb_voisins; i++){
        if (!initiateur && i == nb_voisins-1)   // pas initiateur recue tous le message sauf pere
            break;
        MPI_Recv(&recu, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(recu < min_local)
            min_local = recu;
    }


    if (pere != 0) { // envoi min local au pere
        MPI_Send(&min_local, 1, MPI_INT, pere, TAGMIN, MPI_COMM_WORLD);
    }

    if (initiateur) {
        printf("site: %d, min:%d, decideur\n", rang, min_local);


        for(i=0; i<nb_voisins; i++)
            MPI_Send(&min_local, 1, MPI_INT, voisins[i], TAGANN, MPI_COMM_WORLD);
    } else {
        printf("site: %d, pere:%d\n", rang, pere);

        MPI_Recv(&recu, 1, MPI_INT, pere, TAGANN, MPI_COMM_WORLD, &status);
        min_local = recu;

        printf("site: %d, min:%d\n", rang, min_local);


        for(i=0; i<nb_voisins; i++){  //annonce
            if(voisins[i] != pere)
                MPI_Send(&min_local, 1, MPI_INT, voisins[i], TAGANN, MPI_COMM_WORLD);
        }
    }

    free(voisins);
}

int main (int argc, char* argv[]) {
    int nb_proc, rang;
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
