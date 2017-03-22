Jacobi2D:
Number of threads = 1, N=24, max_it=1000 , time elapsed (second) = 0.004192
Number of threads = 8, N=24, max_it=1000 , time elapsed (second) = 0.087772


Number of threads = 1, N=1000, max_it=1000 , time elapsed (second) = 5.138052
Number of threads = 8, N=1000, max_it=1000 , time elapsed (second) = 1.629359

GS2D:
Number of threads = 1, N=24, max_it=1000 , time elapsed (second) = 0.000671
Number of threads = 8, N=24, max_it=1000 , time elapsed (second) = 0.099269


Number of threads = 1, N=1000, max_it=1000 , time elapsed (second) = 1.903910
Number of threads = 8, N=1000, max_it=1000 , time elapsed (second) = 1.268336

As we can see Jacobi2D, when the number of discretization points (N) is small is faster with 1-thread vs. 8-thread.
However, when we increase N to 1000, the 8-thread version is much faster than 1-thread version.

Similarly, in the case of GS2D, for small N (N=24) 1-thread version is faster than the 8-thread version. When we increase N to 1000, the 8-thread version only by a narrow margin bits the 1-thread version.



I ran my code on my Mac OS X laptop which is a four core Intel processor and has 8GB of RAM. My gcc version is gcc-6.
