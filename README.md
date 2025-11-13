# parallel-programmingg

First thing I've done is run make
```bash
    make
```

Then I've got results for the ```stencil_opt2.c```:
![terminal1](https://github.com/faree2468/parallel-programmingg/blob/assignment-6/assets/w7a61.png)

After that I've changed make file sources to ```stencil_opt4.c```:
```makefile
    # Stencil source files (main programs)
    SOURCES = stencil_opt4.c
```
Results for ```stencil_opt4.c```:
![terminal2](https://github.com/faree2468/parallel-programmingg/blob/assignment-6/assets/w7a62.png)

And lastly I've done the same thing for the ```stencil_opt6.c```:
![terminal3](https://github.com/faree2468/parallel-programmingg/blob/assignment-6/assets/w7a63.png)

My CPU used 12 threads to execute the code.

In the first stencil, it is Loop Level OpenMP however there are issues because it recreates OpenMP on every iteration, too many implicit barriers as well.

In the second stencil, it is still Loop Level OpenMP however it's much better because it avoids thousands of implicit barriers, reuses parallel regions.

We use things like:
```c
#pragma omp parallel // all iterations happen under this pragma
#pragma omp for nowait // removes barrier between end of the flush loop and the start of the stencil
```
In the third stencil, it is High Level OpenMP where we are computing upper and lower bounds for each thread
```c
#pragma omp for // it replaces this
```
Each thread initializes only its part of the matrix
```c
for (int j = jltb0; j < jutb0; j++)
```
Manual flush partition, dividing the flush array
```c
int ifltb = (jmax*imax*4) * thread_id / nthreads;
int ifutb = (jmax*imax*4) * (thread_id+1) / nthreads;
```
It uses explicit barriers
```c
#pragma omp barrier
```


Implicit barrier means that all threads must reach certain point before any thread can continue


Explicit barrier forces all threads to synchronize where the pragma is written

