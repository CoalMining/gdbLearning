# gdbLearning

Learning to use gdb in linux machine....  

Lets make the program that is simple, but still calls a function. Following program calls a function that swaps the numbers passed as arguments.  
Following is the full program.  
```c  
#include <iostream>  
  
bool swap(int &a,int &b)  
{  
	int temp = a;  
	a=b;  
	b=temp;  
	return true;  
}  
int main()  
{  
	int a=5,b=7;  
	std::cout<<"Before swapping: a= "<<a<<",b= "<<b<<std::endl;  
	if(swap(a,b))  
	{  
		std::cout<<"After swapping: a= "<<a<<",b= "<<b<<std::endl;  
	}else{  
		std::cout<<"Error in swapping"<<std::endl;  
	}  
return 0;  
}  
```
Compiling and running the code is fine. But inorder to debug, * -g * flag should be provided while compiling. The command that I use to compile is as follows  
`g++ swap.c -o swap -g`  
To start debugging `gdb swap`  
To set the breakpoint before running the program, we should use `b <lineNumber>`. The first command lies at line number 12, so I used `b 12`