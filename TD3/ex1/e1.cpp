#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
# define ROOT 0
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

  int p = atoi(argv[1]);int q = atoi(argv[2]);

  int ndims=2, dims[2], periods[2], reorder;
  dims[0]=p; dims[1]=q;
  periods[0]=0; periods[1]=0;
  reorder=1;


  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims,periods, reorder, &Comm_grille);
  int *coords = new int [2];
  MPI_Cart_coords(Comm_grille, pid,2,coords);
  int a=(coords[0]*pid+coords[1]);
  cout<< "id : "<<pid<<" " << a <<" "<< coords[0] << " "<< coords[1]  <<endl;
  MPI_Comm comm_col,comm_row;

  int  *c = new int [2] ;c[0]= 0 ;c[1]=1;

  MPI_Cart_sub(Comm_grille, c ,&comm_col);
  int  *c2 = new int [2] ;c2[0]= 1 ;c2[1]=0;

  MPI_Cart_sub(Comm_grille, c2 ,&comm_row);

  int sum_on_l;
  int sum_on_root;
  int rank_root;
  int *ccc=new int[1];
  ccc[0]=0;
  MPI_Cart_rank(comm_row, ccc,&rank_root);

  MPI_Reduce(&a,&sum_on_l, 1, MPI_INT, MPI_SUM,rank_root, comm_row);
  cout<< "l "<<sum_on_l<<endl;
  if(coords[0]==0){

    int sum_on_c;
    MPI_Cart_rank(comm_col, ccc,&rank_root);
    MPI_Reduce(&sum_on_l,&sum_on_root, 1, MPI_INT, MPI_SUM,rank_root, comm_col);

  }

//  cout<<"je marche"<<endl;

  if (pid== ROOT) {
    cout<< "root , somme = "<<sum_on_root<<endl;
  }

  MPI_Finalize() ;



  return 0 ;
}
