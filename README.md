student@itcenter-lab128:~/Desktop/assignment1$ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main  
==21513== Memcheck, a memory error detector  
==21513== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.  
==21513== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info  
==21513== Command: ./main  
==21513==  
==21513== Invalid write of size 4  
==21513==    at 0x1091C6: main (in /home/student/Desktop/assignment1/main)  
==21513==  Address 0x4aa2068 is 0 bytes after a block of size 40 alloc'd  
==21513==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)  
==21513==    by 0x109185: main (in /home/student/Desktop/assignment1/main)  
==21513==   
==21513== Conditional jump or move depends on uninitialised value(s)  
==21513==    at 0x1091F4: main (in /home/student/Desktop/assignment1/main)  
==21513==  Uninitialised value was created by a stack allocation  
==21513==    at 0x109169: main (in /home/student/Desktop/assignment1/main)  
==21513==   
==21513== Invalid read of size 4  
==21513==    at 0x1091EF: main (in /home/student/Desktop/assignment1/main)  
==21513==  Address 0x4aa2068 is 0 bytes after a block of size 40 alloc'd  
==21513==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)  
==21513==    by 0x109185: main (in /home/student/Desktop/assignment1/main)  
==21513==   
==21513== 
==21513== HEAP SUMMARY:  
==21513==     in use at exit: 40 bytes in 1 blocks  
==21513==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated  
==21513==   
==21513== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1  
==21513==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)  
==21513==    by 0x109185: main (in /home/student/Desktop/assignment1/main)  
==21513==   
==21513== LEAK SUMMARY:  
==21513==    definitely lost: 40 bytes in 1 blocks  
==21513==    indirectly lost: 0 bytes in 0 blocks  
==21513==      possibly lost: 0 bytes in 0 blocks  
==21513==    still reachable: 0 bytes in 0 blocks  
==21513==         suppressed: 0 bytes in 0 blocks  
==21513==   
==21513== For lists of detected and suppressed errors, rerun with: -s  
==21513== ERROR SUMMARY: 14 errors from 4 contexts (suppressed: 0 from 0)  
  
1. pointer iarray was not freed  
2. ival and ipos values were not initialized to 0 because it cant be uninitialized  
3. loop went to 10, and last index that can be accessed is 9 (from 0 to 9)  

