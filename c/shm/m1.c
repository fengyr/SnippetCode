#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	key_t key = ftok("/dev/shm/msm", 0);
	printf("key: %d\n", key);
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

	char *pshm;
	pshm = (char*)shmat(shm_id, NULL, 0);
	if (pshm == (void*)-1) {
		perror("shmat err");
		exit(1);
	}
	printf("pm shm addr: %x\n", pshm);

	memcpy(pshm, "this is shm test", sizeof(char)*100);
	sleep(5);
	if (shmdt(pshm) < 0) {
		perror("shmdt error");
	}
	return 0;
}
