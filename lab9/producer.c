#include "sem_util.h"

int main(int argc, char const *argv[])
{
    int semid, shmid;
    char(*buf)[MAX_SHM_MSG];

    if ((semid = semget(45281, MAX_SEM_NUM, IPC_CREAT | 0660)) == -1)
    {
        perror("Error accessing sempahore");
        exit(-1);
    }

    if (semctl(semid, 0, SETVAL, MAX_SEM_NUM) == -1)
    {
        perror("Error setting value of semaphore");
        exit(-1);
    }

    if ((shmid = shmget(45821, MAX_SEM_NUM * MAX_SHM_MSG, IPC_CREAT | 0660)) == -1)
    {
        perror("Error creating shmem");
        exit(-1);
    }

    if ((buf = shmat(shmid, NULL, 0)) == NULL)
    {
        perror("Error accessing shared mem");
        exit(-1);
    }
    char *message = malloc(MAX_SHM_MSG);

    for (int i = 0; i < MAX_SEM_NUM * 1000; i++)
    {

        snprintf(message, MAX_SHM_MSG, "Hello %d\n", i);
        strcpy(buf[i % MAX_SEM_NUM], message);
        decrease(semid, 0);
    }

    free(message);
    return 0;
}