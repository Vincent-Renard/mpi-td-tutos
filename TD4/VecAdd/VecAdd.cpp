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



/* addition sequentielle de deux vecteurs */
void vecadd (int size, float* v1, float* v2, float* v3){
  for (int i = 0; i < size; i++){
    v3[i] = v1[i] + v2[i];
  }
}



// à compléter : addition parallèle
void vecadd_parallel(int size, float* v1, float* v2, float* v3, int nthreads){
  
}

// à compléter : vérification que c1 et c2 sont les mêmes
void verif(float* c1, float* c2, int n){

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
  
  for (int i = 0; i < size ; i++){
    A[i] = dis(gen);
    B[i] = dis(gen);
  }
  
  
  BENCHMARK(vecadd(size, A, B, Cseq));

  verif(Cseq, Cpar,size);
  
  
  delete [] A;
  delete [] B;
  delete [] Cseq;
  delete [] Cpar;
  
  return 0;
}
