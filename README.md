# parallel-programmingg

After running ```sbatch compute.sh``` compute.sh script with a batch scheduler


![batch1](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch1.png)


![batch2](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch2.png)


![batch3](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch3.png)


![batch4](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch4.png)


![batch5](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch5.png)


After running ```sbatch overload.sh``` overload.sh script with a batch scheduler


![batch6andrest](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/batch6andrest.png)


Then later I've ran ```overload.sh``` with 12 CPUs, line I changed in the script

```bash
stress --cpu 12 --timeout 60s # from 1 to 12
```

![top](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/top.png)


After running compute.sh without a scheduler with this command ```salloc --ntasks=1 --cpus-per-task=12``` because I have 12 threads


![top2](https://github.com/faree2468/parallel-programmingg/blob/assignment-10/assets/top2.png)






