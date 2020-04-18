#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int term_desc;

    if ((term_desc = open(argv[1], O_RDWR)) == -1)
    {
        perror("Error opening term desc");
    }

    write(term_desc, "Hello terminal", 20);

    close(term_desc);
    return 0;
}
