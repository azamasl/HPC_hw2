/**
The array size (1048*1048) is too large to fit into the stack. I first tried to increase my stack size (ulimit -s hard) but that wasn't enough. Since all global and static variables are stored in the data segment of the memory, by defining a[N][N] outside of the scope of main function the issue was resolved. Furthermore, I defined array a to be shared across the threads. 
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1048
double a[N][N];

int main (int argc, char *argv[]) 
{
int nthreads, tid, i, j;


/* Fork a team of threads with explicit variable scoping */
#pragma omp parallel shared(nthreads,a) private(i,j,tid)
  {

  /* Obtain/print thread info */
  tid = omp_get_thread_num();
  if (tid == 0) 
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n", tid);

  /* Each thread works on its own private copy of the array */
  #pragma omp for schedule(dynamic)
  for (i=0; i<N; i++){
    for (j=0; j<N; j++)
      a[i][j] = tid + i + j;
  }
  /* For confirmation */
  printf("Thread %d done. Last element= %f\n",tid,a[N-1][N-1]);

  }  /* All threads join master thread and disband */

}

