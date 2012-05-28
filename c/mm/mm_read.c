#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct data{
    char data1[20];
    char data2[20];
    int  len;
} DATA;

DATA* map_memory(char *filename, int size)
{
    int fd;
    DATA *pmstr;

    fd = open(filename, O_RDWR, 0777);
    if (fd < 0) {
        printf("open file err\n");
        return NULL;
    }

    pmstr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pmstr == MAP_FAILED)
        perror("mmap err");
    else
        printf("read map addr: %x\n", pmstr);

    close(fd);

    return pmstr;
}

int main(int argc, char **argv)
{
	DATA *pmstr;
	char context[1024];
    int size = 10*10;
	int i = 0;

	if (argc < 2) {
		printf("useage:exec filename\n");
		exit(1);
	}

	memset(context, 0, sizeof(context));
    pmstr = (DATA*)map_memory(argv[1], size);
	
    while ( sizeof(DATA)*i < size) {
        /*memcpy(pmstr->data1, "change it", sizeof("change it"));*/
        /*memcpy(pmstr->data2, "do it", sizeof("do it"));*/
        /*pmstr->len = 20;*/
        printf("data1: %s, data2: %s, len: %d\n", pmstr->data1, pmstr->data2, pmstr->len);
        i++;
        pmstr++;
        sleep(1);
    }

    munmap(pmstr, size);

    return 0;
}
