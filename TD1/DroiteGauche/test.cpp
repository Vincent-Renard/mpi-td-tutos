#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main ( int argc , char **argv )
{
int nlocal=10;
  int* tablocal = new int[nlocal];



  for (int i=0; i<nlocal; i++){
    tablocal[i] = i;
  }


  for (int i=0; i<nlocal; i++){
      cout << tablocal[i] << " ";
  }
  cout << endl;

  return 0 ;
}
