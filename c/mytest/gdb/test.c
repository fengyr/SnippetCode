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

int main()
{
	int i;
	long result = 0;

	while (1)
	{
		for(i=1; i<=100; i++)
		{
			result += i;
		}

		printf("result[1-100] = %d \n", result );
		printf("result[1-250] = %d \n", func(i) );

		sleep(1);
	}

	return 0;
}
