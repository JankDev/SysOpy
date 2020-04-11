#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUF_SIZE 100

typedef struct
{
    char cmd[MAX_BUF_SIZE];
    char fifo_path[MAX_BUF_SIZE];
} message;

int write_msg(char *cmd, const char *client_fifo, int sdesc)
{
    int bwrite;

    message msg;
    strcpy(msg.cmd, cmd);
    strcpy(msg.fifo_path, client_fifo);

    if ((bwrite = write(sdesc, &msg, sizeof(msg))) == -1)
    {
        perror("Error writing message");
        return -1;
    }

    return bwrite;
}

int connect_to_server(const char *server_fifo)
{
    puts("Connecting to server...");

    int sdesc;
    if ((sdesc = open(server_fifo, O_WRONLY)) == -1)
    {
        perror("Error connecting to server");
        return -1;
    }

    puts("Connected to server");

    return sdesc;
}

void cleanup(int fdesc, int fdesc2, const char *fifo_path)
{
    close(fdesc);
    close(fdesc2);

    unlink(fifo_path);
}

#endif