#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>

using namespace std;


/* à compléter : addition en anneau */ 
void addition_ring  (int nthreads){
}

int main(int argc, char* argv[]){
  if (argc < 2)
    {
      cout << "Mauvaise utilisation du programme :\n " << 
	"./Addring [nombre de threads]" << endl;
      return 1;
    }
  int nthreads = atoi(argv[1]);  
  

  /* à compléter

   */
  
  
  
  return 0;
}
