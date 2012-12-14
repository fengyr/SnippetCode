#include <stdio.h>

int func(int n)
{
	int sum=-10,i;
	for(i=0; i<n; i++)
	{
		sum+=i;
		printf("test:-->%d\n", i/sum);
	}
	return sum;
}

