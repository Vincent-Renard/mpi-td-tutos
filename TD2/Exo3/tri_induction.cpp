#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

/*!
 * pour calculer un indice local � partir d'un indice global
 * /param n la taille totale du tableau
 * /param i_global l'indice dans le tableau complet
 * /return i_local l'indice du tableau en local
 */
int global_to_local(int n, int i_global)
{
  return 0;
}

/*!
 * pour calculer un indice global � partir d'un indice local
 * /param n la taille totale du tableau
 * /param i_local l'indice local initial
 * /return i_global l'indice de l'�l�ment correspondant dans le tableau global
 */
int local_to_global(int n, int i_local)
{
  return 0;
}

/*!
 * pour calculer le num�ro du processeur destinataire � partir d'un indice dans le tableau total
 * /param n la taille totale du tableau
 * /param i_global l'indice dans le tableau global
 * /return pid_dest le num�ro du processeur propri�taire de l'�l�ment
 */
int pid_dest(int n, int i_global)
{
  return 0;
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

/*!
 * Petite fonction pour etre s�r de g�n�rer un tableau dont tous les �l�ments sont distincts
 * \param n la taille du tableau total
 * \param tab le tableau � construire
 * \param graine pour initialiser le processus de g�n�ration al�atoire de nombre
 */
void generation(int n, int* tab, int graine)
{
  srand(time(NULL)+graine);
  //srand(2*graine+10);
  for (int i=0; i<n; i++) {
    bool test = true;
    while (test) {
      test = false;
      tab[i] = rand()%50;
      for (int j=0; j<i; j++)
	if (tab[i]==tab[j])
	  test = true;
    }
  }
}

int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Request request;
  MPI_Status status;

  int n = atoi(argv[1]);
  int root = atoi(argv[2]);


  int* tab_global = new int[n];
  int* tab_local;

  if (pid==root){
      generation(n,tab_global,pid);
      cout<<"Tab global :";
      for (size_t i = 0; i < n; i++) {
          cout<<" " <<tab_global[i];
      }
      cout<<endl;
  }

  int *displs=new int [nprocs];
  int *send_counts = new int [nprocs];
  prepare_scaterv(n,nprocs,displs,send_counts);

  if (pid==root){
    cout<<"send_counts";
    for (size_t i = 0; i < nprocs; i++) {
      cout<<" "<<send_counts[i];
    }
    cout<<endl;
    cout<<"displs";
    for (size_t i = 0; i < nprocs; i++) {
      cout<<" "<<displs[i];
    }
    cout<<endl;
  }


  MPI_Finalize() ;
  return 0 ;
}
