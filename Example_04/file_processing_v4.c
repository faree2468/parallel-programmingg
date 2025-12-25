#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define HALO_WIDTH 2

double **allocate_2d_array(int rows, int cols) {
    double *data = (double*)malloc(rows * cols * sizeof(double));
    double **array = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        array[i] = &data[i * cols];
    }
    return array;
}

void free_2d_array(double **array) {
    free(array[0]);
    free(array);
}

void initialize_grid_with_halos(double **grid, int local_ny, int local_nx, int ng, int rank) {
    int total_ny = local_ny + 2 * ng;
    int total_nx = local_nx + 2 * ng;
    
    for (int i = 0; i < total_ny; i++) {
        for (int j = 0; j < total_nx; j++) {
            if (i < ng || i >= local_ny + ng || j < ng || j >= local_nx + ng) {
                grid[i][j] = -999.0;
            } else {
                grid[i][j] = rank * 1000.0 + (i - ng) * 100.0 + (j - ng);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    if (rank == 0) {
        printf("=== VERSION 4: MPI-IO WITH DATATYPES (HALO CELLS) ===\n");
        printf("Processes: %d\n\n", nprocs);
    }
    
    int ng = HALO_WIDTH;
    int local_ny = 20;
    int local_nx = 20;
    
    int nprocs_y = 1;
    int nprocs_x = nprocs;
    
    int rank_y = rank / nprocs_x;
    int rank_x = rank % nprocs_x;
    
    int global_ny = nprocs_y * local_ny;
    int global_nx = nprocs_x * local_nx;
    
    if (rank == 0) {
        printf("Grid configuration:\n");
        printf("  Local grid per process: %dx%d\n", local_ny, local_nx);
        printf("  Halo width: %d\n", ng);
        printf("  Memory per process: %dx%d (with halos)\n", 
               local_ny + 2*ng, local_nx + 2*ng);
        printf("  Global grid: %dx%d\n", global_ny, global_nx);
        printf("  Process layout: %dx%d\n\n", nprocs_y, nprocs_x);
    }
    
    double **grid = allocate_2d_array(local_ny + 2*ng, local_nx + 2*ng);
    
    double start_init = MPI_Wtime();
    initialize_grid_with_halos(grid, local_ny, local_nx, ng, rank);
    double end_init = MPI_Wtime();
    
    if (rank == 0) {
        printf("Initialized grid with halos\n");
        printf("  Sample process 0 corner (with halos):\n");
        for (int i = 0; i < 5; i++) {
            printf("    ");
            for (int j = 0; j < 5; j++) {
                printf("%7.1f ", grid[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    double start_write = MPI_Wtime();
    
    MPI_Datatype memspace, filespace;
    
    int local_sizes[2] = {local_ny + 2*ng, local_nx + 2*ng};
    int local_subsizes[2] = {local_ny, local_nx};
    int local_starts[2] = {ng, ng};
    
    MPI_Type_create_subarray(2, local_sizes, local_subsizes, local_starts,
                             MPI_ORDER_C, MPI_DOUBLE, &memspace);
    MPI_Type_commit(&memspace);
    
    int global_sizes[2] = {global_ny, global_nx};
    int global_subsizes[2] = {local_ny, local_nx};
    int global_starts[2] = {rank_y * local_ny, rank_x * local_nx};
    
    MPI_Type_create_subarray(2, global_sizes, global_subsizes, global_starts,
                             MPI_ORDER_C, MPI_DOUBLE, &filespace);
    MPI_Type_commit(&filespace);
    
    MPI_File fh;
    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "collective_buffering", "true");
    
    MPI_File_open(MPI_COMM_WORLD, "output_with_halos.dat",
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  info, &fh);
    
    MPI_Offset file_size = global_ny * global_nx * sizeof(double);
    MPI_File_set_size(fh, file_size);
    
    MPI_File_set_view(fh, 0, MPI_DOUBLE, filespace, "native", MPI_INFO_NULL);
    
    MPI_File_write_all(fh, &grid[0][0], 1, memspace, MPI_STATUS_IGNORE);
    
    MPI_File_close(&fh);
    MPI_Info_free(&info);
    
    MPI_Type_free(&memspace);
    MPI_Type_free(&filespace);
    
    double end_write = MPI_Wtime();
    
    double init_time = end_init - start_init;
    double write_time = end_write - start_write;
    
    double max_init, max_write;
    MPI_Reduce(&init_time, &max_init, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&write_time, &max_write, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double data_mb = (global_ny * global_nx * sizeof(double)) / (1024.0 * 1024.0);
        double memory_per_proc = (local_ny + 2*ng) * (local_nx + 2*ng) * sizeof(double) / (1024.0 * 1024.0);
        
        printf("TIMING RESULTS:\n");
        printf("  Initialization: %.4f seconds\n", max_init);
        printf("  Write time:     %.4f seconds\n", max_write);
        printf("  Bandwidth:      %.2f MB/s\n\n", data_mb / max_write);
        
        printf("MEMORY EFFICIENCY:\n");
        printf("  Memory per process: %.2f MB (includes halos)\n", memory_per_proc);
        printf("  Data written: %.2f MB (halos stripped)\n", data_mb);
        printf("  Memory overhead: %.1f%% (halo cells)\n", 
               ((memory_per_proc * nprocs) / data_mb - 1.0) * 100.0);
        printf("\n");
        
        printf("KEY FEATURES:\n");
        printf("  + MPI_Type_create_subarray defines data layout\n");
        printf("  + Memspace: describes data in memory (with halos)\n");
        printf("  + Filespace: describes data in file (without halos)\n");
        printf("  + Automatic extraction of real data\n");
        printf("  + No manual copying needed\n");
        printf("  + Typical pattern in scientific computing\n\n");
    }
    
    if (rank == 0) {
        printf("Verifying output file...\n");
        FILE *fp = fopen("output_with_halos.dat", "rb");
        double *verify_data = (double*)malloc(global_nx * sizeof(double));
        
        printf("  First row of file (should have NO halo values):\n    ");
        fread(verify_data, sizeof(double), global_nx, fp);
        for (int j = 0; j < (global_nx < 10 ? global_nx : 10); j++) {
            printf("%.1f ", verify_data[j]);
        }
        printf("\n\n");
        
        free(verify_data);
        fclose(fp);
    }
    
    free_2d_array(grid);
    
    MPI_Finalize();
    return 0;
}