CC = mpicc
CFLAGS = -O3 -Wall

all: version2

version2: file_processing_v2.c
	$(CC) $(CFLAGS) -o version2 file_processing_v2.c

clean:
	rm -f version2 output_independent.dat

run:
	mpirun -n 4 ./version2

.PHONY: all clean run1 