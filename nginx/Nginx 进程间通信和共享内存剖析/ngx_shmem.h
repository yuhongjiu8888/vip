


#ifndef __SHMEM_H_INCLUDED_
#define _SHMEM_H_INCLUDED_

#define _HAVE_SYSVSHM 1 
//#define _HAVE_MAP_DEVZERO  1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2




typedef struct {
    u_char      *addr;
    size_t       size;
   
} shm_t;


int shm_alloc(shm_t *shm);
void shm_free(shm_t *shm);


#endif /* _SHMEM_H_INCLUDED_ */

