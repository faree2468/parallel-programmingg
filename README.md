# parallel-programmingg

## Assignment 5

I've taken 3 same timestep functions from official repo with different #pragma

timestep_opt1.c:
```c
#pragma omp simd reduction(min:mymindt)
```
This tells the compiler to vectorize the loop, to execute multiple iterations in parallel using SIMD


timestep_opt2.c:
```c
#pragma omp simd private(wavespeed, xspeed, yspeed, dt) reduction(min:mymindt)
```
This also tells the compiler to vectorize the loop however here each vector element gets its own private copy of the variables during the loop.
This prevents data races


timestep_opt3.c:
```c
#pragma omp simd reduction(min:mymindt)
```
It has the same pragma as the 1st one however the difference is that the variables are declared inside the loop which means, by C rules that they have automatic
storage duration, each loop iteration already gets its own copy

