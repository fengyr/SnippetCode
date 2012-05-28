#include "test.h"

int compar(int x, int y)
{
	return (x < y) ? 10 : 11;
}

int test_call(int a, int b, int compar(int x, int y))
{
	return compar(a, b);
}

void test_struct(struct point pt)
{
	printf("%d-%d-%s\n", pt.x, pt.y, pt.name);
}

int main()
{
	int a = 10;
	int b = 11;

	int res = test_call(a, b, compar);
	printf("res:%d\n", res);

	struct point pt;
	pt.x = 1;
	pt.y = 2;
	pt.name = "zenki";
	test_struct(pt);

	return 0;
}

