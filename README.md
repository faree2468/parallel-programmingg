# parallel-programmingg

![cudaw11](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/cudaweek11.png)

Here we can see that the GPU is able to process the StreamTriad operation extremely quickly once data is on the device

However the data transfer time is 0.15 ms which is significantly larger than the processing time because memory movement

is much slower than computation on the GPU (over 20x slower)

Performance is limited by the PCIe bandwidth, not by GPU compute or device memory speed


![oclw11](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/oclweek11.png)

OpenCL is reporting a ridiculously low kernel time (29 nanoseconds) because of this line:
```c
    clEnqueueBarrier(command_queue);
```
it does not guarantee kernel completion, it only ensures ordering between commands. This is kernel submit time not execution time.

Data transfer time is pretty much the same


![openaccw11](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/openaccweek11.png)

OpenACC is much slower because it's a high level model, unlike CUDA/OpenCL you are not the one who manages memory transfers.

OpenACC automatically does it between the host and GPU when entering and exiting the accelerated region


![ompw11err](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ompweek11error.png)

GCC on Arch is not configured with NVIDIA OpenMP offloading that's why it doesn't work (the flags are invalid)

![ompw11](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ompweek11.png)

This is CPU Only OpenMP (I've removed flags -foffload=nvptx-none, -foffload-lm and -qthreaded)


# Running OpenACC on a different machine through ssh
![openacc1](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh1.png)

![openacc2](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh2.png)

![openacc3](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh3.png)

![openacc4](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh4.png)

![openacc5](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh5.png)

![openacc6](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh6.png)

![openacc7](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh7.png)

![openacc8](https://github.com/faree2468/parallel-programmingg/blob/assignment-9/assets/ssh8.png)

In the first `StreamTriad_kern1.c`, on every iteration data gets copied which is bad and severely bottlenecks gpu

In the second `StreamTriad_kern2.c`, we use ```restrict``` to tell the compiler that variables do not point to overlapping regions of memory

In the third `StreamTriad_kern3.c`, data is allocated on the gpu and we start the dynamic data region after memory allocation and end the dynamic data region
before memory deallocation



