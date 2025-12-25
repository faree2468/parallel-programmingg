CC = mpicc
CFLAGS = -O3 -Wall

all: version1

version1: file_processing_v1.c
	$(CC) $(CFLAGS) -o version1 file_processing_v1.c

clean:
	rm -f version1 output_serial.dat

run:
	mpirun -n 4 ./version1

.PHONY: all clean run