# parallel-programmingg

## Assignment 3

## AoSoA measurement

1. I've allocated an AoSoA of size numblocks (size of array / vector size)
```cpp
SoA_type* AoSoA = new SoA_type[num_blocks];
``` 

2. I've uncommented lines
```cpp
AoSoA[j].R[i] = rand();
AoSoA[j].G[i] = rand();
AoSoA[j].B[i] = rand();
```
3. I've deallocated resources of the AoSoA after using it
```cpp
delete[] AoSoA;
```

## Makefile changes

1. I've added TARGET_SRC for my .cpp file and added 100M test
```
TARGET_SRC = aosoa_measurement.cpp
```
Then after that I've run these commands in the terminal
```bash
make test_1K
make test_10K
make test_100K
make test_1M
make test_10M
make test_100M
```

Google spreadsheet link:
https://docs.google.com/spreadsheets/d/1f5XVMrMRfbBWUfzhxZ8kMPe1C5ggji32JIkr9uaHCog/edit?gid=0#gid=0

## Graph created
[graph](https://github.com/faree2467/parallel-programmingg/blob/assignment3/assets/graph.png?raw=true)
