#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

long count = 0;

typedef struct e_info{
	int pp_time;
	int logsize;
	double freerate;
}EXTRA_INFO, *PEXTRA_INFO;

typedef struct p_time {
	int a;
	double b;
}PTIME;

void quit()
{
	printf("count");
	printf("quit\n");
}

int main()
{
	EXTRA_INFO info;
	char str[128];
	char a[20], b[20], c[20];
	memset(str, 0, sizeof(str));
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	memset(c, 0, sizeof(c));
	strcpy(str, "008,009,1234");

	printf("你好, size: %d\n", sizeof("你好"));
	sscanf(str, "%3s,%3s", a, b);
	printf("a:%s,b:%s,c:%s\n", a, b, c);
	char str2[32] = {'1', '2', '3', '4', '\0'};
	printf("str2:%s\n", str2);
	time_t t = time(NULL);
	printf("time:%d\n", (int)t);

    int i = 0x01;
    int j = (i << 24) + (0xff << 16) + 0x01;
    /*int j = (i << 8) + 0x01;*/
    printf("i: %d, j: %d\n", i, j);

	return 0;
}

