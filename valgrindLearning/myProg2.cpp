#include <iostream>
#include <cstdlib>

using namespace std;

int *func()
{
	int *thisPointer;
	thisPointer = (int*)malloc(10*sizeof(int));
	return thisPointer;
}

int main()
{
	int *myIntOuter = func();

	for(;*(myIntOuter++)!=NULL;)
		cout<<*(myIntOuter);

	return 0;
}