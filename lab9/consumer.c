#include "sem_util.h"

int main(int argc, char const *argv[])
{
    int semid, shmid;
    char (*buf)[MAX_SHM_MSG];

    if ((semid = semget(45281, 1, IPC_CREAT | 0660)) == -1)
    {
        perror("Error accessing sempahore");
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

    for (int i = 0; i < MAX_SEM_NUM * 1000; i++)
    {

        // snprintf(message, (size_t)MAX_SHM_MSG, "Hello %d", i);
        printf("%s", buf[i % MAX_SEM_NUM]);
        increase(semid, 0);
    }
    return 0;
}
