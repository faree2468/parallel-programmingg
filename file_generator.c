#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int num_stations = 50;
    int grid_size = 100;
    int days = 30;
    
    if (argc > 1) num_stations = atoi(argv[1]);
    if (argc > 2) grid_size = atoi(argv[2]);
    if (argc > 3) days = atoi(argv[3]);
    
    srand(time(NULL));
    
    FILE *fp = fopen("temperature_data.csv", "w");
    if (!fp) {
        fprintf(stderr, "Error creating file\n");
        return 1;
    }
    
    fprintf(fp, "station_id,day,row,col,temperature,humidity,pressure,wind_speed,precipitation\n");
    
    long total = 0;
    for (int station = 0; station < num_stations; station++) {
        for (int day = 0; day < days; day++) {
            for (int row = 0; row < grid_size; row++) {
                for (int col = 0; col < grid_size; col++) {
                    double base_temp = 20.0 + sin(day * 0.2) * 10.0;
                    double variation = ((double)rand() / RAND_MAX - 0.5) * 8.0;
                    double spatial = sin(row * 0.1) * cos(col * 0.1) * 3.0;
                    double temp = base_temp + variation + spatial;
                    
                    double humidity = 50.0 + ((double)rand() / RAND_MAX) * 40.0;
                    double pressure = 1013.0 + ((double)rand() / RAND_MAX - 0.5) * 20.0;
                    double wind_speed = ((double)rand() / RAND_MAX) * 15.0;
                    double precipitation = ((double)rand() / RAND_MAX) * 5.0;
                    
                    fprintf(fp, "%d,%d,%d,%d,%.6f,%.6f,%.6f,%.6f,%.6f\n", 
                            station, day, row, col, temp, humidity, pressure, wind_speed, precipitation);
                    total++;
                }
            }
        }
    }
    
    fclose(fp);
    
    printf("Generated %ld records in temperature_data.csv\n", total);
    printf("File size: ~%.1f MB\n", total * 80.0 / (1024.0 * 1024.0));
    
    return 0;
}