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

  max_loc *max_global = new max_loc ;
  max_global->max=tab[0];
  max_global->pos=0;
  for (size_t i = 0; i < taille_globale; i++) {
    if (max_global->max<tab[i]) {
      max_global->max=tab[i];
      max_global->pos=i;
    }
  }
  return max_global;
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
    for (size_t proc_i = 0; proc_i < nprocs; proc_i++) {
      send_per_proc[proc_i]=taille_globale/nprocs;
      //  offset[i]=i*nprocs;
      offset[proc_i]=s;
      s+=send_per_proc[proc_i];
    }
  }else{
    for (size_t proc_i = 0; proc_i < nprocs; proc_i++) {
      send_per_proc[proc_i]=taille_globale/nprocs;
      if (proc_i<taille_globale%nprocs){
        send_per_proc[proc_i]+=1;
      }
      offset[proc_i]=s;
      s+=send_per_proc[proc_i];
    }
  }


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
  // distribution du tableau global par procs
  MPI_Scatterv(tab_global, send_per_proc, offset, MPI_INT,tab_local , send_per_proc[pid], MPI_INT, root, MPI_COMM_WORLD);

  cout << "Mon pid est : " << pid << " et le tableau local   est ";
  for (size_t i = 0; i < send_per_proc[pid]; i++)
  cout << tab_local[i] << " ";
  cout << endl;
/*
q=n/nprocs
r = n%nprocs

  count[i] = i < r ? q+1 : q ;
  displ[i] = i < r ? (q+1)*i : q*i +r ;
*/
  // à compléter avec la distribution du tableau et le calcul du max associé à sa position dans le tableau global
  max_loc *mx = calcul_max(send_per_proc[pid],tab_local);
  mx->pos += (offset[pid]);

  max_loc *max_on_root = new max_loc;
  cout<<"p: "<<pid<<" "<<mx->max<<" ["<<mx->pos<<"] "<<endl;
  /*REINIT*/

  MPI_Reduce(mx,max_on_root, 1, MPI_2INT, MPI_MAXLOC,root, MPI_COMM_WORLD);

  if (pid == root){
  cout<<"MAX "<<max_on_root->max<<" ["<<max_on_root->pos<<"] "<<endl;

}

MPI_Finalize();

return 0;
}
