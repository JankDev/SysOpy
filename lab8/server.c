#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "util.h"

const char *fifo_path = "server_fifo";

char *read_client_requests(int fdesc);
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

    if (fdesc == -1)
    {
        perror("Error opening fifo");
        printf("Server exiting...");
        exit(-1);
    }

    char *cmd;
    int cdesc = open("client_fifo", O_WRONLY);

    cmd = read_client_requests(fdesc);

    execute_request(cmd, cdesc);

    close(cdesc);
    close(fdesc);
    
    unlink(fifo_path);

    return 0;
}

char *read_client_requests(int fdesc)
{
    char *buf = malloc(sizeof(char) * MAX_BUF_SIZE);

    if (read(fdesc, buf, MAX_BUF_SIZE) == -1)
    {
        perror("Error reding from fifo");
        exit(-1);
    }

    return buf;
}

void execute_request(char *cmd, int cdesc)
{

    switch (fork())
    {
    case -1:
        perror("Error forking");
        exit(-1);
    case 0:
        printf("In child");
        close(1);
        dup2(cdesc, 1);
        execlp(cmd, cmd, NULL);
        exit(1);
    }
}