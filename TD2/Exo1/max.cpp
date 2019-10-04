#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct max_loc {
    int max;
    int pos;
};


void generation_aleatoire(int taille_globale, int *tab, int graine, int max) {
    srand(time(NULL) + graine);
    //srand(2*graine+10);
    for (int i = 0; i < taille_globale; i++)
        tab[i] = rand() % (max + 1);
}

struct max_loc* calcul_max(int taille_globale, int* tab) {
  // à compléter
}

int main(int argc, char **argv) {
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Request request;
    MPI_Status status;

    int taille_globale = atoi(argv[1]); // la taille du tableau global
    int root = atoi(argv[2]); // le processeur root

    int *tab_global;

    if (pid == root) {
        tab_global = new int[taille_globale];
        generation_aleatoire(taille_globale, tab_global, pid, 50);
        cout << "Mon pid est : " << pid << " et le tableau initial est ";
        for (int i = 0; i < taille_globale; i++)
            cout << tab_global[i] << " ";
        cout << endl;
    }

    // à compléter avec la distribution du tableau et le calcul du max associé à sa position dans le tableau global

    MPI_Finalize();
    return 0;
}
