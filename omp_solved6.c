/**
The error was that reduction variable 'sum' is private in outer context. However, 'sum' should be shared in the parallel region. Note that 'sum' was defined inside the function dotprod and as a result it was private. There was another declaration of 'sum' in the main, which was redundant. To fix the issue, I removed the "float sum;" from both dotprod and main functions and defined it globally.
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN];
float sum;

float dotprod ()
{
int i,tid;


tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
  for (i=0; i < VECLEN; i++)
    {
    sum = sum + (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }
}
int main (int argc, char *argv[]) {
int i;

for (i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;
sum = 0.0;

#pragma omp parallel
  dotprod();

printf("Sum = %f\n",sum);

}

