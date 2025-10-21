#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

#define nombre_clefs_exposant 6

#define demand 0
#define lookup 1
#define responsable 2
#define succe 3
#define echec 4

#define initia 5

int *pairs_ids ;
struct pair *pairs;

int nombre_clefs = 1 << nombre_clefs_exposant;

//Chaque nœud a deux éléments : son propre identifiant et la représentation de son successeur
// (plus le numéro de processus du nœud pour l'envoi et la réception de messages, puisque mpi est utilisé).
struct pair {
    int id;
    int succ;
    int rang;
};


unsigned int getTimeSeed(void) {
    static int seedUsed = 0;
    if (!seedUsed) {
        srand((unsigned int)time(NULL));
        seedUsed = 1;
    }
    return rand();
}

//fonction de hachage
int hashF(int mod){
    return getTimeSeed() % mod;
}

//Construisez chaque paire
struct pair *creer_tableau_pair(int *pairs_ids, int taille) {
    struct pair *tableau = malloc(sizeof(struct pair) * taille);

    for (int i = 0; i < taille; i++) {
        tableau[i].id = pairs_ids[i];
        tableau[i].succ = pairs_ids[(i + 1) % taille];
        tableau[i].rang = i + 1;
    }
    return tableau;
}

//Comparaison pour le tri d'identifiants de nœuds obtenus de manière aléatoire
int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}


//Initialise un tableau d'identifiants de nœuds.
// Il génère un identifiant unique pour chaque nœud et s'assure que tous les identifiants ne sont pas dupliqués.
void initialisation_ids(int **p_pairs_ids, int nombre_pairs) {
    *p_pairs_ids = malloc(sizeof(int) * nombre_pairs);
    int *pairs_ids = *p_pairs_ids;
    bool unique;

    for (int i = 0; i < nombre_pairs; i++) {
        do {
            unique = true;
            pairs_ids[i] = hashF(nombre_clefs);
            for (int j = 0; j < i; j++) {
                if (pairs_ids[j] == pairs_ids[i]) {
                    unique = false;
                    break;
                }
            }
        } while (!unique);
    }

    qsort(pairs_ids, nombre_pairs, sizeof(int), compare_ints);
}

//Appelle la fonction paires pour créer des paires.
void simulateur_ids(int nbPair) {

    int nombre_pairs = nbPair;

    initialisation_ids(&pairs_ids, nombre_pairs);
    pairs = creer_tableau_pair(pairs_ids, nombre_pairs);

}

//Générer une table d'empreintes digitales pour chaque nœud afin de déterminer quel nœud est responsable
// d'une valeur clé particulière.
struct pair *initialisation_finger_table(int id, struct pair *pairs, int nombre_pairs) {
    struct pair *finger_table = malloc(sizeof(struct pair) * nombre_clefs_exposant);

    for (int i = 0; i < nombre_clefs_exposant; i++) {
        int target_id = (id + (int)pow(2, i)) % nombre_clefs;
        struct pair closest = { .id = -1, .rang = -1 };
        int distance_min = nombre_clefs;

        for (int j = 0; j < nombre_pairs; j++) {
            int distance = (pairs[j].id - target_id + nombre_clefs) % nombre_clefs;
            if (distance < distance_min && pairs[j].id >= target_id) {
                closest = pairs[j];
                distance_min = distance;
            }
        }

        if (closest.id == -1) {
            for (int j = 0; j < nombre_pairs; j++) {
                int distance = (pairs[j].id - target_id + nombre_clefs) % nombre_clefs;
                if (distance < distance_min) {
                    closest = pairs[j];
                    distance_min = distance;
                }
            }
        }

        finger_table[i] = closest;
    }
    return finger_table;
}

//Créez une finger table pour chaque nœud
struct pair **simulateur_finger_tables(int nbPair) {
    int nombre_pairs = nbPair;

    struct pair **finger_tables = malloc(sizeof(struct pair*) * nombre_pairs);

    for (int i = 0; i < nombre_pairs; i++) {
        finger_tables[i] = initialisation_finger_table(pairs[i].id, pairs, nombre_pairs);
    }


//Affichez l'identifiant du nœud créé ci-dessus.
    for (int i = 0; i < nombre_pairs; i++) {
        printf("Node %d Finger Table first entry: Node ID %d\n", pairs[i].id, finger_tables[i][0].id);
    }


//Afficher toutes les finger tables.
    for (int i = 0; i < nombre_pairs; i++) {
        printf("Node %d Finger Table:\n", pairs[i].id);
        for (int j = 0; j < nombre_clefs_exposant; j++) {
            printf("Entry %d: Node ID %d\n",
                   j, finger_tables[i][j].id);
        }
        printf("\n");
    }

    return finger_tables;
}

//Envoyer l'identifiant et la finger table au processus correspondant
void send_id_finger(struct pair** finger_tables, int nbPair) {
    int id;
    for (int i = 0; i < nbPair; i++) {

        for (int j = 0; j < nbPair; ++j) {
            if (pairs[j].rang == i+1){
                id = pairs[j].id;
            }
        }
        //id
        MPI_Send(&id, 1, MPI_INT, i + 1, initia, MPI_COMM_WORLD);
        //finger table
        MPI_Send(finger_tables[i], sizeof(struct pair) * nombre_clefs_exposant, MPI_BYTE, i + 1, initia, MPI_COMM_WORLD);
    }
}

