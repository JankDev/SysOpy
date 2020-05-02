#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <stdbool.h>

#define string char *
#define MAX_MSG_SIZE 100

static inline void ensur_args(int argc)
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: ./udpchat hostname port otherhost otherport\n");
        exit(EXIT_FAILURE);
    }
}

static inline void no_err(int code)
{
    if (code == -1)
    {
        perror("err");
        exit(EXIT_FAILURE);
    }
}

static bool read_msg(char *msg);
static void setsockaddr(struct sockaddr_in *addr, const string hostname, const string port);

static void reader_proc(struct sockaddr_in *you, struct sockaddr_in *them, int sockfd);
static void writer_proc(struct sockaddr_in *you, struct sockaddr_in *them, int sockfd);

int main(int argc, char const *argv[])

{
    ensur_args(argc);

    struct sockaddr_in your_addr;
    struct sockaddr_in thei_addr;

    const string hostname = argv[1];
    const string port = argv[2];
    const string otherhostname = argv[3];
    const string otherport = argv[4];
    int sockfd;

    setsockaddr(&your_addr, hostname, port);
    setsockaddr(&thei_addr, otherhostname, otherport);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd, (struct sockaddr *)&your_addr, sizeof(your_addr)) == -1)
    {
        perror(" ");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
    case -1:
        perror("fork ");
        exit(EXIT_FAILURE);
        break;
    case 0: //reader
        reader_proc(&your_addr, &thei_addr, sockfd);
        exit(EXIT_SUCCESS);
    default: //writer
        writer_proc(&your_addr, &thei_addr, sockfd);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

static bool read_msg(char *msg)
{
    if (fgets(msg, sizeof(msg), stdin) == NULL)
        return false;
    return true;
}

static void setsockaddr(struct sockaddr_in *addr, const string hostname, const string port)
{
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(hostname);
    addr->sin_port = htons(atoi(port));

    memset(addr->sin_zero, 0, sizeof(addr->sin_zero));
}

static void reader_proc(struct sockaddr_in *you, struct sockaddr_in *them, int sockfd)
{
    char msg[MAX_MSG_SIZE];
    ssize_t nrecv;
    socklen_t sa_len;

    while (true)
    {
        sa_len = sizeof(*them);
        no_err(nrecv = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)them, &sa_len));

        if (nrecv == sizeof(msg))
        {
            printf("Them: %s\n", msg);
        }
        if (strncmp("break", msg, 5) == 0)
        {
            break;
        }
        sleep(1);
    }
    close(sockfd);
}

static void writer_proc(struct sockaddr_in *you, struct sockaddr_in *them, int sockfd)
{
    char msg[MAX_MSG_SIZE];

    while (true)
    {
        read_msg(msg);
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)them, sizeof(*them));
        __fpurge(stdin);
        printf("You: %s\n", msg);
        if (strncmp("break", msg, 5) == 0)
        {
            break;
        }
    }

    close(sockfd);
}