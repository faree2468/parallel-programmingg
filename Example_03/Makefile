CC = mpicc
CFLAGS = -O3 -Wall

all: version3

version3: file_processing_v3.c
	$(CC) $(CFLAGS) -o version3 file_processing_v3.c

clean:
	rm -f version3 output_collective.dat

run:
	mpirun -n 4 ./version3

.PHONY: all clean run 