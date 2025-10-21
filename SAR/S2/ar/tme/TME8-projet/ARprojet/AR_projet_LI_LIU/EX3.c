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

struct pair {
    int id;
    int succ;
    int rang;
};

struct reverse_pair {
    int ref_id;
    int ref_index;
};

struct reverse_table {
    struct reverse_pair *entries;
    int size;
};

struct reverse_table *create_reverse_tables(struct pair **finger_tables, int nb_pairs) {
    struct reverse_table *reverse_tables = malloc(nb_pairs * sizeof(struct reverse_table));
    for (int i = 0; i < nb_pairs; i++) {
        reverse_tables[i].entries = malloc(nombre_clefs_exposant * nb_pairs * sizeof(struct reverse_pair)); // 大致上界
        reverse_tables[i].size = 0;
    }

    for (int i = 0; i < nb_pairs; i++) {
        for (int j = 0; j < nombre_clefs_exposant; j++) {
            int target_id = finger_tables[i][j].id;
            for (int k = 0; k < nb_pairs; k++) {
                if (pairs[k].id == target_id) {
                    int idx = reverse_tables[k].size++;
                    reverse_tables[k].entries[idx].ref_id = pairs[i].id;
                    reverse_tables[k].entries[idx].ref_index = j;
                    break;
                }
            }
        }
    }
    return reverse_tables;
}


unsigned int getTimeSeed(void) {
    static int seedUsed = 0;
    if (!seedUsed) {
        srand((unsigned int)time(NULL));
        seedUsed = 1;
    }
    return rand();
}

int hashF(int mod){
    return getTimeSeed() % mod;
}


struct pair *creer_tableau_pair(int *pairs_ids, int taille) {
    struct pair *tableau = malloc(sizeof(struct pair) * taille);

    for (int i = 0; i < taille; i++) {
        tableau[i].id = pairs_ids[i];
        tableau[i].succ = pairs_ids[(i + 1) % taille];
        tableau[i].rang = i + 1;
    }
    return tableau;
}

int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}



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


void simulateur_ids(int nbPair) {

    int nombre_pairs = nbPair;

    initialisation_ids(&pairs_ids, nombre_pairs);
    pairs = creer_tableau_pair(pairs_ids, nombre_pairs);

}

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

struct pair **simulateur_finger_tables(int nbPair, struct reverse_table **rev_tables) {
    int nombre_pairs = nbPair;
    struct pair **finger_tables = malloc(sizeof(struct pair*) * nombre_pairs);

    for (int i = 0; i < nombre_pairs; i++) {
        finger_tables[i] = initialisation_finger_table(pairs[i].id, pairs, nombre_pairs);
    }

    *rev_tables = create_reverse_tables(finger_tables, nbPair);

    return finger_tables;
}



void send_id_finger(struct pair** finger_tables, int nbPair) {
    int id;
    for (int i = 0; i < nbPair; i++) {

        for (int j = 0; j < nbPair; ++j) {
            if (pairs[j].rang == i+1){
                id = pairs[j].id;
            }
        }

        MPI_Send(&id, 1, MPI_INT, i, initia, MPI_COMM_WORLD);

        MPI_Send(finger_tables[i], sizeof(struct pair) * nombre_clefs_exposant, MPI_BYTE, i, initia, MPI_COMM_WORLD);
    }
}



struct pair* receive_finger() {
    struct pair* received_table = malloc(sizeof(struct pair) * nombre_clefs_exposant);
    MPI_Status status;
    MPI_Recv(received_table, sizeof(struct pair) * nombre_clefs_exposant, MPI_BYTE, 0, initia, MPI_COMM_WORLD, &status);
    return received_table;
}

int receive_id() {
    int my_id;
    MPI_Status status;
    MPI_Recv(&my_id, 1, MPI_INT, 0, initia, MPI_COMM_WORLD, &status);
    return my_id;
}

void simulateur_find(int nbPair){
    srand((unsigned)time(NULL));

    int initiatorIndex, initiatorId, initiatorRank, target;

    do {
        initiatorIndex = (rand() % nbPair);
        initiatorId = pairs[initiatorIndex].id;
        initiatorRank = pairs[initiatorIndex].rang;
        target = (rand() % (nombre_clefs -2)) +1;

    } while (initiatorId == target);

    printf("Initiator ID: %d\n", initiatorId);
    printf("Initiator Rank: %d\n", initiatorRank);
    printf("target : %d\n", target);

    MPI_Send(&target, 1, MPI_INT, initiatorRank, demand, MPI_COMM_WORLD);
    printf("find demand Message sent\n");
}


