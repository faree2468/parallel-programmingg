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
    
    FILE *fp = NULL;
    long total_records = 0;
    int records_per_proc = 0;
    
    if (rank == 0) {
        printf("=== VERSION 1: SERIAL I/O ===\n");
        printf("Processes: %d\n\n", nprocs);
        
        fp = fopen("../temperature_data.csv", "r");
        if (!fp) {
            fprintf(stderr, "Error opening file\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        char line[MAX_LINE];
        fgets(line, MAX_LINE, fp);
        
        while (fgets(line, MAX_LINE, fp)) {
            total_records++;
        }
        
        records_per_proc = total_records / nprocs;
        printf("Total records: %ld\n", total_records);
        printf("Records per process: %d\n\n", records_per_proc);
    }
    
    MPI_Bcast(&records_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    double *local_temp = (double*)malloc(records_per_proc * sizeof(double));
    double *local_humidity = (double*)malloc(records_per_proc * sizeof(double));
    double *local_pressure = (double*)malloc(records_per_proc * sizeof(double));
    
    double start_read = MPI_Wtime();
    
    if (rank == 0) {
        double *all_temp = (double*)malloc(nprocs * records_per_proc * sizeof(double));
        double *all_humidity = (double*)malloc(nprocs * records_per_proc * sizeof(double));
        double *all_pressure = (double*)malloc(nprocs * records_per_proc * sizeof(double));
        
        rewind(fp);
        char line[MAX_LINE];
        fgets(line, MAX_LINE, fp);
        
        for (int i = 0; i < nprocs * records_per_proc; i++) {
            int station, day, row, col;
            double temp, hum, press, wind, precip;
            
            if (fscanf(fp, "%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf\n",
                      &station, &day, &row, &col, &temp, &hum, &press, &wind, &precip) != 9) {
                break;
            }
            
            all_temp[i] = temp;
            all_humidity[i] = hum;
            all_pressure[i] = press;
        }
        
        fclose(fp);
        
        MPI_Scatter(all_temp, records_per_proc, MPI_DOUBLE,
                    local_temp, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(all_humidity, records_per_proc, MPI_DOUBLE,
                    local_humidity, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(all_pressure, records_per_proc, MPI_DOUBLE,
                    local_pressure, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        
        free(all_temp);
        free(all_humidity);
        free(all_pressure);
    } else {
        MPI_Scatter(NULL, 0, MPI_DOUBLE,
                    local_temp, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, MPI_DOUBLE,
                    local_humidity, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, MPI_DOUBLE,
                    local_pressure, records_per_proc, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
    }
    
    double end_read = MPI_Wtime();
    
    double start_compute = MPI_Wtime();
    
    double sum_temp = 0.0, sum_hum = 0.0, sum_press = 0.0;
    for (int i = 0; i < records_per_proc; i++) {
        sum_temp += local_temp[i];
        sum_hum += local_humidity[i];
        sum_press += local_pressure[i];
    }
    
    double mean_temp = sum_temp / records_per_proc;
    double mean_hum = sum_hum / records_per_proc;
    double mean_press = sum_press / records_per_proc;
    
    for (int i = 0; i < records_per_proc; i++) {
        local_temp[i] = local_temp[i] - mean_temp;
        local_humidity[i] = local_humidity[i] - mean_hum;
        local_pressure[i] = local_pressure[i] - mean_press;
    }
    
    double end_compute = MPI_Wtime();
    
    double start_write = MPI_Wtime();
    
    double *global_temp = NULL;
    double *global_humidity = NULL;
    double *global_pressure = NULL;
    
    if (rank == 0) {
        global_temp = (double*)malloc(nprocs * records_per_proc * sizeof(double));
        global_humidity = (double*)malloc(nprocs * records_per_proc * sizeof(double));
        global_pressure = (double*)malloc(nprocs * records_per_proc * sizeof(double));
    }
    
    MPI_Gather(local_temp, records_per_proc, MPI_DOUBLE,
               global_temp, records_per_proc, MPI_DOUBLE,
               0, MPI_COMM_WORLD);
    MPI_Gather(local_humidity, records_per_proc, MPI_DOUBLE,
               global_humidity, records_per_proc, MPI_DOUBLE,
               0, MPI_COMM_WORLD);
    MPI_Gather(local_pressure, records_per_proc, MPI_DOUBLE,
               global_pressure, records_per_proc, MPI_DOUBLE,
               0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        FILE *out = fopen("output_serial.dat", "wb");
        
        fwrite(&nprocs, sizeof(int), 1, out);
        fwrite(&records_per_proc, sizeof(int), 1, out);
        
        fwrite(global_temp, sizeof(double), nprocs * records_per_proc, out);
        fwrite(global_humidity, sizeof(double), nprocs * records_per_proc, out);
        fwrite(global_pressure, sizeof(double), nprocs * records_per_proc, out);
        
        fclose(out);
        
        free(global_temp);
        free(global_humidity);
        free(global_pressure);
    }
    
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