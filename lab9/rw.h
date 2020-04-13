#ifndef RW
#define RW

#include "unix.h"
#include "sem_util.h"

#define SEM_KEY 42187
#define MAX_SEM_NUM 1

static struct sembuf buf;

int get_sem()
{
    int semid;
    if ((semid = semget(SEM_KEY, MAX_SEM_NUM, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        if ((semid = semget(SEM_KEY, MAX_SEM_NUM, IPC_CREAT | 0666)) == -1)
        {
            perror("Cant get sem");
            exit(-1);
        }
    }
    else
    {
        printf("Setting value of sem to %d", MAX_SEM_NUM);
        if (semctl(semid, 0, SETVAL, MAX_SEM_NUM) == -1)
        {
            perror("Error setting intial value");
            exit(-1);
        }
    }

    return semid;
}

int is_readable(int semid, int sem_num)
{
    buf.sem_num = sem_num;
    buf.sem_flg = 0;
    buf.sem_op = IPC_NOWAIT;

    if (semop(semid, &buf, MAX_SEM_NUM) == -1)
    {
        return 0;
    }

    return 1;
}

#endif