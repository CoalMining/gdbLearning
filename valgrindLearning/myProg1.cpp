#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	int *myIntMalloc = (int*)malloc(10*sizeof(int));
	int *myIntNew = new int[10];


	//delete[] myIntNew;
	free(myIntMalloc);

	return 0;
}