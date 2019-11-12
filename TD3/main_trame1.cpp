#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;


// génération aléatoire d'une matrice d'entiers taillextaille
// comme un tableau à une dimension
int*  generation_mat(int taille)
{
  int* mat = new int[taille*taille];
  for (int i=0; i<taille*taille; i++)
  mat[i] = rand()%10;
  return mat;
}

// Produit C+=AxB
void operation_elementaire(int n, int* A, int* B, int* C)
{
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      int ele = 0;
      for (int k=0; k<n; k++){
        ele = ele + A[i*n+k]*B[k*n+j];
      }
      C[i*n+j] = C[i*n+j] + ele;
    }
  }
}


void prepare_scaterv( int global_size, int nb_procs, int displs[], int send_counts[]){
  int sum=0;
  if (global_size%nb_procs==0){
    for (size_t i = 0; i < nb_procs; i++) {
      send_counts[i]=global_size/nb_procs;
      displs[i]=sum;
      sum+=send_counts[i];
    }
  }else{
    for (size_t i = 0; i < nb_procs; i++) {
      send_counts[i]=global_size/nb_procs;
      if (i<global_size%nb_procs){
        send_counts[i]+=1;
      }
      displs[i]=sum;
      sum+=send_counts[i];
    }
  }

}

// affichage d'une matrice nxn
void ecriture(int n, int* A)
{
  for (int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      cout << A[i*n+j] << " ";
    }
      //cout << endl;
  }
  cout << endl;



}

int main ( int argc , char *argv[] )
{
  int pid, nprocs ,n,p;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Comm grille; // Nouveau communicateur pour la topologie cartésienne.

  //int n = atoi(argv[1]);

  n = p = sqrt(nprocs);

  srand(pid);
  //int* A_local = generation_mat(n);
  //int* B_local = generation_mat(n);
  int* C_local = new int[n*n];

  cout << "je suis " << pid << " et A_local : ";
//  ecriture(n,A_local);
  cout << "je suis " << pid << " et B_local : ";
//  ecriture(n,B_local);

  //INIT C
/*
  for (int i=0; i<n*n; i++)
  C_local[i] = 0;
*/

  // A compléter ici étape par étape.


    int ndims=2, reorder;
    int dims[2] = {p,p};
    int periods[2]={0,1};
    reorder=1;
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims,periods, reorder, &grille);
    int *coords = new int [2];
    MPI_Cart_coords(grille, pid,2,coords);
  //DISTRIBUTION
  MPI_Cart_rank(grille, coords,&pid);
  int *send_counts=new int [nprocs];
  int *displs=new int [nprocs];

cout << "je suis " << pid << " et je suis en ("<< coords[0]<<","<< coords[1] <<")"<<endl;

cout << "je suis " << pid << " et C_local : ";
  ecriture(n,C_local);
/*
  delete[] A_local;
  delete[] B_local;
  delete[] C_local;
*/
  MPI_Finalize() ;



  return 0 ;
}
