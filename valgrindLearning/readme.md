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

In step 3, I have tried to create a little more complex program with a class of my own. Two object pointers are created: one using smart pointer **unique_ptr** and other using normal pointer. Following is the code snippet that I have used in the beginning.  
```C++  
#include <iostream>
#include <memory>

using namespace std;

class myClass
{
        private:
                int id;
                string name;
        public:
        myClass()
        {
                id = -1;
                name = "";
        }
        ~myClass(){}
        void setMembers(int idArg,string nameArg)
        {
                id = idArg;
                name = nameArg;
        }
        void printMembers()
        {
                cout<<"My id is "<<id<<", my name is "<<name<<endl;
        }
};

int main()
{
        //defining one object of the class here using conventional pointer
        myClass *myClassObj1 = new myClass;
        myClassObj1->setMembers(1,"Adam");
        myClassObj1->printMembers();

        unique_ptr<myClass> myClassObj2(new myClass());
        myClassObj2->setMembers(2,"Bob");
        myClassObj2->printMembers();

}

```
     
The executable is named as *uniquePointer*. Following is what I get when I use `valgrind --leak-check=yes ./uniquePointer`  
```
==31593== Memcheck, a memory error detector
==31593== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==31593== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==31593== Command: ./uniquePointer
==31593==
My id is 1, my name is Adam
My id is 2, my name is Bob
==31593==
==31593== HEAP SUMMARY:
==31593==     in use at exit: 72,744 bytes in 2 blocks
==31593==   total heap usage: 4 allocs, 2 frees, 73,808 bytes allocated
==31593==
==31593== 40 bytes in 1 blocks are definitely lost in loss record 1 of 2
==31593==    at 0x4C2E0EF: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==31593==    by 0x400E99: main (in /home/pp0030/gdb/smartPointers/uniquePointer)
==31593==
==31593== LEAK SUMMARY:
==31593==    definitely lost: 40 bytes in 1 blocks
==31593==    indirectly lost: 0 bytes in 0 blocks
==31593==      possibly lost: 0 bytes in 0 blocks
==31593==    still reachable: 72,704 bytes in 1 blocks
==31593==         suppressed: 0 bytes in 0 blocks
==31593== Reachable blocks (those to which a pointer was found) are not shown.
==31593== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==31593==
==31593== For counts of detected and suppressed errors, rerun with: -v
==31593== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

```
  
This clearly shows that there is some memory that needs to be freed. After I free the pointer *myClassObj1*, the error is not there. This proves that unique_ptr doesnot need to be freed (which is the whole point for using smart pointer)  
  
Defining pointers inside some other function and not freeing them is also one source of memory leakage, and Valgrid would find these kind of leakage. However, when smart pointer is used we do not need to free them. This is because the memory held by mart pointer is cleared as soon as the execution goes out of scope.  

## Step4  
My step 4 would be to see the thing described in the above paragraph. This would give insight more to how unique_ptr works rather than Valgrind. I came up with the following code that would roughly demonstrate the same. (The class is same as above code)  
  
So I wrote a function something like following and the call was made as shown below:
```C++
void myFunc1(myClass* &inClass)
{
	cout<<"..Inside the function myFunc()"<<endl;

	myClass *myClassObjFun = new myClass;
	myClassObjFun->setMembers(23,"InFunc");
	myClassObjFun->printMembers();
	inClass = myClassObjFun;
	cout<<"..Leaving the function.."<<endl;
}

int main()
{
	//defining one object of the class here using conventional pointer

	myClass *myClassObj4;
	myFunc1(myClassObj4);
	myClassObj4->printMembers();

	delete myClassObj4;
	return 0;
}
```  
Here I have passed an reference to the pointer to the function. An object is created inside the function and now both the pointers point to the same object. So deleting is not necessary for inside the function pointer, since the pointer in main is deleted. (One pitfall can be incase when assigning fails inside the called function. But that can not be the case in this small  example.)  
```
==2842== Memcheck, a memory error detector
==2842== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==2842== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==2842== Command: ./uniquePointer3
==2842==
..Inside the function myFunc()
My id is 23, my name is InFunc
..Leaving the function..
My id is 23, my name is InFunc
Destructor called 23 InFunc
==2842==
==2842== HEAP SUMMARY:
==2842==     in use at exit: 72,704 bytes in 1 blocks
==2842==   total heap usage: 3 allocs, 2 frees, 73,768 bytes allocated
==2842==
==2842== LEAK SUMMARY:
==2842==    definitely lost: 0 bytes in 0 blocks
==2842==    indirectly lost: 0 bytes in 0 blocks
==2842==      possibly lost: 0 bytes in 0 blocks
==2842==    still reachable: 72,704 bytes in 1 blocks
==2842==         suppressed: 0 bytes in 0 blocks
==2842== Reachable blocks (those to which a pointer was found) are not shown.
==2842== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2842==
==2842== For counts of detected and suppressed errors, rerun with: -v
==2842== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```
   
But when I change the function to use the smart pointer instead of normal pointer, this would be a problem in this scenario because for every smart pointer, if you travel out of scope, destructor is called. (The message to the destructor is added later on).  So simply replacing the pointer in the above case with *uniqie_ptr* would not help, rather degrade the situation.  
Following kind of code can be used instead where a copy is made rather than pointing to same object.  
```C++
void myFunc1(myClass* &inClass)
{
	cout<<"..Inside the function myFunc()"<<endl;
	
	unique_ptr<myClass> myClassObj3(new myClass());
	myClassObj3->setMembers(3,"Charlie");
	myClassObj3->printMembers();
	*inClass = *myClassObj3.get();

	cout<<"..Leaving the function.."<<endl;
}

int main()
{
	//defining one object of the class here using conventional pointer

	myClass *myClassObj4 =  new myClass;
	myFunc1(myClassObj4);
	myClassObj4->printMembers();

	delete myClassObj4;
	return 0;
}
```  
What this basically does is creates a smart pointer pointing to object inside the function. Destructor to it is called whenever we leave the function. So there is not point in pointing to that object by a normal pointer. So the best idea would be to make a copy.
