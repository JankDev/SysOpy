#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUF_SIZE 100

int write_msg(char *text, int sdesc)
{
    int bwrite;

    if ((bwrite = write(sdesc, text, MAX_BUF_SIZE)) == -1)
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

#endif