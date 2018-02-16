#Reading Valgrind  
  
So I started learning about Valgrind because I had to. There is this huge project for creating a cache simulator for parallel simulator that I have been working on, and I have to find the memory leakage because the program is simple taking 60s GB of memory when run with SPEC 2017 benchmark.  
  
While my temporary fix was to run the simulator with smaller benchmarks as SPLASH, and it runs fine. But what if we need to run on large benchmarks, like SPEC 2017 itself.  
  
So here I am.  

##Step 1  
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
`
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

`