//les processuses pour recevoir la finger table du processus 0.
struct pair* receive_finger() {
    struct pair* received_table = malloc(sizeof(struct pair) * nombre_clefs_exposant);
    MPI_Status status;
    MPI_Recv(received_table, sizeof(struct pair) * nombre_clefs_exposant, MPI_BYTE, 0, initia, MPI_COMM_WORLD, &status);
    return received_table;
}

//les processuses pour recevoir l'identifiant du processus 0.
int receive_id() {
    int my_id;
    MPI_Status status;
    MPI_Recv(&my_id, 1, MPI_INT, 0, initia, MPI_COMM_WORLD, &status);
    return my_id;
}

//L'initiateur est généré de manière aléatoire et la clé cible est générée de manière aléatoire,
// puis un message de début de recherche est envoyé au processus correspondant.
void simulateur_find(int nbPair){
    srand((unsigned)time(NULL));

    int initiatorIndex, initiatorId, initiatorRank, target;

    do {
        initiatorIndex = (rand() % nbPair);
        initiatorId = pairs[initiatorIndex].id;    //id
        initiatorRank = pairs[initiatorIndex].rang;   //rank
        target = (rand() % (nombre_clefs -2)) +1;   //dest

    } while (initiatorId == target);

    printf("Initiator ID: %d\n", initiatorId);
    printf("Initiator Rank: %d\n", initiatorRank);
    printf("target : %d\n", target);

    MPI_Send(&target, 1, MPI_INT, initiatorRank, demand, MPI_COMM_WORLD);
}

//La fonction findnext de l'algorithme décrit dans TD est utilisée pour trouver le prochain nœud approprié.
struct pair findnext(int target, struct pair* finger_table, int my_id) {
    struct pair closest = { .id = -1, .succ = -1, .rang = -1 };

    int min_distance = (target - my_id + nombre_clefs) % nombre_clefs;
    for (int i = 0; i < nombre_clefs_exposant; i++) {
        int dist = (target - finger_table[i].id + nombre_clefs) % nombre_clefs;
        if (dist <= min_distance) {
            closest = finger_table[i];
            min_distance = dist;
        }else{
            break;
        }
    }

    if (closest.id == -1 || closest.id == my_id) {
        return (struct pair){ .id = -1, .succ = -1, .rang = -1 };
    }

    return closest;
}


//La fonction initiate_lookup de l'algorithme décrit dans TD p
// oursuit la recherche si la fonction findnext ne trouve pas le nœud le plus approprié.
void initiate_lookup(int target, int my_id, struct pair* finger_table) {
    MPI_Request request;
    struct pair next = findnext(target, finger_table,my_id);
    if (next.id == -1 || next.id == my_id) {
        printf("Node %d is responsible for key %d\n", my_id, target);
        MPI_Isend(&target, 1, MPI_INT, finger_table[0].rang, succe, MPI_COMM_WORLD,&request);
    } else {
        printf("Node %d forwarding lookup for key %d to node %d\n", my_id, target, next.id);
        MPI_Send(&target, 1, MPI_INT, next.rang, lookup, MPI_COMM_WORLD);
    }
}

//Fonction principale de la recherche, utilisée pour démarrer la recherche, la faire progresser et la terminer.
void find(int my_id, struct pair* finger_table) {
    int target;
    MPI_Status status;
    struct pair next;
    MPI_Request request;
    int act = 1;
    while (act) {
        MPI_Recv(&target, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


        switch (status.MPI_TAG) {
            case demand:        //C'est l'initiation
                initiate_lookup(target, my_id, finger_table);
                break;
            case lookup:        //C'est le transfert.
                next = findnext(target, finger_table, my_id);
                if (next.id == -1 || next.id == my_id) {
                    printf("Node %d is responsible for key %d\n", my_id, target);
                    MPI_Isend(&target, 1, MPI_INT, finger_table[0].rang, succe, MPI_COMM_WORLD,&request);
                    act = 0;
                } else {
                    printf("Node %d forwarding lookup for key %d to node %d\n", my_id, target, next.id);
                    MPI_Send(&target, 1, MPI_INT, next.rang, lookup, MPI_COMM_WORLD);
                }
                break;
            case succe:     //C'est la fin.
                MPI_Isend(&target, 1, MPI_INT, finger_table[0].rang, succe, MPI_COMM_WORLD,&request);
                act = 0;
                break;
        }
    }
}



int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int nb_proc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



    struct pair **finger_tables = NULL;  //for pro 0
    struct pair* my_finger_table;         //for another pro

    if (rank == 0) {// Processus 0
        simulateur_ids(nb_proc - 1); //Création des paires
        finger_tables = simulateur_finger_tables(nb_proc - 1); //Création des finger table
        send_id_finger(finger_tables, nb_proc - 1); //Envoyer les paires et les fingers tables
        simulateur_find(nb_proc - 1); //Lancer à chercher.
    }else { //autre processuses
        int my_id = receive_id(); //Obtenir l'identifiant
        my_finger_table = receive_finger();//Obtenir la finger table
        find(my_id, my_finger_table); //Commencer à chercher.
    }

    //Libérer de la mémoire
    if (rank == 0) {
        for (int i = 0; i < nb_proc - 1; i++) {
            free(finger_tables[i]);
        }
        free(finger_tables);
        free(pairs_ids);
        free(pairs);
    } else{
        free(my_finger_table);
    }



    MPI_Finalize();
    return 0;
}


