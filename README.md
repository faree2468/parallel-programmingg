# parallel-programmingg

## Assignment 7

```c++
long ibegin = ncells * rank / nprocs;
long iend = ncells * (rank + 1) / nprocs;
int nsize = (int)(iend-ibegin);
```
This part is from the book. It computes the size of the array on every process


```c++
MPI_Allgather(&nsize, 1, MPI_INT, nsizes, 1, MPI_INT, comm);
```
We call MPI_Allgather function that takes:
- send buffer (the data)
- send count (number of elements in send buffer)
- datatype of elements in send buffer
- receive buffer
- receive count (number of elements received from each process)
- comm (the communicator containing the group with all the processes involved)

And what the function does is it collects value from every process and sends the entire collection to every process



```c++
double *a_local = (double *)malloc(nsize * sizeof(double));
```
We allocate *a_local array that we use for each process



```c++
MPI_Scatterv(a_global, nsizes, offsets, MPI_DOUBLE,
                 a_local, nsize, MPI_DOUBLE, 0, comm);
```
Sends a variable amount of data to each process, unlike ```Scatter``` which sends same amount of data to each process



```c++
MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
```

Reduce is used here to collect all the partial sums, combines them with MPI_SUM function and stores the final result on rank 0


Rank 0 only needs to deallocate resources because it's the only process that allocated global array. Other processes never allocated it and only work with their local ones.

