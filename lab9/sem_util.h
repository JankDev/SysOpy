#ifndef MYSEM_TEST
#define MYSEM_TEST

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define MAX_SEM_NUM 1
#define MAX_SHM_MSG 20

static struct sembuf buf;

/**
 * Increases the semaphore count by 1
 * */
void increase(const int semid, const unsigned sem_num)
{
    const unsigned sem_size = 1;
    buf.sem_num = sem_num;
    buf.sem_op = 1;
    buf.sem_flg = 0;

    if (semop(semid, &buf, sem_size) == -1)
    {
        perror("Error operating on semaphore");
        exit(-1);
    }
}

/**
 * Decreases the semaphore count by 1
 * */
void decrease(const int semid, const unsigned sem_num)
{
    const unsigned sem_size = 1;
    buf.sem_num = sem_num;
    buf.sem_op = -1;
    buf.sem_flg = 0;

    if (semop(semid, &buf, sem_size) == -1)
    {
        perror("Error operating on semaphore");
        exit(-1);
    }
}
#endif