#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
        int pfd[2];
        pid_t pid;
        char string[] = "Test";
        char buf[10];

        int a = pipe(pfd);
        if (a == -1)
        {
                perror("An error occurred");
        }
        pid = fork();

        if (pid == 0)
        {
                close(pfd[1]);
                close(0);
                dup2(pfd[0],0);
                read(STDIN_FILENO, buf, sizeof(buf));
                printf("Wypisuje: %s", buf);
        }
        else
        {
                close(pfd[0]);
                close(STDOUT_FILENO);
                dup2(pfd[1],STDOUT_FILENO);
                write(pfd[1], string, (strlen(string) + 1));
        }

        return 0;
}
