#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_LINE 512

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    double start_total = MPI_Wtime();
    
    if (rank == 0) {
        printf("=== VERSION 2: MPI-IO INDEPENDENT ===\n");
        printf("Processes: %d\n\n", nprocs);
    }
    
    FILE *fp = fopen("../temperature_data.csv", "r");
    if (!fp) {
        fprintf(stderr, "Rank %d: Error opening file\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    char line[MAX_LINE];
    fgets(line, MAX_LINE, fp);
    
    long total_records = 0;
    while (fgets(line, MAX_LINE, fp)) {
        total_records++;
    }
    
    int records_per_proc = total_records / nprocs;
    int my_start = rank * records_per_proc;
    int my_count = (rank == nprocs - 1) ? (total_records - my_start) : records_per_proc;
    
    if (rank == 0) {
        printf("Total records: %ld\n", total_records);
        printf("Records per process: %d\n\n", records_per_proc);
    }
    
    double *local_temp = (double*)malloc(my_count * sizeof(double));
    double *local_humidity = (double*)malloc(my_count * sizeof(double));
    double *local_pressure = (double*)malloc(my_count * sizeof(double));
    
    double start_read = MPI_Wtime();
    
    rewind(fp);
    fgets(line, MAX_LINE, fp);
    
    for (int i = 0; i < my_start; i++) {
        fgets(line, MAX_LINE, fp);
    }
    
    for (int i = 0; i < my_count; i++) {
        int station, day, row, col;
        double temp, hum, press, wind, precip;
        
        if (fscanf(fp, "%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf\n",
                  &station, &day, &row, &col, &temp, &hum, &press, &wind, &precip) != 9) {
            break;
        }
        
        local_temp[i] = temp;
        local_humidity[i] = hum;
        local_pressure[i] = press;
    }
    
    fclose(fp);
    
    double end_read = MPI_Wtime();
    
    double start_compute = MPI_Wtime();
    
    double sum_temp = 0.0, sum_hum = 0.0, sum_press = 0.0;
    for (int i = 0; i < my_count; i++) {
        sum_temp += local_temp[i];
        sum_hum += local_humidity[i];
        sum_press += local_pressure[i];
    }
    
    double mean_temp = sum_temp / my_count;
    double mean_hum = sum_hum / my_count;
    double mean_press = sum_press / my_count;
    
    for (int i = 0; i < my_count; i++) {
        local_temp[i] = local_temp[i] - mean_temp;
        local_humidity[i] = local_humidity[i] - mean_hum;
        local_pressure[i] = local_pressure[i] - mean_press;
    }
    
    double end_compute = MPI_Wtime();
    
    double start_write = MPI_Wtime();
    
    MPI_File fh;
    MPI_Status status;
    
    MPI_File_open(MPI_COMM_WORLD, "output_independent.dat",
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &fh);
    
    if (rank == 0) {
        MPI_File_write(fh, &nprocs, 1, MPI_INT, &status);
        MPI_File_write(fh, &records_per_proc, 1, MPI_INT, &status);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Offset offset_temp = 2 * sizeof(int) + rank * my_count * sizeof(double);
    MPI_Offset offset_hum = 2 * sizeof(int) + nprocs * records_per_proc * sizeof(double) + 
                            rank * my_count * sizeof(double);
    MPI_Offset offset_press = 2 * sizeof(int) + 2 * nprocs * records_per_proc * sizeof(double) + 
                              rank * my_count * sizeof(double);
    
    MPI_File_write_at(fh, offset_temp, local_temp, my_count, MPI_DOUBLE, &status);
    MPI_File_write_at(fh, offset_hum, local_humidity, my_count, MPI_DOUBLE, &status);
    MPI_File_write_at(fh, offset_press, local_pressure, my_count, MPI_DOUBLE, &status);
    
    MPI_File_close(&fh);
    
    double end_write = MPI_Wtime();
    double end_total = MPI_Wtime();
    
    double read_time = end_read - start_read;
    double compute_time = end_compute - start_compute;
    double write_time = end_write - start_write;
    double total_time = end_total - start_total;
    
    double max_read, max_compute, max_write, max_total;
    MPI_Reduce(&read_time, &max_read, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&compute_time, &max_compute, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&write_time, &max_write, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&total_time, &max_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        long total_data = nprocs * records_per_proc;
        double data_mb = (total_data * 3 * sizeof(double)) / (1024.0 * 1024.0);
        
        printf("TIMING RESULTS:\n");
        printf("  Read time:    %.4f seconds\n", max_read);
        printf("  Compute time: %.4f seconds\n", max_compute);
        printf("  Write time:   %.4f seconds\n", max_write);
        printf("  Total time:   %.4f seconds\n\n", max_total);
        
        printf("PERFORMANCE:\n");
        printf("  Data written: %.2f MB\n", data_mb);
        printf("  Write bandwidth: %.2f MB/s\n\n", data_mb / max_write);
    }
    
    free(local_temp);
    free(local_humidity);
    free(local_pressure);
    
    MPI_Finalize();
    return 0;
}