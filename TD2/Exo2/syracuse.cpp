#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*!
* Pour tester si une suite est une suite de syracuse (à partir du premier élément du tableau)
* \param n la taille du tableau (la suite)
* \param tab la suite à tester
* \return 0 si ce n'est pas une suite de Syracuse 1 sinon
*/
int test_syracuse(int n, int* tab)
{
  if (tab[0]<=0){
    return 0;
  }
  for (size_t i = 1; i < n; i++) {
    if (tab[i-1]%2 ==0){
      if(tab[i]!=tab[i-1]/2) return 0;
    }else
    {
      if(tab[i] !=(3*tab[i-1]+1)){
        return 0;
      }
    }
  }
  return 1;
}


/*!
* génération d'une suite de Syracuse à partir de U0=x
* \param n la taille voulue
* \param tab le tableau qui enregistre la suite
* \param x la valeur initiale
*/
void syracuse(int n, int* tab, int x)
{
  tab[0] = x;
  for (int i=1; i<n; i++) {
    if (tab[i-1]%2==0)
    tab[i] = tab[i-1]/2;
    else
    tab[i] = 3*tab[i-1]+1;
  }
}

/*!
* génération aléatoire d'une suite quelconque
* \param n la taille voulue
* \param tab le tableau qui enregistre la suite
* \param graine pour le processus de génération aléatoire
*/
void non_syracuse(int n, int* tab, int graine)
{
  srand(time(NULL)+graine);
  //srand(2*graine+10);
  for (int i=0; i<n; i++)
  tab[i] = rand()%50;
}

int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  MPI_Request request;
  MPI_Status status;

  int n = atoi(argv[1]); // la taille du tableau global
  int root = atoi(argv[2]); // le processeur root
  int x = atoi(argv[3]); // le x pour générer une suite de syracuse
  int cas = atoi(argv[4]);  // les différentes générations possibles

  int* tab_global = new int[n];
  int* tab_local = new int[n/nprocs +1];
  if (pid==root) {
    switch(cas) {
      case 0:
      cout << "test génération aléatoire"<< endl;
      non_syracuse(n,tab_global,pid);
      break;
      case 1:
      cout << "test syracuse partiel"<< endl;
      syracuse(n/2,tab_global,x);
      non_syracuse(n-n/2,(tab_global+n/2),pid);
      break;
      default:
      cout << "test syracuse"<< endl;
      syracuse(n,tab_global,x);
        cout << "syracuse ? "<< test_syracuse(n,tab_global) << endl;
    }
  }
  if (pid==root){
    cout << "je suis :" << pid << " ";
    for (int i=0; i<n; i++)
    cout << tab_global[i] << " ";
    cout << endl;
  }




  int *send_per_proc = new int [nprocs];
  int *offset = new int [nprocs];
  int s=0;
  if (n%nprocs==0){
    for (size_t i = 0; i < nprocs; i++) {
      send_per_proc[i]=n/nprocs;
      offset[i]=s;
      s+=send_per_proc[i];
    }
  }else{
    for (size_t i = 0; i < nprocs; i++) {
      send_per_proc[i]=n/nprocs;
      if (i<n%nprocs){
        send_per_proc[i]+=1;
      }
      offset[i]=s;
      s+=send_per_proc[i];
    }
  }




  /*GESTION DES GHOSTS*/
  for (size_t proc_i = 0; proc_i < nprocs; proc_i++) {
    send_per_proc[proc_i]+=1;
    if (proc_i!= 0){
      offset[proc_i]-=1; // Décale le displs pour prndre le ghost de gauche
    }
  }

  //Envoi du tableau
  MPI_Scatterv(tab_global, send_per_proc, offset, MPI_INT,tab_local , send_per_proc[pid], MPI_INT, root, MPI_COMM_WORLD);


  // affichage des suites locales
  cout << "Mon pid est : " << pid << " et la suite locale   est ";
  for (size_t i = 0; i < send_per_proc[pid]; i++)
  cout << tab_local[i] << " ";
  cout << endl;


  int tst =test_syracuse(send_per_proc[pid],tab_local);
  cout << "p : "<<pid << " r:" << tst << endl;


  int res ; // le min des tests de Syracuse
  MPI_Reduce( &tst, &res,1, MPI_INT, MPI_MIN,root, MPI_COMM_WORLD);

  /* le résultat (res ) n'est disponible que sur le processeur root */


  if (pid==root)
  cout << "test de syracuse : " << res << endl;

  MPI_Finalize() ;
  return 0 ;
}
