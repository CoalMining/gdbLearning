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