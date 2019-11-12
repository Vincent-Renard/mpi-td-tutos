/* *********************************************
Pour plus d'informations sur les threads c++11
http://en.cppreference.com/w/cpp/thread/thread
et en français mal traduit :
http://fr.cppreference.com/w/cpp/thread/thread
***********************************************/

#include <random>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <omp.h>
using namespace std;

/*
   Mesure et affiche le temps d'exécution d'une fonction.
   Pour mesurer le temps de calcul de l'appel f(args) :
   BENCHMARK(f(args));
*/
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



/* addition séquentielle de deux vecteurs */
void vecadd (int size, float* v1, float* v2, float* v3){
  for (int i = 0; i < size; i++){
    v3[i] = v1[i] + v2[i];
  }
}



/* addition parallèle (TD n°4) */
void vecadd_parallel(int size, float* v1, float* v2, float* v3, int nthreads){
  thread th[nthreads];
  int q ,r ,start, len;
  q=size/nthreads;
  r=size%nthreads;

  for (size_t i = 0; i < nthreads; i++) {
    // le th i commence a
    start = i<r ? (q+1)*i :q*i + r;
    //sur
    len = i<r ? q+1 :q; // nb elements
    th[i]=thread(vecadd,len,v1+start,v2+start,v3+start);
  }
  for (size_t i = 0; i < nthreads; i++) {
    th[i].join();
  }
}

/* addition parallèle avec openmp (à compléter) */
void vecadd_omp (int size, float* v1, float* v2, float* v3, int nthreads){
  omp_set_num_threads(nthreads);

  #pragma parallel for
  for (int i = 0; i < size; i++){
    v3[i] = v1[i] + v2[i];
  }
}


// à compléter : vérification que c1 et c2 sont les mêmes
void verif(int size, float* c1, float* c2){
  bool finpropre=1;
  for (size_t i = 0; i < size; i++) {
    if (c1[i] !=c2[i] ){
      cout<<"Erreur de valeur "<<i<<endl;
      finpropre=0;
      break;
    }

  }
  if (finpropre)
    cout<<"OK "<<endl;

}

int main(int argc, char* argv[]){
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0, 256);
  if (argc < 3)
    {
      cout << "Mauvaise utilisation du programme :\n " <<
	"./VecAdd [taille du tableau] [nombre de threads]" << endl;
      return 1;
    }
  int size = atoi(argv[1]);
  int nthreads = atoi(argv[2]);

  float * A = new float[size];
  float * B = new float[size];
  float * Cseq = new float[size];

  float * Cpar = new float[size];



  #pragma omp parallel for
  for (int i = 0; i < size ; i++){
    A[i] = dis(gen);
    B[i] = dis(gen);
  }

  BENCHMARK(vecadd(size, A, B, Cseq));
  BENCHMARK(vecadd_parallel(size, A, B, Cpar, nthreads));

  verif(size,Cseq,Cpar);


  BENCHMARK(vecadd_omp(size, A, B, Cpar, nthreads));
  verif(size,Cseq, Cpar);



  delete [] A;
  delete [] B;
  delete [] Cseq;
  delete [] Cpar;

  return 0;
}
