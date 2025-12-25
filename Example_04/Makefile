CC = mpicc
CFLAGS = -O3 -Wall

all: version4

version4: file_processing_v4.c
	$(CC) $(CFLAGS) -o version4 file_processing_v4.c

clean:
	rm -f version4 output_with_halos.dat

run:
	mpirun -n 4 ./version4

.PHONY: all clean run 