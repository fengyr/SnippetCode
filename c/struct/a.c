#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dlfcn.h>

struct fl {
	unsigned char a:3;
	unsigned char b:4;
	unsigned char c:7;
	unsigned char d:2;
};

struct f2 {
	int a;
	int b;
	char *c;
	char *d;
	double e;
	int (*myfunc)(int*);
};

int tsfunc(int* str)
{
	printf("this is tsfunc\n");
	int *p = str;
	printf("%d--%d\n", *p, p);
	return *p;
}


int main()
{
/* 	int i = 100;
 * 	int *s = func(i);
 * 	printf("%d\n", *s);
 */

	struct f2 f = {
		.a = 10, 		\
		.c = "hello", 	\
		.d = "world", 	\
		.myfunc = tsfunc, \
	};

	int i = 100;
	int s = f.myfunc(&i);
  	printf("%d, %d, %s, %s, %d, %d\n", f.a, f.b, f.c, f.d, f.e, s);

	return 0;
}
