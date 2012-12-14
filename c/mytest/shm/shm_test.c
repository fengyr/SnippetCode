#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void ShowShm(int shm_id)
{
	struct shmid_ds ds;
	memset(&ds, 0, sizeof(ds));
	if (shmctl(shm_id, IPC_STAT, &ds) < 0) {
		perror("shmctl error");
	}

	printf("segsz:%d\nseg atime:%d\nseg dtime:%d\nseg ctime:%d\nseg cpid:%d\nseg lpid:%d\n", \
			ds.shm_segsz, ds.shm_atime, ds.shm_dtime, ds.shm_ctime, ds.shm_cpid, ds.shm_lpid);
}

int main(int argc, char **argv)
{
	key_t key = ftok("/tmp", 0);
	if (key < 0) {
		printf("key is err\n");
		exit(1);
	}

	int shm_id;
	shm_id = shmget(key, 40960, IPC_CREAT);
	if (shm_id < 0) {
		if (errno == EEXIST) {
			printf("shmget error: EEXIST\n");
			exit(1);
		}
		exit(1);
	}

	ShowShm(shm_id);
	char *pshm;
	pshm = (char*)shmat(shm_id, NULL, 0);
	if (pshm == (void*)-1) {
		perror("shmat err");
		exit(1);
	}
    memcpy(pshm, "hello world", sizeof("hello world"));
	printf("cm shm addr: %x\n", pshm);
	printf("shm context: %s\n", pshm);

	sleep(5);
	if (shmdt(pshm) < 0) {
		perror("shmdt error");
	}
	return 0;
}
