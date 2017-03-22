/* Jacobi smoothing to solve -u''=f
 * Global vector has N inner unknowns.
 * Author: Georg Stadler
 */
#include <stdio.h>
#include <math.h>
#include "util.h"
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

/* compuate global residual, assuming ghost values are updated */
double compute_residual(double* u, int N, double invhsq)
{
  int i,j;
  double tmp, res = 0.0;
#pragma omp parallel for default(none) shared(u,N,invhsq) private(i,j,tmp) reduction(+:res)
  for (i = 1; i <= N; i++){
      for(j = 1; j <= N; ++j){
	tmp = (4.0*u[i*(N+2)+j] -u[(i-1)*(N+2)+j] -u[i*(N+2)+j-1] -u[(i+1)*(N+2)+j] - u[i*(N+2)+j+1])*invhsq -1;
	res += tmp * tmp;
      }
  }
  return sqrt(res);
}


int main(int argc, char * argv[])
{
  int i,j, N, iter, max_iters;
  
  sscanf(argv[1], "%d", &N);
  sscanf(argv[2], "%d", &max_iters);
# pragma omp parallel
  {
#ifdef _OPENMP
    int my_threadnum = omp_get_thread_num();
    int numthreads = omp_get_num_threads();
#else
    int my_threadnum = 0;
    int numthreads = 1;
#endif
    printf("Hello, I'm thread %d out of %d\n", my_threadnum, numthreads);
  }

  /* timing */
  timestamp_type time1, time2;
  get_timestamp(&time1);

  /* Allocation of matrices, including left and right ghost points */
  double *u= (double *) calloc(sizeof(double), (N+2) * (N+2));

  //Taking care of the boundary points
  for(int i = 0; i < N+2; ++i){
    u[i*(N+2)]=0.0;//j=0
    u[i*(N+2)+N+1]=0.0;//j=N+1
  }
  for(int j = 0; j < N+2; ++j){
    u[j]=0.0;//i=0
    u[(N+1)*(N+2)+j]=0.0;//i=N+1
  }
  
  double h = 1.0 / (N + 1);
  double hsq = h * h;
  double invhsq = 1./hsq;
  double res, res0, tol = 1e-4;

  /* initial residual */
  res0 = compute_residual(u, N, invhsq);
  res = res0;


  for (iter = 0; iter < max_iters && res/res0 > tol; iter++) {

#pragma omp parallel for default(none) private(j) shared(N,u,hsq)
    /* GS step for all the inner points */
    //Red sweep   
    for (i = 1; i <= N; i++){
      if(i % 2==1){
          for (j = 1; j <= N; j+=2){
	    u[i*(N+2)+j]  = 0.25 * (hsq + u[(i-1)*(N+2)+j] + u[i*(N+2)+j-1] +u[(i+1)*(N+2)+j] + u[i*(N+2)+j+1]);
	  }
      }else{
          for (j = 2; j <= N; j+=2){
	    u[i*(N+2)+j]  = 0.25 * (hsq + u[(i-1)*(N+2)+j] + u[i*(N+2)+j-1]+u[(i+1)*(N+2)+j] + u[i*(N+2)+j+1]);
	  }
      }
    }

#pragma omp parallel for default(none) private(j) shared(N,u,hsq)
    /* GS step for all the inner points */
    //Black sweep  
    for (i = 1; i <= N; i++){
      if(i % 2==1){
          for (j = 2; j <= N; j+=2){
	    u[i*(N+2)+j]  = 0.25 * (hsq + u[(i-1)*(N+2)+j] + u[i*(N+2)+j-1] +u[(i+1)*(N+2)+j] + u[i*(N+2)+j+1]);
	  }
      }else{
          for (j = 1; j <= N; j+=2){
	    u[i*(N+2)+j]  = 0.25 * (hsq + u[(i-1)*(N+2)+j] + u[i*(N+2)+j-1]+u[(i+1)*(N+2)+j] + u[i*(N+2)+j+1]);
	  }
      }
    }

    
    if (0 == (iter % 10)) {
      res = compute_residual(u, N, invhsq);
      printf("Iter %d: Residual: %g\n", iter, res);
    }
  }

  /* Clean up */
  free(u);

  /* timing */
  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  printf("Time elapsed is %f seconds.\n", elapsed);
  return 0;
}
