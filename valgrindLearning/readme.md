# Reading Valgrind    
  
So I started learning about Valgrind because I had to. There is this huge project for creating a cache simulator for parallel simulator that I have been working on, and I have to find the memory leakage because the program is simple taking 60s GB of memory when run with SPEC 2017 benchmark.  
  
While my temporary fix was to run the simulator with smaller benchmarks as SPLASH, and it runs fine. But what if we need to run on large benchmarks, like SPEC 2017 itself.  
  
So here I am.  

## Step 1  

I wrote myself a small program that creates arrays. Since I wanted to see how defining array using *malloc* differs vs *new*. The first program is written with freeing of memory, so as expected theer is no error.  
```C++
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	int *myIntMalloc = (int*)malloc(10*sizeof(int));
	int *myIntNew = new int[10];


	delete[] myIntNew;
	free(myIntMalloc);

	return 0;
}  
```
  
Fllowing was the output gained when I used command `valgrind --leak-check=yes ./myProg1`  
```
==4271== Memcheck, a memory error detector
==4271== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==4271== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==4271== Command: ./myProg1
==4271== 
==4271== 
==4271== HEAP SUMMARY:
==4271==     in use at exit: 72,704 bytes in 1 blocks
==4271==   total heap usage: 3 allocs, 2 frees, 72,784 bytes allocated
==4271== 
==4271== LEAK SUMMARY:
==4271==    definitely lost: 0 bytes in 0 blocks
==4271==    indirectly lost: 0 bytes in 0 blocks
==4271==      possibly lost: 0 bytes in 0 blocks
==4271==    still reachable: 72,704 bytes in 1 blocks
==4271==         suppressed: 0 bytes in 0 blocks
==4271== Reachable blocks (those to which a pointer was found) are not shown.
==4271== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==4271== 
==4271== For counts of detected and suppressed errors, rerun with: -v
==4271== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```
  
While the information that is present here is too much, and since the program has no any error in terms of memory, we do not need to worry. But is is worthwhile to note that **definitely lost** means the memory that we need to fix as memory is being leaked . Other kind of leak is **possibly lost** which means the memory is not leaked unless you do something with pointers. (i will try to create one example for this later in this document)  
  
## Step2
  
So inorder to see the leaks I modified the program to have no free statements for memory allocated by *malloc*.  
I simply commented out the `free(myIntMalloc)` statement from the above code, and got the following result from valgrind  
```
==4833== HEAP SUMMARY:
==4833==     in use at exit: 72,744 bytes in 2 blocks
==4833==   total heap usage: 3 allocs, 1 frees, 72,784 bytes allocated
==4833== 
==4833== 40 bytes in 1 blocks are definitely lost in loss record 1 of 2
==4833==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4833==    by 0x4007AA: main (in /home/prawar/prawar/gdbLearning/valgrindLearning/myProg1)
==4833== 
==4833== LEAK SUMMARY:
==4833==    definitely lost: 40 bytes in 1 blocks
==4833==    indirectly lost: 0 bytes in 0 blocks
==4833==      possibly lost: 0 bytes in 0 blocks
==4833==    still reachable: 72,704 bytes in 1 blocks
==4833==         suppressed: 0 bytes in 0 blocks
==4833== Reachable blocks (those to which a pointer was found) are not shown.
==4833== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==4833== 
==4833== For counts of detected and suppressed errors, rerun with: -v
==4833== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

```
So not freeing is a clear memory leak, and as expected, we see 40 bytes in 1 block as **definitely lost**
Failure to delete the memory allocated with **new** also gives similar result.
  
## Step3

In step 3, I will try to recreate **possibly lost** leakage
