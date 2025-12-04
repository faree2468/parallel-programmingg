# parallel-programmingg

My machine has a dedicated GPU (Nvidia RTX 2060, 6GB GDDR6 VRAM) however the processor (AMD Ryzen 5 5600x) does not have an integrated GPU

![cuda](https://github.com/faree2468/parallel-programmingg/blob/assignment-8/assets/cuda.png)

On this image we can see CUDA running on RTX 2060
-Driver: 13000 means it's CUDA 13. driver version

-Each array is ~268mb and since the precision is 8 bytes per element (double), we can calculate total number of elements; 268,4/8 ~ 33,5 million elements

-There are 120 blocks and 1024 threads per block meaning it's 120x1024 = 122,800 threads

-Initialization bandwidth is 13078 MB/s

-Read bandwidth is 285362 MB/s which is global memory read bandwidth (Theoretical is 336 GB/s which means it's 85% of peak)

STREAM Kernels:

-Copy	298,431 MB/s	0.00180 s

-Mul	297,414 MB/s	0.00181 s

-Add	304,252 MB/s	0.00203 s

-Triad	304,302 MB/s	0.00265 s

-Dot	313,361 MB/s	0.00171 s



![ocl](https://github.com/faree2468/parallel-programmingg/blob/assignment-8/assets/ocl.png)

It's pretty much similar results for OCL like cuda except it's maybe 2 to 3% faster because of some nvidia optimization paths, lower dispatch overhead and better compiling scheduling

![omp](https://github.com/faree2468/parallel-programmingg/blob/assignment-8/assets/omp.png)

These are OpenMP results which are 10-15x slower than GPU because they run on the CPU

-GPU Memory bandwidth is higher (336GB/s GDDR6 --- 20/40GB/s DDR4)

-CUDA/OpenCL run tens of thousands of threads, hiding memory latency, OpenMP runs only 8-12 CPU threads limited by CPU cores

-GPUs are optimized for throughput