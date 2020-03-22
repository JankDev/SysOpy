#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <errno.h>

#include <stdlib.h>

#define BUFSIZE 512

void copy(char *from, char *to)

{

int fromfd = -1, tofd = -1;

ssize_t nread;

char buf[BUFSIZE];

if ((fromrf = open(from,O_RDONLY))=-1){

if (errno == EACCES) //Errno :: EACCES permission Denied

	perror(“First file EACCES”); //Wypisuje zrozumiały komunikat o błędzie.

else if (errno = ENOENT) //Errno::ENOENT No such file or directory

	perror(“First file ENOENT”);

else

	perror(“Second file”);

exit(EXIT_FAILURE);

while ((nread = read(fromfd, buf, sizeof(buf))) > 0)

  if(( nwrite=write(tofd, buf, nread)==-1){

perror(“write”);

exit(EXIT_FAILURE);

close(fromfd);

close(tofd);

return;

}

int main(int argc, char **argv){

if (argc != 3)

{

	[fprintf](http://www.opengroup.org/onlinepubs/009695399/functions/fprintf.html)(stderr,"usage: %s from_pathname to_pathname\n", argv[0]);

	return 1;

}

copy(argv[1], argv[2]);

return 0;}
