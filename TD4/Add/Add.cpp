#include <iostream>
#include <chrono>
#include <ctime>
#include <mutex>
#include <thread>
using namespace std;

int somme_globale=0;

mutex mtx;


/* à compléter : addition en anneau */
void addition_ring  (int numthread ,int nthreads){
  int a = rand()%numthread*nthreads;

mtx.unlock();
  somme_globale+=a;
  cout<<"th : "<<numthread<<" a: "<<a<<" sg = "<<somme_globale<<endl;
  mtx.lock();

}



int main(int argc, char* argv[]){
  if (argc < 2)
    {
      cout << "Mauvaise utilisation du programme :\n " <<
	"./Addring [nombre de threads]" << endl;
      return 1;
    }
  int nthreads = atoi(argv[1]);
  somme_globale=0;
  thread thd [nthreads];

  mtx.lock();
  for (size_t thread_i = 0; thread_i < nthreads; thread_i++) {
    thd[thread_i]=thread(addition_ring,thread_i,nthreads);
  }

  for (size_t thread_i = 0; thread_i < nthreads; thread_i++) {
    thd[thread_i].join();
  }
  cout<<"somme_globale = " <<somme_globale<<endl;
  /* à compléter

   */



  return 0;
}
