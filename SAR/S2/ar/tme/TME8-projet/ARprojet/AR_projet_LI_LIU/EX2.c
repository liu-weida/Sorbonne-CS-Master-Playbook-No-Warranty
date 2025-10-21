#include <stdio.h>
#include <mpi.h>
#include <stdlib.h> // for rand and srand
#include <time.h>   // for time
#include <math.h>
#include <unistd.h>
#include <err.h>

#define nombre_clefs_exposant 6

#define initialization 0

#define initialization_again 1

#define array 2

#define launch 3

#define collect 4
#define transmit 5

int nombre_clefs = 1 << nombre_clefs_exposant;

int compare(const void *a, const void *b) {
    const int *elem1 = *(const int **)a;
    const int *elem2 = *(const int **)b;
    return *elem1 - *elem2;
}


//La fonction de processus du simulateur, qui est responsable de l'organisation en anneaux et de la sélection du processus d'initiation.
void simulateur(int nbPeers) {
    MPI_Status status;
    int **ids = malloc(nbPeers * sizeof(int*));
    int duplicate_found;

    do {
        duplicate_found = 0;

        // Recevez les identifiants générés par tous les nœuds.
        for (int i = 0; i < nbPeers; ++i) {
            ids[i] = malloc(2 * sizeof(int));
            MPI_Recv(&ids[i][0], 1, MPI_INT, MPI_ANY_SOURCE, initialization, MPI_COMM_WORLD, &status);
            ids[i][1] = status.MPI_SOURCE;
        }

        //mettre en ordre
        qsort(ids, nbPeers, sizeof(int *), compare);


        // Vérifier s'il y a des doublons d'identification
        for (int i = 1; i < nbPeers; i++) {
            if (ids[i][0] == ids[i-1][0]) {
                duplicate_found = 1;
                printf("dublicate found! Please wait for re-initialization\n");
                break;
            }
        }

        //  Envoyer un message à tous les processus pour savoir s'il faut régénérer l'identifiant
        //  (car il peut y avoir des doublons dans les identifiants générés par les différents nœuds)
        //  (duplicate_found de 1 signifie qu'il y a des doublons et qu'il faut les régénérer).
        for (int i = 0; i < nbPeers; ++i) {
            MPI_Send(&duplicate_found, 1, MPI_INT, ids[i][1], initialization_again, MPI_COMM_WORLD);
        }

        // S'il y a des doublons, effacez la mémoire pour accepter à nouveau les données.
        if (duplicate_found) {
            for (int i = 0; i < nbPeers; ++i) {
                free(ids[i]);
            }
        }

    } while (duplicate_found);
    // Produire le résultat trié
        printf("id and rank\n");
        for (int i = 0; i < nbPeers; ++i) {
            printf("ids[%d][0] = %d, ids[%d][1] = %d\n", i, ids[i][0], i, ids[i][1]);
        }

    // Envoyer l'identifiant et le rang du nœud suivant
    for (int i = 0; i < nbPeers; ++i) {
        int succ_id = ids[(i + 1) % nbPeers][0];
        int succ_rank = ids[(i + 1) % nbPeers][1];
        int suc[2];
        suc[0] = succ_id;
        suc[1] = succ_rank;
        MPI_Send(&suc, 2, MPI_INT, ids[i][1], array, MPI_COMM_WORLD); //
    }

    //Sélection aléatoire d'un processus comme processus de démarrage
    srand(time(NULL));
    int dest = (rand() % nbPeers) + 1;

    MPI_Send(NULL, 0, MPI_INT, dest, launch, MPI_COMM_WORLD); //Envoyer un message de départ.


    for (int i = 0; i < nbPeers; ++i) {
        free(ids[i]);
    }
    free(ids);



}


// Trouvez l'ID du nœud qui satisfait à la condition
int find_successor(int target_id, int *ids, int nbPeers) {
    int successor = ids[0];
    int diff = nombre_clefs;
    for (int i = 0; i < nbPeers; i++) {
        int current_diff = ids[i] - target_id;
        if (current_diff < 0) {
            current_diff += nombre_clefs;  // Ajuster la structure de l'anneau.
        }
        if (current_diff < diff) {
            diff = current_diff;
            successor = ids[i];
        }
    }
    return successor;
}

