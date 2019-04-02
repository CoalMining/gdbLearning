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