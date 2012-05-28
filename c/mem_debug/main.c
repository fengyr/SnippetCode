#include <stdio.h>

int main()
{
	int *i;
	int j[10] = {0};
	i = malloc(sizeof(int));

	*i = 10;
	printf("i: %d, j[0]:%d\n", *i, j[0]);
	free(i);

	return 0;
}
