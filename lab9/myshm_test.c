#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SHM_SIZE 100

int main(int argc, char const *argv[])
{
    int shmid;
    char *pbuf;
    char *cbuf;

    if ((shmid = shmget(IPC_PRIVATE, MAX_SHM_SIZE, IPC_CREAT | 0660)) == -1)
    {
        perror("Error creating shared mem");
        exit(-1);
    }
    char msg[6] = "hello";

    switch (fork())
    {
    case -1:
        perror("fork error");
        exit(-1);
    case 0:
        cbuf = (char *)shmat(shmid, NULL, 0);
        for (int i = 0; i < sizeof(msg); i++)
        {
            cbuf[i] = msg[i];
        }

        exit(1);
        break;
    default:
        pbuf = (char *)shmat(shmid, NULL, 0);

        wait(0);

        if (pbuf == NULL)
        {
            perror("Error accessing shmem");
            exit(-1);
        }

        for (int i = 0; i < sizeof(msg); i++)
        {
            printf("%c", pbuf[i]);
        }
        puts("");
    }

    return 0;
}
