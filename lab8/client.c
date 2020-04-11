#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "util.h"

const char *fifo_path = "client_fifo";

int main(int argc, char const *argv[])
{
    const char *server_fifo = argv[1];

    int sdesc = connect_to_server(server_fifo);
    int pdesk;

    if (sdesc == -1)
    {
        exit(-1);
    }

    if (mkfifo(fifo_path, 0666) == -1)
    {
        perror("Error creating client fifo");
    }

    while (1)
    {
        puts("Type the command the server should execute");
        fflush(stdout);

        char cmd[MAX_BUF_SIZE];
        scanf("%s", cmd);

        if (strcmp("exit", cmd) == 0)
        {
            cleanup(sdesc, pdesk, fifo_path);
            exit(1);
        }

        if (write_msg(cmd, fifo_path, sdesc) == -1)
        {
            cleanup(sdesc, pdesk, fifo_path);
            exit(-1);
        }

        pdesk = open(fifo_path, O_RDONLY);

        if (pdesk == -1)
        {
            cleanup(sdesc, pdesk, fifo_path);
            perror("Error opening fifo");
        }

        char buf[MAX_BUF_SIZE];
        if (read(pdesk, buf, MAX_BUF_SIZE) == -1)
        {
            cleanup(sdesc, pdesk, fifo_path);
            perror("Error reading response from server");
        }

        fputs(buf, stdout);
        memset(buf, 0, MAX_BUF_SIZE);
    }

    cleanup(pdesk, sdesc, fifo_path);

    return 0;
}
