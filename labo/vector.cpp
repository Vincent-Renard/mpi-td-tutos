
#include <iostream>
#include <mpi.h>
using namespace std;
int* add_vector(int* v1 , int* v2,int n_elements){
  int* v3 =  new int[n_elements];
  for (size_t i = 0; i < n_elements; i++) {
    v3[i]=v1[i]+v2[i];
  }
  return v3;

}

void read_vector(int* v,int size){
  for (size_t i = 0; i < size; i++) {
    cout << v[i] << ' ';

  }
  cout << endl;


}

void feed_vector(int* v1 , int* v2,int n_elements){
  for (size_t i = 0; i < n_elements; i++) {
    v1[i]=i;
    v2[i]=n_elements-i;

  }

}

 int main ( int argc , char **argv ){
   int pid, nprocs;

   MPI_Init (&argc , &argv) ;
   MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
   MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;
//mpicc -o vector vector.cpp && mpirun --oversubscribe 4 vector && ./vector && rm vector
//mpicc -c vector.c && mpicc -o vector vector.o && mpirun --oversubscribe -np 4 ./vector && rm vector.o && rm vector
  int n=10;
  int* a = new int[n];
  int* b =  new int[n];

  feed_vector(a,b,n);
  read_vector(a,n);
  read_vector(b,n);
  int* c = add_vector(a,b,n);
  read_vector(c,n);
  delete[] a;
  delete[] b;
  delete[] c;

  return 0;
}
