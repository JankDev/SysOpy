#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"

const char *fifo_path = "server_fifo";

void read_client_requests(int fdesc, message *msg);
void execute_request(char *cmd, int cdesc);

int main(int argc, char const *argv[])
{
    puts("Server starting...");

    if (mkfifo(fifo_path, 0666) == -1)
    {
        perror("Error creating server fifo");
        puts("Server exiting...");
        exit(-1);
    }

    int fdesc = open(fifo_path, O_RDONLY);
    int cdesc;

    if (fdesc == -1)
    {
        perror("Error opening fifo");
        printf("Server exiting...");
        cleanup(cdesc, fdesc, fifo_path);
        exit(-1);
    }
    message *msg;
    while (1)
    {
        msg = malloc(sizeof(message));
        read_client_requests(fdesc, msg);

        cdesc = open(msg->fifo_path, O_WRONLY);

        if (cdesc == -1)
        {
            cleanup(cdesc, fdesc, fifo_path);
            perror("Error connecting to client");
        }

        execute_request(msg->cmd, cdesc);

        free(msg);
    }

    cleanup(cdesc, fdesc, fifo_path);

    return 0;
}

void read_client_requests(int fdesc, message *msg)
{
    if (read(fdesc, msg, sizeof(*msg)) == -1)
    {
        perror("Error reding from fifo");
        exit(-1);
    }

    return;
}

void execute_request(char *cmd, int cdesc)
{
    switch (fork())
    {
    case -1:
        perror("Error forking");
        exit(-1);
    case 0:
        close(1);
        dup2(cdesc, 1);
        execlp(cmd, cmd, NULL);
        exit(1);
    }
}