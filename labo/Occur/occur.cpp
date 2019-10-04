#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;




int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Status status;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

  int taille_t_local=atoi(argv[1]);

  int root = atoi(argv[2]);
  int n_elts_diffs = atoi(argv[3]);

  //TAB LOCAL
  int* t_l = new int[taille_t_local];
  //INIT OCCURS LOCAL

int *occs_f= new int[n_elts_diffs*nprocs];
  int* o_local = new int[n_elts_diffs];
  for (size_t idx = 0; idx < n_elts_diffs; idx++) {
    o_local[idx]=0;
  }
  int taille_t_g = taille_t_local*nprocs;
  int t_g [taille_t_g];
  if (pid==root){


    //INIT OCCURS
    int* o_global = new int[n_elts_diffs];
    for (size_t idx = 0; idx < n_elts_diffs; idx++) {
      o_global[idx]=0;
    }

    srand(time(NULL)+pid);
    int e=0;
    //REMPLISSAGE ET DECOMPTE( pour vérification)
    cout << "TG [";
    for (int i=0; i<taille_t_g; i++){
      e=rand()%n_elts_diffs;
      t_g[i] = e;
      cout <<" "<<e;

      o_global[e]+=1;
    }
      cout<<" ]"<<endl;
    cout <<"Occurences globales [";
    for (int i=0; i<n_elts_diffs; i++){
      cout << o_global[i] << " ";
    }
    cout <<"]" <<endl;


    delete[] o_global;


  }
  MPI_Scatter(t_g,taille_t_local,MPI_INT,t_l,taille_t_local,MPI_INT,root,MPI_COMM_WORLD);

//  cout<<"pid: "<<pid<<" [";
  for (size_t i = 0; i < taille_t_local; i++) {
    //cout<<" "<<t_l[tli];

    o_local[ t_l[i] ]+=1;

  }

MPI_Barrier(MPI_COMM_WORLD);
MPI_Gather(o_local, n_elts_diffs, MPI_INT,  occs_f, n_elts_diffs, MPI_INT, root,MPI_COMM_WORLD);


  if (pid==root) {
    /*
    0 = 0 4 8   12 16 20
    1 = 1 5 9   13 17 21
    2 = 2 6 10  14 18 22
    3 = 3 7 11  15 19 23
    */

    int * occ_final = new int[n_elts_diffs];

    for (size_t of_idx = 0; of_idx < n_elts_diffs; of_idx++) {
      occ_final[of_idx]=0;
      for (size_t p = 0; p < nprocs; p++) {

        occ_final[of_idx]+=occs_f[(n_elts_diffs*p)+of_idx];
      }
      cout<<" "<<occ_final[of_idx];
    }
      cout<<endl;

  }

  delete[] t_l;

  delete[] o_local;
  MPI_Finalize() ;
  return 0 ;
}
