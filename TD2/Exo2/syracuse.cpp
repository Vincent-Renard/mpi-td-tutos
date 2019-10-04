#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*! 
 * Pour tester si une suite est une suite de syracuse (à partir du premier élément du tableau)
 * \param n la taille du tableau (la suite)
 * \param tab la suite à tester
 * \return 0 si ce n'est pas une suite de Syracuse 1 sinon
 */
int test_syracuse(int n, int* tab)
{
  return 0;
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
    }
  }
  if (pid==root){
    cout << "je suis :" << pid << " ";
    for (int i=0; i<n; i++)
      cout << tab_global[i] << " ";
    cout << endl;
  }

  /* à compléter */

  /* le résultat n'est disponible que sur le processeur root */
  int res;
  /* à compléter */
  if (pid==root)
    cout << "test de syracuse : " << res << endl;
  
  MPI_Finalize() ;
  return 0 ;
}
