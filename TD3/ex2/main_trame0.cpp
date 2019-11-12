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


  MPI_Status status;
  n = p = sqrt(nprocs);


  // A compléter ici étape par étape.


  int ndims=2, reorder;
  int dims[2] = {p,p};
  int periods[2]={1,1};
  reorder=1;
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims,periods, reorder, &grille);
  int *coords = new int [2];
  MPI_Cart_coords(grille, pid,2,coords);
  //DISTRIBUTION
  MPI_Cart_rank(grille, coords,&pid);
  int *send_counts=new int [nprocs];
  int *displs=new int [nprocs];


  cout << "je suis " << pid << " et je suis en ("<< coords[0]<<","<< coords[1] <<")"<<endl;
  MPI_Barrier(grille);

  srand(pid);
  int A_local=rand()%10;
  int B_local=rand()%10;
  int C_local=0;

  cout << "je suis " << pid << " et A_local : "<<A_local<<endl;
  cout << "je suis " << pid << " et B_local : "<<B_local<<endl;
  cout << "je suis " << pid << " et C_local : "<<C_local<<endl;
  // A -> lignes
  // B -> colonnes
  //int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype,int dest, int sendtag, int source, int recvtag, MPI_Comm comm,  MPI_Status *status)
  // MPI_Cart_shift(MPI_Comm comm, int direction, int disp,int *rank_source, int *rank_dest)

  int *coords_voisin[2];
  int droite;int gauche; int haut;int bas;
  MPI_Cart_shift(grille,0,coords[1],&gauche,&droite);
  MPI_Cart_shift(grille,1,coords[0],&haut,&bas);
  MPI_Barrier(grille);
  //cout << "je suis " << pid << " ("<< coords[0]<<","<< coords[1] <<")" <<"voisin = " << " ("<< coords_voisin[0]<<","<< coords_voisin[1] <<")"<<endl;
  cout << "je suis " << pid << " ("<< coords[0]<<","<< coords[1] <<")" <<" voisin_droite = " << " ("<<droite<<")"<<endl;
  MPI_Sendrecv_replace(&A_local,1,MPI_INT,droite,45,gauche,45,grille,&status);
  MPI_Sendrecv_replace(&B_local,1,MPI_INT,haut,54,bas,54,grille,&status);
  MPI_Barrier(grille);
  cout << "je suis " << pid << " et A_local_1 : "<<A_local<<endl;
  cout << "je suis " << pid << " et B_local_1 : "<<B_local<<endl;
  MPI_Finalize() ;



  return 0 ;
}