struct pair findnext(int target, struct pair* finger_table, int my_id) {
    struct pair closest = { .id = -1, .succ = -1, .rang = -1 };

    int min_distance = (target - my_id + nombre_clefs) % nombre_clefs;
    for (int i = 0; i < nombre_clefs_exposant; i++) {
        //printf("node %d finding 2^%d\n",my_id,i);
        int dist = (target - finger_table[i].id + nombre_clefs) % nombre_clefs;
        if (dist <= min_distance) {
            closest = finger_table[i];
            //printf("new close :%d\n",closest.id);
            min_distance = dist;
        }else{
            break;
        }
    }

    // If no closer node is found, return an indication to stop further forwarding
    if (closest.id == -1 || closest.id == my_id) {
        printf("Node %d is the closest to target %d, stopping further forwarding.\n", my_id, target);
        return (struct pair){ .id = -1, .succ = -1, .rang = -1 };
    }

    return closest;
}



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


void find(int my_id, struct pair* finger_table) {
    int target;
    MPI_Status status;
    struct pair next;
    MPI_Request request;
    int act = 1;
    while (act) {
        MPI_Recv(&target, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        // 这里根据不同的消息类型执行不同的操作
        switch (status.MPI_TAG) {
            case demand:
                // 发起查找
                initiate_lookup(target, my_id, finger_table);
                break;
            case lookup:
                // 处理转发的查找请求
                next = findnext(target, finger_table, my_id);
                if (next.id == -1 || next.id == my_id) {
                    // 如果没有找到更接近的节点，或者当前节点就是目标节点
                    printf("Node %d is responsible for key %d\n", my_id, target);
                    MPI_Isend(&target, 1, MPI_INT, finger_table[0].rang, succe, MPI_COMM_WORLD,&request);
                    act = 0;
                } else {
                    // 找到了更接近的节点，转发查找请求
                    printf("Node %d forwarding lookup for key %d to node %d\n", my_id, target, next.id);
                    MPI_Send(&target, 1, MPI_INT, next.rang, lookup, MPI_COMM_WORLD);
                }
                break;
            case succe:
                MPI_Isend(&target, 1, MPI_INT, finger_table[0].rang, succe, MPI_COMM_WORLD,&request);
                act = 0;
                break;
        }
    }
}

void print_finger_and_reverse_tables(struct pair **finger_tables, struct reverse_table *reverse_tables, int nb_pairs) {
    for (int i = 0; i < nb_pairs; i++) {
        printf("Node ID %d - Finger Table:\n", pairs[i].id);
        for (int j = 0; j < nombre_clefs_exposant; j++) {
            printf("  Entry %d: Node ID %d, Successor %d, Position %d\n",
                   j, finger_tables[i][j].id, finger_tables[i][j].succ, finger_tables[i][j].rang);
        }
        printf("\n");

        printf("Node ID %d - Reverse Finger Table:\n", pairs[i].id);
        for (int j = 0; j < reverse_tables[i].size; j++) {
            printf("  Ref by Node ID %d, Index %d\n",
                   reverse_tables[i].entries[j].ref_id, reverse_tables[i].entries[j].ref_index);
        }
        printf("\n");
    }
}


void act(){
    int id;
    MPI_Status status;
    MPI_Recv(&id, 1, MPI_INT, 0, initia, MPI_COMM_WORLD, &status);

    printf("act_id:%d",id);
    fflush(stdout);

}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int nb_proc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    struct pair **finger_tables = NULL;  //for pro 0
    struct pair* my_finger_table;         //for another pro
    struct reverse_table *reverse_tables = NULL;

    if (rank == 0) {
        simulateur_ids(nb_proc - 2);
        finger_tables = simulateur_finger_tables(nb_proc - 1, &reverse_tables);
        send_id_finger(finger_tables, nb_proc - 2);
        simulateur_find(nb_proc - 1);
        print_finger_and_reverse_tables(finger_tables, reverse_tables, nb_proc - 1);
    }else if(rank == nb_proc-1){

        fflush(stdout);

        act();

    }else{
        int my_id = receive_id();
        my_finger_table = receive_finger();
        find(my_id, my_finger_table);


    }

    //释放内存
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


