#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstring>
#include <omp.h>

#include <chrono>

# define N_NODES 8

using namespace std;

#define BENCHMARK(fun)							\
do { chrono::time_point<chrono::system_clock> start, end;		\
  start = chrono::system_clock::now();				\
  fun;								\
  end = chrono::system_clock::now();					\
  chrono::duration<double> elapsed_seconds = end-start;		\
  time_t end_time = chrono::system_clock::to_time_t(end);		\
  cout << #fun 							\
  << " elapsed time: " << elapsed_seconds.count() << "s\n";	\
} while (0)





int fibo_seq(int n){
  if (n==1||n==2) return 1;

  return fibo_seq(n-2) + fibo_seq(n-1);
}

int fibo_section(int n ){
  //omp_set_num_threads(N_NODES);
  int x,y;
  #pragma omp parallel sections num_threads(N_NODES/2)
  {
    #pragma omp section
    {
      y=fibo_seq(n-2);
    }
    #pragma omp section
    {
      x= fibo_seq(n-1);

    }

  }

  return y+x;
}

int fibo_iter(int n ){
  int t [n+1];
  t[0] = 0; t[1]=1;
  for (size_t i = 2; i <= n; i++) {
    t[i]=t[i-1]+t[i-2];
  }
  return t[n];
}

int fibo_tasks(int n ){
  //omp_set_num_threads(N_NODES);
  int x,y,q;
  #pragma omp single
  {
    #pragma omp task shared(x)
    {
      x= fibo_seq(n-1);
    }
    #pragma omp task shared(y)
    {
      y=fibo_seq(n-2);
    }

    #pragma omp taskwait
    {
       q = y+x;

    }


  }
  return q;


}

int main ( int argc, char *argv[] ){




  cout << "FIBO" << endl;

  int a = atoi(argv[1]);
  BENCHMARK(fibo_iter(a));
  BENCHMARK(fibo_seq(a));
  BENCHMARK(fibo_section(a));
  BENCHMARK(fibo_tasks(a));
  cout << "FIBO ITER " <<  fibo_iter(a) << endl;

  cout << "FIBO SEQ " <<  fibo_seq(a) << endl;

  cout << "FIBO SECTIONS " << fibo_section(a) << endl;

    cout << "FIBO SECTIONS " << fibo_tasks(a) << endl;



  return 0;
}
