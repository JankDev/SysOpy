#include <stdlib.h>
#include <unistd.h>
#include <aio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#define BUFFSIZE 1024

static void print_content(const char *name);

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./aio file");
        exit(EXIT_FAILURE);
    }

    print_content(argv[1]);
    return 0;
}

static void print_content(const char *name)
{
    printf("\n\nWould you like to read %s content?[y/n]", name);
    char if_read;
    scanf("%c", &if_read);

    if (if_read == 'n')
    {
        return;
    }

    int fd;
    ssize_t bytes;
    char *buff[BUFFSIZE];

    fd = open(name, O_RDONLY);
    if (fd == -1)
    {
        perror("Unable to open this file!");
        exit(EXIT_FAILURE);
    }

    struct aiocb *cb;
    cb = malloc(sizeof(struct aiocb));

    cb->aio_fildes = fd;
    cb->aio_offset = 0;
    cb->aio_buf = buff;
    cb->aio_nbytes = sizeof(buff);

    if (aio_read(cb) == -1)
    {
        close(fd);
        exit(EXIT_FAILURE);
    }

    while (aio_error(cb) == EINPROGRESS)
    {
        printf("%s", (char *)cb->aio_buf);
    }
    ssize_t bread;
    if ((bread = aio_return(cb)) == -1)
    {
        perror(NULL);
    }
    else
    {
        printf("\nBytes read: %ld\n", bread);
    }
    close(fd);
}