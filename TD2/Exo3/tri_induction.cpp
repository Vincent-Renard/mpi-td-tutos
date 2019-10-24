#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;



/*!
 * Tri sequentiel
 * /param n la taille totale du tableau
 * /param t le tableau complet
 * /return le tableau trié
 */
int* tri_insert(int n, int *t)
{
  int *r = new int[n];

  for (size_t i = 0; i < n; i++) {
    int p = 0;
    for (size_t j = 0; j < n; j++) {
        if(t[j]<t[i]) p++;
    }
    r[p]=t[i];
  }
  return r;
}
/*!
 * pour calculer un indice local à partir d'un indice global
 * /param n la taille totale du tableau
 * /param i_global l'indice dans le tableau complet
 * /return i_local l'indice du tableau en local
 */
int global_to_local(int n, int i_global)
{

  return 0;
}

/*!
 * pour calculer un indice global à partir d'un indice local
 * /param n la taille totale du tableau
 * /param i_local l'indice local initial
 * /return i_global l'indice de l'élément correspondant dans le tableau global
 */
int local_to_global(int n, int i_local)
{
  return i_local;
}

/*!
 * pour calculer le numéro du processeur destinataire à partir d'un indice dans le tableau total
 * /param n la taille totale du tableau
 * /param i_global l'indice dans le tableau global
 * /return pid_dest le numéro du processeur propriétaire de l'élément
 */
int pid_dest(int n, int i_global)
{
  int pid, nprocs;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;




      return   0;
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
/*!
 * Petite fonction pour etre sûr de générer un tableau dont tous les éléments sont distincts
 * \param n la taille du tableau total
 * \param tab le tableau à construire
 * \param graine pour initialiser le processus de génération aléatoire de nombres
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

  int *displs;
  int *send_counts;

  displs= new int [nprocs];
  send_counts= new int [nprocs];
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
  //--
  //MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(tab_global, n, MPI_INT, root, MPI_COMM_WORLD);
    //--
  MPI_Barrier(MPI_COMM_WORLD);
/*--
    cout << "Mon pid est : " << pid << " et mon tableau global  est ";
    for (size_t i = 0; i < n; i++)
    cout << tab_global[i] << " ";
    cout << endl;
*/
//ENVOYER un tab [2] indice val

  tab_local= new int [send_counts[pid]];
  MPI_Scatterv(tab_global, send_counts, displs, MPI_INT,tab_local , send_counts[pid], MPI_INT, root, MPI_COMM_WORLD);

  cout << "Mon pid est : " << pid << " et le tableau local   est ";
  for (size_t i = 0; i < send_counts[pid]; i++)
  cout << tab_local[i] << " ";
  cout << endl;




  int* tab_idx_val = new int [send_counts[pid] * 2];
// tri induction here
  int p;

  for (size_t i = 0; i < send_counts[pid]; i++) {
    p=0;

    for (size_t j = 0; j < n; j++) {
      if(tab_global[j]<tab_local[i])
        p++;

    }
  //0 1 2 3
  //0 2 4 6
  //1 3 5 7
    tab_idx_val[2*i]=p;
    tab_idx_val[2*i +1]=tab_local[i];


  }

  // On parcourt le tab_idx_val pour determiner :
  // - combien de recv on va executer  ()
  // - combien de sends :
  //                        - a qui
  int nb_recv=send_counts[pid];
  for (size_t i = 0; i < send_counts[pid]; i++) {
    if(tab_idx_val[2*i] >= displ[pid]  && tab_idx_val[2*i] < displ[pid]+send_counts[pid]){
      // l'indice final apparient à pid
      nb_recv--;


    }else{

      int p_dest_id;
      for (size_t i = 0; i < nprocs; i++) {
        if (displ[i] <){
          p_dest_id++;
        }

      }
      int *buffer= new int[2];
      tab_idx_val[2*i]
      // on send
    }
    tab_idx_val[2*i +1]=tab_local[i];

  }

  cout << "pid i"<<pid<< " [";
  for (size_t i = 0; i <send_counts[pid] ; i++)
  cout << tab_idx_val[2*i] << " ";
  cout <<"]" <<endl;
/*
  cout << "pid "<<pid<< " [";
  for (size_t i = 0; i <send_counts[pid] ; i++)
  cout << tab_idx_val[2*i +1] << " ";
  cout <<"]" <<endl;
*/
  //--
  MPI_Barrier(MPI_COMM_WORLD);
  if (pid==root){

    int *tri_seq=tri_insert(n,tab_global);
    cout << "Le tableau trié est ";
    for (size_t i = 0; i < n; i++)
    cout << tri_seq[i] << " ";
    cout << endl;
  }



  MPI_Finalize() ;
  return 0 ;
}
