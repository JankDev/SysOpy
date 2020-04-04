#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define String char *

#define BUFF_SIZE 300

static int num_lines;

int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Only 1 argument allowed\n");
        exit(-1);
    }

    const String fname = argv[1];
    int fd;

    if ((fd = open(fname, O_RDONLY)) == -1)
    {
        perror("Error opening file");
        exit(1);
    }

    int pdesc[2], resdesc[2];
    char rbuff[2];

    char file_content[BUFF_SIZE];
    size_t line_size = BUFF_SIZE;

    if (pipe(pdesc) == -1 || pipe(resdesc) == -1)
    {
        perror("Error creating pipe");
        exit(1);
    }

    switch (fork())
    {
    case -1:
        perror("Error forking");
        exit(1);
    case 0:
        close(pdesc[1]);
        dup2(pdesc[0], 0);
        dup2(resdesc[1], 1);
        close(pdesc[0]);
        close(resdesc[1]);
        execlp("wc", "wc", "-l", NULL);
    default:
        close(pdesc[0]);
        read(fd, file_content, BUFF_SIZE);
        write(pdesc[1], file_content, BUFF_SIZE);
        close(pdesc[1]);
    }
    read(resdesc[0], rbuff, 1);
    printf("Number of num_lines in file %s = %s ", fname, rbuff);
    close(fd);
    return 0;
}
