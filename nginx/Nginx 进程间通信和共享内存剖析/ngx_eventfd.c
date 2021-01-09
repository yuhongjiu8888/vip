#include <stdlib.h>
#include <stdio.h>
#include <sys/eventfd.h>
#include <pthread.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

int efd = -1;
//循环接收线程。
void * recv(void * pars) {
    int len = -1;
    uint64_t msg = 0;

    printf("Thread created!\n");
    while(1) {
        len = read(efd, &msg, sizeof(uint64_t)); //如果计数器为0则阻塞。
        printf("Current msg is %ld\n", msg);
    }
}

int main(int argc, char ** argv) {
    pthread_t tid;
    pid_t fpid;
    int len = -1;
    static uint64_t item = 1;
    int i = 0;

    efd = eventfd(0, 0);
    if(efd == -1) {
        printf("Error! Fail to create event fd.\n");
    } else {
        printf("OK! Create event fd is %d\n", efd);
    }
    fpid = fork();
    if(fpid > 0) {  //父进程接收消息（父进程同时也发送消息。）
        pthread_create(&tid, NULL, recv, NULL);
    }

    //循环发送。
    while(1) {
        sleep(2);
        len = write(efd, &item, sizeof(uint64_t)); //如果写sizeof(unsigned int)则参数错误。必须传8。
                                    //如果计数器达到0xfffffffe，则阻塞。
        if(len != sizeof(uint64_t)) {
            printf("Error! Fail to write event fd, errno: %d,errmsg: %s\n"
                    ,errno, strerror(errno));
        }else {
            printf("Success to write.\n");
        }
    }
    pthread_exit(0);
    return 0;
}
