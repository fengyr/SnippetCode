#include <stdio.h>

struct point {
	int x;
	int y;
	char *name;
};

int test_call(int a, int b, int (*)(int, int));

int compar(int x, int y);

void test_struct(struct point);
