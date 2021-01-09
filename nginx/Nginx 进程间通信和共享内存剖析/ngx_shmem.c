
#include "shmem.h"



#if (_HAVE_MAP_ANON)

int
shm_alloc(shm_t *shm)
{
    shm->addr = (u_char *) mmap(NULL, shm->size,
                                PROT_READ|PROT_WRITE,
                                MAP_ANON|MAP_SHARED, -1, 0);

    if (shm->addr == MAP_FAILED) {
        fprintf(stderr,
                      "mmap(MAP_ANON|MAP_SHARED, %lu) failed, reason: %s.\n", shm->size,strerror(errno));
        return NGX_ERROR;
    }

    return NGX_OK;
}


void
shm_free(shm_t *shm)
{
    if (munmap((void *) shm->addr, shm->size) == -1) {
        fprintf(stderr,
                      "munmap(%p, %lu) failed, reason: %s.\n", shm->addr, shm->size, strerror(errno));
    }
}

#elif (_HAVE_MAP_DEVZERO)

int
shm_alloc(shm_t *shm)
{
    int  fd;

    fd = open("/dev/zero", O_RDWR);

    if (fd == -1) {
        fprintf(stderr,
                      "open(\"/dev/zero\") failed.\n");
        return NGX_ERROR;
    }

    shm->addr = (u_char *) mmap(NULL, shm->size, PROT_READ|PROT_WRITE,
                                MAP_SHARED, fd, 0);

    if (shm->addr == MAP_FAILED) {
        fprintf(stderr,
                      "mmap(/dev/zero, MAP_SHARED, %lu) failed, reason: %s.\n", shm->size, strerror(errno));
    }

    if (close(fd) == -1) {
        fprintf(stderr,
                      "close(\"/dev/zero\") failed.\n");
    }

    return (shm->addr == MAP_FAILED) ? NGX_ERROR : NGX_OK;
}


void
shm_free(shm_t *shm)
{
    if (munmap((void *) shm->addr, shm->size) == -1) {
        fprintf(stderr,
                      "munmap(%p, %lu) failed", shm->addr, shm->size);
    }
}

#elif (_HAVE_SYSVSHM)

//#include <sys/ipc.h>
#include <sys/shm.h>


int
shm_alloc(shm_t *shm)
{
    int  id;
	
	key_t shm_key = ftok("test",889);
	
	fprintf(stdout,"use shmget for sharing memory.IPC_PRIVATE:%d\n",(int)IPC_PRIVATE);
    //id = shmget(IPC_PRIVATE, shm->size, (SHM_R|SHM_W|IPC_CREAT));
	id = shmget(shm_key, shm->size, (SHM_R|SHM_W|IPC_CREAT));
    //id = shmget(shm_key, shm->size, (0666|IPC_CREAT));
	
	

    if (id == -1) {
        fprintf(stderr,
                      "shmget(%lu) failed, reason: %s.\n", shm->size, strerror(errno));
        return NGX_ERROR;
    }

    fprintf(stdout, "shmget id: %d.\n", id);

    shm->addr = shmat(id, NULL, 0);

    if (shm->addr == (void *) -1) {
        fprintf(stderr, "shmat() failed. reason: %s.\n",strerror(errno));
    }

    /*if (shmctl(id, IPC_RMID, NULL) == -1) {
        fprintf(stderr,
                      "shmctl(IPC_RMID) failed, reason: %s.\n",strerror(errno));
    }*/

    return (shm->addr == (void *) -1) ? NGX_ERROR : NGX_OK;
}


void
shm_free(shm_t *shm)
{
    if (shmdt(shm->addr) == -1) {
        fprintf(stderr,
                      "shmdt(%p) failed, reason: %s.\n", shm->addr,strerror(errno));
    }
}

#endif
