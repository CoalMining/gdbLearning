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
`g++ swap.cpp -o swap -g`  
To start debugging `gdb swap`  
To set the breakpoint before running the program, we should use `b <lineNumber>`. The first command lies at line number 12, so I used `b 12`  
Following is how it looks:  
`$ gdb swap`  
The program *swap* is not run yet. This will only start the debugger. The prompt will contain message `(gdb)` from now on, so the actual command that I needed to type is without (gdb)  
`(gdb) b 12`  
The above statement will set break point at line number 12. This produces output as  
`Breakpoint 1 at 0x400951: file swap.cpp, line 12.`  

`(gdb) run`  
`Starting program: /home/pp0030/gdb/swap`   
`Breakpoint 1, main () at swap.cpp:12`  
`12              int a=5,b=7;`      
This command will actually start to run our program and the program execution stops at line 12 where we have our breakpoint.  
  
Command `n` will execute the program step by step. `n` actually stands for `next` which means *Step Over*, as compared to `s` or `step` which means to *Step Into*   
Following is what I would see when I use `n`  
`(gdb) n`  
`13              std::cout<<"Before swapping: a= "<<a<<",b= "<<b<<std::endl;`  
  
Simply presssing *enter* means that the previous command will be used  

`(gdb)`  
`Before swapping: a= 5,b= 7`  
`14              if(swap(a,b))`    
  
Now since I am at a function call, I would press `s` to Step Into the function  
`(gdb) s`  
`swap (a=@0x7fffffffe3c0: 5, b=@0x7fffffffe3c4: 7) at swap.cpp:5`  
`5               int temp = a;`  
`(gdb) n`  
`6               a=b;`  
`(gdb) n`  
`7               b=temp;`  
`(gdb)`  
`8               return true;`  
  
At any time I can print the value of a variable using `print <variable>`  
`(gdb) print a`  
`$1 = (int &) @0x7fffffffe3c0: 7`  
  
quit will Quit the gdb.    
`(gdb) quit`  
`A debugging session is active.` 
`  `  
`        Inferior 1 [process 12172] will be killed.  `  
`  `  
`Quit anyway? (y or n) y`  

-----

Gdb also allows you to perform debug of a program that is running. You can attach to a running process by specifying the process id of the process. Let us try that functionality also.

Let us write a program that runs infinitely so that we have plenty of time of run this process and attach to it.

```cpp
#include <iostream>

int main()
{
	int *a = (int*)malloc(1000*sizeof(int));
	a[999] = 4;
	while(1)
	{
		a[0] = (a[999]+100)%1000;
		for(int i=1;i<1000;i++)
		{
			a[i] = (a[i-1]+100)%1000;
		}
	}
}
```
I compile the program using `g++ infinte.cpp -o infinte -g` command to compile. Let us run this in background so that we have terminal under us. You can do this in two terminals: first terminal to run the `infinite` program and second to run the other `gdb` commands.

`ps` command in terminal will reveal the process id of the process that I am running. If you do not see it, you can run `ps -u <username> -a`. The pid in my case is `9838`.

Now we will start gdb such that the process with id 9838 will be attached to gdb. The command that I used was  
`sudo gdb -p 9838`
I could not do it without sudo access.

Now we can follow the similar operation to debug and see the intermediate values.

----


