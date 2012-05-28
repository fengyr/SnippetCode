#include <stdio.h>
int main()
{
	int i = 1;
	int j = 2;
	char *str = "hello";
	printf("%x--%x--%x\n", &i, &j, str);
	return 0;
}