// Création des finger table
void build_finger_table(int my_id, int *ids,  int *finger_table) {
    for (int k = 0; k < nombre_clefs_exposant; k++) {
        int step = (1 << k);
        int desired_id = (my_id + step) % nombre_clefs;
        finger_table[k] = find_successor(desired_id, ids, nombre_clefs_exposant);
    }
}

// Imprimer les finger table
void print_finger_table(int my_id, int *finger_table) {
    printf("Finger table for node %d:\n", my_id);
    for (int i = 0; i < nombre_clefs_exposant; i++) {
        printf("Entry %d: Node ID %d\n", i, finger_table[i]);
    }
}

//Comportement des processus non simulateurs.
void peers(int rang, int nbPeers) {
    int my_id, continue_process = 1;
    MPI_Status status;
    int *finger_table = malloc(nbPeers * sizeof(int));

    //La génération d'identifiants est répétée jusqu'à ce que le processus du simulateur reçoive un identifiant non dupliqué.
    srand(time(NULL) + rang);
    int random = (rand() % (nombre_clefs - 1)) + 1;
    while (continue_process) {
        my_id = random + rang;
        MPI_Send(&my_id, 1, MPI_INT, 0, initialization, MPI_COMM_WORLD);
        MPI_Recv(&continue_process, 1, MPI_INT, 0, initialization_again, MPI_COMM_WORLD, &status);
        if (continue_process == 0) {
            break;
        }
    }

    int succ[2];
    int succ_rank;

    MPI_Recv(&succ, 2, MPI_INT, 0, array, MPI_COMM_WORLD, &status);

    succ_rank = succ[1];

    int *id = malloc(nbPeers * sizeof(int)); //Construit une liste de taille nbPeers.

    MPI_Recv(id, nbPeers, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    if(status.MPI_SOURCE == 0 && status.MPI_TAG == launch){ // Le comportement du nœud initiateur.


        id[0]=my_id;

        for (int i = 1; i < nbPeers; ++i) {
            id[i] = -1;
        }


        MPI_Send(id, nbPeers, MPI_INT, succ_rank, collect, MPI_COMM_WORLD);


        //Attendez la première réception d'un message contenant une étiquette COLLECT
        // (indiquant que le premier tour a fini).
        MPI_Recv(id, nbPeers, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == collect){
            //Début du deuxième tour.
            MPI_Send(id, nbPeers, MPI_INT, succ_rank, transmit, MPI_COMM_WORLD);
        }

        //Attendez la première réception d'un message contenant une étiquette transmit
        // (indiquant que le 2e tour a été fini).
        MPI_Recv(id, nbPeers, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == transmit) {
            for (int i = 0; i < nbPeers; ++i) {
                if (id[i] == -1){
                    perror("error");
                }
            }

            //Construction de la finger table
            build_finger_table(my_id,id,finger_table);
            fflush(stdout);
        }

    }else if(status.MPI_TAG == collect){   //Comportement des nœuds non initiateurs.

        for (int i = 0; i < nbPeers; ++i) {
            if(id[i] == -1){
                id[i] = my_id;
                break;
            }
        }
        //La première fois que vous recevez un message "collec", le processus y attache son identifiant et le transmet.
        MPI_Send(id, nbPeers, MPI_INT, succ_rank, collect, MPI_COMM_WORLD);

        //Le processus attend de recevoir le message de transmit
        MPI_Recv(id, nbPeers, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == transmit){
            //Dès réception, transmetter et construiser la finger table.
            MPI_Send(id, nbPeers, MPI_INT, succ_rank, transmit, MPI_COMM_WORLD);
            build_finger_table(my_id,id,finger_table);
        }



        fflush(stdout);

    }
//Imprimer la finegr table
    print_finger_table(my_id,finger_table);

}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int nb_proc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        simulateur(nb_proc-1);  //Fonctions de comportement du simulateur
    } else {
        peers(rank, nb_proc-1); //Fonctions de comportement du nœud pair
    }

    MPI_Finalize();
    return 0;
}
