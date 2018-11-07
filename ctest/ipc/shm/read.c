#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>

#define CREATE_SHM_FAILED 1
#define ATTACH_SHM_FAILED 2


int main()
{
    key_t key_id = ftok("/home/darren", 666);
    int shmid;
    int *data;
    if ((shmid = shmget(key_id, 4, IPC_CREAT)) < 0) {
        printf("shared mem create failed (%s)\n", strerror(errno));
        exit(CREATE_SHM_FAILED);
    }
    
    if ((data = shmat(shmid, NULL, SHM_RDONLY)) == (void *)-1) {
        printf("shared mem attach failed (%s)\n", strerror(errno));
        exit(ATTACH_SHM_FAILED);
	}
    printf("%d\n", *data);
}
