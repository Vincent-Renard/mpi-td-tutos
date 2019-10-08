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




  return 0;
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
      int *tab_local = new int [taille_globale/nprocs +1];

    if (pid == root) {
        tab_global = new int[taille_globale];
        generation_aleatoire(taille_globale, tab_global, pid, 50);
        cout << "Mon pid est : " << pid << " et le tableau initial est ";
        for (int i = 0; i < taille_globale; i++)
            cout << tab_global[i] << " ";
        cout << endl;
    }
    int *send_per_proc = new int [nprocs];
    int *offset = new int [nprocs];
    int s=0;
    if (taille_globale%nprocs==0){
      for (size_t i = 0; i < nprocs; i++) {
          send_per_proc[i]=taille_globale/nprocs;
        //  offset[i]=i*nprocs;
        offset[i]=s;
        s+=send_per_proc[i];
      }
    }else{
      for (size_t i = 0; i < nprocs; i++) {
          send_per_proc[i]=taille_globale/nprocs;
        //  offset[i]=i*nprocs;
          if (i<taille_globale%nprocs){
                send_per_proc[i]+=1;
                //offset[i]+=1;
          }
          offset[i]=s;
          s+=send_per_proc[i];

      }

    }
  MPI_Scatterv(tab_global, send_per_proc, offset, MPI_INT,tab_local , send_per_proc[pid], MPI_INT, root, MPI_COMM_WORLD);



    if(pid == root){

      cout<< "offset";
      for (size_t i = 0; i < nprocs; i++) {
          cout<<" "<<offset[i];
      }
      cout<<endl;


      cout<< "send_per_proc";
      for (size_t i = 0; i < nprocs; i++) {
          cout<<" "<<send_per_proc[i];
      }
      cout<<endl;





    }



    cout << "Mon pid est : " << pid << " et le tableau local   est ";
    for (int i = 0; i < send_per_proc[pid]; i++)
        cout << tab_local[i] << " ";
    cout << endl;


    //calcul_max(taille_globale,tab_local);

    // à compléter avec la distribution du tableau et le calcul du max associé à sa position dans le tableau global
  max_loc mx= malloc()
  mx ={tab_local[0],0};
for (size_t i = 0; i < send_per_proc[pid]; i++) {
  if (mx.max<tab_local[i]) {
    mx.max=tab_local[i];
    mx.pos=offset[pid]+i;
  }
}
cout<<"p: "<<pid<<" "<<mx.max<<" ["<<mx.pos<<"] "<<endl;
MPI_Reduce(&mx, &mx, 2, MPI_2INT, MPI_MAXLOC,root, MPI_COMM_WORLD);
/*
if (pid == root){
  cout<<"p: "<<pid<<" "<<mx.max<<" ["<<mx.pos<<"] "<<endl;
}
*/

    MPI_Finalize();
    return 0;
}
