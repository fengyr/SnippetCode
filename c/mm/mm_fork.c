#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct data{
    char data1[20];
    char data2[20];
    int  len;
} DATA;

DATA* map_memory(int size)
{
    DATA *pmstr;

    pmstr = (DATA*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("write map addr: %x\n", pmstr);

    return pmstr;
}

int main(int argc, char **argv)
{
	DATA *pmstr, *orgstr;
    int size = 10*22;       // 5组数据的大小为220字节
    int len = 0;
    char *str = "hello";
    int i = 0;
    pid_t pid = -1;

    // show page size
    orgstr = pmstr = map_memory(size);

    while ( len < size) {
        memcpy(pmstr->data1, str, 5);
        memcpy(pmstr->data2, str, 5);
        pmstr->len = i;
        i++;
        len += sizeof(DATA);
        printf("in parent process i: %d, len: %d\n", pmstr->len, len);

        pmstr++;
        sleep(1);
    }

    pid = fork();
    if ( pid == 0) {
        len = 0;
        pmstr = orgstr;
        while ( len < size) {
            printf("in child data1: %s, data2: %s, i: %d\n", pmstr->data1, pmstr->data2, pmstr->len);

            len += sizeof(DATA);
            pmstr++;
        }

        munmap(pmstr, size);
        printf("child exit\n");
    }
    else {
        printf("this is parent exit\n\n");
    }

    return 0;
}
