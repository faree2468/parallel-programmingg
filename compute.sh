#!/bin/bash
#SBATCH -J compute_work
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 00:05:00
#SBATCH -o work_%j.out

echo "Job started at $(date)"
echo "Job ID: $SLURM_JOB_ID"
echo "Running on: $(hostname)"
echo "Using $SLURM_NTASKS processors"

for i in {1..4}; do
    (
        for j in {1..50000}; do
            echo "scale=500; 4*a(1)" | bc -l > /dev/null 2>&1
        done
        echo "Process $i completed"
    ) &
done

wait

echo "All processes finished at $(date)"