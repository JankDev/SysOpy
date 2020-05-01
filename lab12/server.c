/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#define MYPORT 8000 // the port users will be connecting to
#define BUF_SIZE 100
#define BACKLOG 10 // how many pending connections queue will hold

void sigchld_handler(int s)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}
bool handle_request(int client_desc, struct sockaddr_in their_addr);

int main(void)
{
	int sockfd, new_fd;			   // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;	   // my address information
	struct sockaddr_in their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET;		  // host byte order
	my_addr.sin_port = htons(MYPORT);	  // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1)
	{
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	sin_size = sizeof their_addr;
	while (true)
	{
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
		{
			perror("accept");
		}

		if (fork() == 0)
		{
			handle_request(new_fd, their_addr);
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}

bool handle_request(int client_desc, struct sockaddr_in their_addr)
{
	ssize_t nread;
	char buf[BUF_SIZE];
	printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
	while ((nread = recv(client_desc, buf, BUF_SIZE, 0)) > 0 && strncmp("quit", buf, 4) != 0)
	{
		if (nread == 0)
			break;
		if (send(client_desc, buf, sizeof(buf), 0) == -1)
			perror("send");
	}
	close(client_desc);
}