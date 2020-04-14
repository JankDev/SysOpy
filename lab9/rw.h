#ifndef RW
#define RW

#include "unix.h"
#include "sem_util.h"


#define MAX_SEM_NUM 1
#define INIT_SEM_STATE 0

static struct sembuf buf;

int get_sem()
{
    int semid;
    key_t key = ftok("/tmp", 'a');
    
    if ((semid = semget(key, MAX_SEM_NUM, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        if ((semid = semget(key, MAX_SEM_NUM, IPC_CREAT | 0666)) == -1)
        {
            perror("Cant get sem");
            exit(-1);
        }
    }
    else
    {
        printf("Setting value of sem to %d\n", INIT_SEM_STATE);
        if (semctl(semid, 0, SETVAL, INIT_SEM_STATE) == -1)
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
    buf.sem_flg = IPC_NOWAIT;
    buf.sem_op = 0;

    if (semop(semid, &buf, MAX_SEM_NUM) == -1)
    {
        return 0;
    }

    return 1;
}

#endif