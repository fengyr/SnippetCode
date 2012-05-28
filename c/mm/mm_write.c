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

void Pagesize()
{
	long page_size;
	page_size = sysconf(_SC_PAGE_SIZE);
	printf("page size: %d\n", page_size);
}

DATA* map_memory(char *filename, int size)
{
    int fd;
    DATA *pmstr;

    fd = open(filename, O_CREAT | O_TRUNC |O_RDWR, 0777);
    if (fd < 0) {
        printf("open file err\n");
        return NULL;
    }
    lseek(fd, size, SEEK_SET);
    write(fd, "", 1);

    pmstr = (DATA*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("write map addr: %x\n", pmstr);

    close(fd);

    return pmstr;
}

int main(int argc, char **argv)
{
	DATA *pmstr;
    int size = 10*10;
    int len = 0;
    char *str = "hello";
    int i = 0;

    if (argc < 2) {
        printf("useage:exec filename\n");
        exit(1);
    }

    // show page size
    Pagesize();
    pmstr = map_memory(argv[1], size);

    while (1) {
        if ( len >= size )
            break;
        memcpy(pmstr->data1, str, 5);
        memcpy(pmstr->data2, str, 5);
        pmstr->len = i;
        printf("i: %d\n", i);
        len += sizeof(DATA)*(++i);
        pmstr++;
        sleep(1);
    }

    munmap(pmstr, size);

    return 0;
}
