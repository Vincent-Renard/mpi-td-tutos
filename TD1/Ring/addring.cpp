#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int addition_ring(int a, int root)
{
  int pid, nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid );
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs );
  MPI_Status status;

  int pid_envoi = (pid+1)%nprocs;
  int pid_recu = ((pid-1)+nprocs)%nprocs;
  int a_local=0;
  if (pid==root){
    MPI_Send(&a,1,MPI_INT,(pid+1)%nprocs,34,MPI_COMM_WORLD);


  }
  else {

    MPI_Recv(&a_local,1,MPI_INT,((pid-1)+nprocs)%nprocs,34,MPI_COMM_WORLD,&status);
 a_local+=a;
    MPI_Send(&a_local,1,MPI_INT,(pid+1)%nprocs,34,MPI_COMM_WORLD);
  }

  if (pid==root){
    MPI_Recv(&a_local,1,MPI_INT,((pid-1)+nprocs)%nprocs,34,MPI_COMM_WORLD,&status);
    return a_local;

  }



}

int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Status status;
  MPI_Init (&argc , &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);

  int root = atoi(argv[1]);

  srand(time(NULL)+pid);
  int a = rand()%10;

  cout << "je suis " << pid << " et la valeur est " << a << endl;

  int somme = addition_ring(a,root);
  if (pid==root)
    cout << "je suis " << pid << " et la somme est " << somme << endl;


  MPI_Finalize() ;
  return 0 ;
}
