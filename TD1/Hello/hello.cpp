#include <iostream>
#include <mpi.h>

using namespace std;

void af(int* in,int size){
  for (int i=0; i<size; i++){
      cout << in[i] << " ";
  }
  cout << endl;
}
int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
  cout << "I am " << pid << " of " << nprocs << " processors" << endl;
  





  MPI_Finalize( ) ;
  return 0 ;
}
