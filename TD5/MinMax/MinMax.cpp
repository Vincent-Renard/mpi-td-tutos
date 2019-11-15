#include <random>
#include <iostream>
#include <chrono>
#include <ctime>
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


float min(float* v, int size, int* pos){
  float min;
  int idx;
  if (size > 0){
    min = v[0];
    idx= 0;
    for (int i = 1; i < size; i++)
    //min=(min<v[i])?min:(idx=i,v[i]);
    if (min > v[i]){
      min = v[i];
      idx = i;
    }
  }
  *pos = idx;
  return min;
}

float max(float* v, int size, int* pos){
  float max_value;
  int idx;
  if (size > 0){
    max_value = v[0];
    idx= 0;
    for (int i = 1; i < size; i++)
    //min=(min<v[i])?min:(idx=i,v[i]);
    if (max_value < v[i]){
      max_value = v[i];
      idx = i;
    }
  }
  *pos = idx;
  return max_value;
}

void verif(int pos_min_seq ,float val_min_seq,int pos_max_seq ,float val_max_seq,int pos_min_omp ,float val_min_omp,int pos_max_omp ,float val_max_omp ) {
  bool ok = true;
  if (pos_min_seq != pos_min_omp) {
    cout<<"pos_min KO"<<endl;
    cout<<"pos_min_seq : "<< pos_min_seq<<endl;
    cout<<"pos_min_omp : "<< pos_min_omp<<endl;
    ok = false;
  }
  if (pos_max_seq != pos_max_omp) {
    cout<<"pos_max KO"<<endl;
    cout<<"pos_max_seq : "<< pos_max_seq<<endl;
    cout<<"pos_max_omp : "<< pos_max_omp<<endl;

    ok = false;
  }
  if (val_min_seq != val_min_omp) {
    cout<<"val_min KO"<<endl;
    cout<<"val_min_seq : "<< val_min_seq<<endl;
    cout<<"val_min_omp : "<< val_min_omp<<endl;
    ok = false;
  }
  if (val_max_seq != val_max_omp) {
    cout<<"val_max KO"<<endl;
    cout<<"val_max_seq : "<< val_max_seq<<endl;
    cout<<"val_max_omp : "<< val_max_omp<<endl;
    ok = false;
  }
  if (ok) {
    cout<<"OK"<<endl;
  }
}


// Si nécessaire vous pouvez rajouter un paramètre : le nombre de threads
float omp_max(float* v, int size, int* pos,int nthreads){
  omp_set_num_threads(nthreads);
  int *index= new int[nthreads];
  float *maxs = new float[nthreads];

  float max;
  int idx;
  if (size > 0){
    max = v[0];
    idx= 0;
    #pragma omp parallel
    {
      float value=max;
      int id = idx;
      #pragma omp for
      for (int i = 1; i < size; i++){
        //min=(min<v[i])?min:(idx=i,v[i]);
        if (value < v[i]){
          value = v[i];
          id = i;
        }
      }
      maxs[omp_get_thread_num()]=value;
      index[omp_get_thread_num()]=id;

      #pragma omp critical
      {
        //printf("ID : %d  i = %d  max = %f \n",omp_get_thread_num(),id,value );

        if ( value >= max) {
          if (value == max){
            if (idx>id){
              max=value;
              idx=id;
            }
            }else{
              max=value;
              idx=id;

          }


        }
      }

    }
  }
  /*
  for (size_t i = 0; i < nthreads; i++) {
  if ( max < maxs[i]) {
  max=maxs[i];
  idx=index[i];
}
}
*/
*pos = idx;
return max;

}

// Si nécessaire vous pouvez rajouter un paramètre : le nombre de threads
float omp_min(float* v, int size, int* pos,int nthreads){
  omp_set_num_threads(nthreads);
  int *index= new int[nthreads];
  float *mins = new float[nthreads];

  float min;
  int idx;
  if (size > 0){
    min = v[0];
    idx= 0;
    #pragma omp parallel
    {
      float value=min;
      int id = idx;
      #pragma omp for
      for (int i = 1; i < size; i++){
        //min=(min<v[i])?min:(idx=i,v[i]);
        if (value > v[i]){
          value = v[i];
          id = i;
        }
      }
      mins[omp_get_thread_num()]=value;
      index[omp_get_thread_num()]=id;

      #pragma omp critical
      {
        //printf("ID : %d  i = %d  max = %f \n",omp_get_thread_num(),id,value );

        if ( value <= min) {
          if (value == min){
            if (idx>id){
              min=value;
              idx=id;
            }
            }else{
              min=value;
              idx=id;

          }


        }
      }

    }
  }
  /*
  for (size_t i = 0; i < nthreads; i++) {
  if ( min > mins[i]) {
  min=mins[i];
  idx=index[i];
}
}
*/
*pos = idx;
return min;

}


float random(float max){
  return (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/max)));
}

int main(int argc, char* argv[]){
  srand (time(0));
  if (argc < 3)
  {
    cout << "Mauvaise utilisation du programme :\n " <<
    "./VecAdd [taille du tableau] [nombre de threads]" << endl;
    return 1;
  }

  int size = atoi(argv[1]);
  int nthreads = atoi(argv[2]);

  float* v = new float[size];

  #pragma omp parallel for
  for (int i = 0; i < size ; i++){
    v[i] = random(size/2);
  }

  /*
  for (int i = 0; i < size ; i++){
  cout<<" "<<  v[i];
}
cout<<endl;
*/
int pos1;
float mn1 = min(v,size,&pos1);
int pos2;
float mn2 = omp_min(v,size,&pos2,nthreads);

int pos3;
float mx1 = max(v,size,&pos3);

int pos4;
float mx2 = omp_max(v,size,&pos4,nthreads);


verif(pos1,mn1,pos3,mx1,pos2,mn2,pos4,mx2);

BENCHMARK(min(v,size,&pos1));
BENCHMARK(omp_min(v,size,&pos2,nthreads));
BENCHMARK(max(v,size,&pos3));
BENCHMARK(omp_max(v,size,&pos4,nthreads));
delete [] v;
}
