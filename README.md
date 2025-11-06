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

```bash
make # before putting the -fno-trapping-math -fno-math-errno
```

![terminal](https://github.com/faree2468/parallel-programmingg/blob/assignment-5/assets/terminal.png)


```bash
./stream_triad
```

![terminal2](https://github.com/faree2468/parallel-programmingg/blob/assignment-5/assets/terminal2.png)



```bash
likwid-perfctr -C 0 -g FLOPS_DP ./stream_triad
```

We can see from the results that the code was indeed vectorized
```bash
RETIRED_SSE_AVX_FLOPS_ALL 320000006
```
320 milion FLOPS via SSE or AVX however the kernel achieved 1.2 GFLOPS/s (1196 MFLOPS/s) on a single core
Theoretical max is 64 GFLOPS/s which makes this memory bound

![terminal3](https://github.com/faree2468/parallel-programmingg/blob/assignment-5/assets/terminal3.png)



```bash
make # after putting -fno-trapping-math -fno-math-errno
```

![terminal4](https://github.com/faree2468/parallel-programmingg/blob/assignment-5/assets/terminal4.png)

