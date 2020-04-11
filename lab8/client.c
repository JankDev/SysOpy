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

    if (sdesc == -1)
    {
        exit(-1);
    }

    if (mkfifo(fifo_path, 0666) == -1)
    {
        perror("Error creating client fifo");
    }

    int pdesk = open(fifo_path, O_RDONLY);

    if (pdesk == -1)
    {
        perror("Error opening fifo");
    }

    if (write_msg("ls", sdesc) == -1)
    {
        exit(-1);
    }

    char buf[MAX_BUF_SIZE];
    if (read(pdesk, buf, MAX_BUF_SIZE) == -1)
    {
    }

    printf("%s", buf);

    close(pdesk);
    close(sdesc);

    unlink(fifo_path);

    return 0;
}
