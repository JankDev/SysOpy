/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>

#define PORT "8000" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; // connector's address information

	struct addrinfo hint, *infop;
	int get_add_err_code;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	if (argc != 2)
	{
		fprintf(stderr, "usage: client hostname\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if ((get_add_err_code = getaddrinfo(argv[1], PORT, &hint, &infop)) < 0)
	{
		fprintf(stderr, "addrinfo %s", gai_strerror(get_add_err_code));
		exit(-1);
	}

	for (; infop != NULL; infop = infop->ai_next)
	{
		struct sockaddr_in *sa = (struct sockaddr_in *)infop->ai_addr;
		their_addr.sin_addr = sa->sin_addr;
	}

	their_addr.sin_family = AF_INET;		 // host byte order
	their_addr.sin_port = htons(atoi(PORT)); // short, network byte order
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1)
	{
		perror("connect");
		exit(1);
	}

	char msg[MAXDATASIZE];

	fflush(stdin);
	while (true)
	{
		printf("You:\t");
		fgets(msg, sizeof(msg), stdin);
		puts(msg);
		if (strncmp("quit", msg, 4) == 0)
		{
			printf("Quitting...");
			break;
		}
		if (send(sockfd, msg, sizeof(msg), 0) == -1)
		{
			perror("send");
			exit(EXIT_FAILURE);
		}

		if ((numbytes = recv(sockfd, buf, sizeof(msg), 0)) == -1)
		{
			perror("recv");
			exit(1);
		}

		buf[numbytes] = '\0';

		printf("Received: %s\n", buf);
		fflush(stdin);
	}

	close(sockfd);

	return 0;
}