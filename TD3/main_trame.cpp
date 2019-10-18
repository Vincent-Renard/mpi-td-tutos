#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
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
      for (int k=0; k<n; k++)
	ele = ele + A[i*n+k]*B[k*n+j];
      C[i*n+j] = C[i*n+j] + ele;      
    }
  }  
}

// affichage d'une matrice nxn
void ecriture(int n, int* A)
{
  for (int i=0; i<n; i++)
    for (int j=0; j<n; j++)
      cout << A[i*n+j] << " ";
  cout << endl; 
}

int main ( int argc , char **argv )
{
  int pid, nprocs;  
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Comm Comm_grille; // Nouveau communicateur pour la topologie cartésienne.

  int n = atoi(argv[1]);
  
  srand(pid);
  int* A_local = generation_mat(n);
  int* B_local = generation_mat(n);
  int* C_local = new int[n*n];
  
  cout << "je suis " << pid << " et A_local : ";
  ecriture(n,A_local);
  
  cout << "je suis " << pid << " et B_local : ";
  ecriture(n,B_local);
  
  for (int i=0; i<n*n; i++)
    C_local[i] = 0;
  
  // A compléter ici étape par étape.

  
  cout << "je suis " << pid << " et C_local : ";
  ecriture(n,C_local);
  
  delete[] A_local;
  delete[] B_local;
  delete[] C_local;
  
  MPI_Finalize() ;
  
  

  return 0 ;
}
