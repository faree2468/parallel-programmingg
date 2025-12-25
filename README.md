# parallel-programmingg

![img1](https://github.com/faree2468/parallel-programmingg/blob/assignment-11/assets/week12a11.png)

I've first run ```file_generator.c```


![img2](https://github.com/faree2468/parallel-programmingg/blob/assignment-11/assets/week12a11_1.png)

Example1, ```file_processing_v1.c``` Serial I/O without optimizations

Rank0 does everything, reads entire CSV, stores all data in memory and scatters chunks to other ranks
After computation Rank0 writes everything using ```fwrite```

![img3](https://github.com/faree2468/parallel-programmingg/blob/assignment-11/assets/week12a11_2.png)

Example2, ```file_processing_v2.c``` MPI-IO Independent Writes

Each rank reads its own portion of the CSV file and writes its own data directly to the output file
It uses ```MPI_File_open``` and ```MPI_File_Write_at```

It's better because it removes Rank0 bottleneck


![img4](https://github.com/faree2468/parallel-programmingg/blob/assignment-11/assets/week12a11_3.png)

Example3, ```file_processing_v3.c``` Collective MPI-IO + Hints

Here instead of ```MPI_File_Write_at``` it's ```MPI_File_Write_all``` performs fewer and larger writes

```MPI_File_set_view``` All ranks write as if file starts at offset 0 and MPI handles displacement internally

```setup_hints()``` using ROMIO hints





