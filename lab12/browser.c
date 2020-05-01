#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define GET_REQUEST "GET /%s HTTP/1.0\r\n\r\n"
#define HTTP_PORT "80"

typedef char *string;

static bool connect_to_server(string url, int *sdesc);

static inline void cleanup(struct addrinfo *a)
{
    freeaddrinfo(a);
}

int main(void)
{
    char url[100], s[500], *path = "", *p;
    int fd;
    ssize_t nread;

    while (true)
    {
        printf("URL: ");
        if (fgets(url, sizeof(url), stdin) == NULL)
            break;
        if ((p = strrchr(url, '\n')) != NULL)
            *p = '\0';
        if ((p = strchr(url, '/')) != NULL)
        {
            path = p + 1;
            *p = '\0';
        }

        if (connect_to_server(path, &fd))
        {

            snprintf(s, sizeof(s), GET_REQUEST, p);
            puts(s);
            if ((nread = send(fd, s, sizeof(s), 0)) == -1)
            {
                perror("send");
                continue;
            }

            if ((nread = recv(fd, s, sizeof(s), 0)) == -1)
            {
                perror("read");
                continue;
            }

            printf("%s", s);
        }
    }
}
static bool connect_to_server(string url, int *sdesc)
{
    int err;
    struct addrinfo hint, *info;
    struct sockaddr_in in;

    puts(url);
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(url, HTTP_PORT, &hint, &info)) < 0)
    {
        fprintf(stderr, "addrinfo %s", gai_strerror(err));
        return false;
    }
    puts("im here");
    in.sin_family = AF_INET;              // host byte order
    in.sin_port = htons(atoi(HTTP_PORT)); // short, network byte order
    memset(in.sin_zero, '\0', sizeof in.sin_zero);

    while (info == NULL)
    {
        info = info->ai_next;
    }
    if (info == NULL)
    {
        fprintf(stderr, "no information");
    }
    in.sin_addr = ((struct sockaddr_in *)info)->sin_addr;

    printf("%s", inet_ntoa(in.sin_addr));

    if ((*sdesc = socket(AF_INET, SOCK_STREAM, info->ai_protocol)) == -1)
    {
        perror("socket");
        cleanup(info);
        return false;
    }

    if (connect(*sdesc, (struct sockaddr *)&in, sizeof(in)) == -1)
    {
        perror("connect");
        cleanup(info);
        return false;
    }

    cleanup(info);
    return true;
}