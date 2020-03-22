// stdio dla printa
#include <stdio.h>
// stdlib dla kodów EXIT
#include <stdlib.h>
// fcntl zawiera flagi O_*
#include <fcntl.h>
// unistd definiuje fujnkcje read itd.
#include <unistd.h>
// errno zapewnia zmienną
#include <errno.h>
#include "time.h"

// Rozmiar bufora
#define BUFSIZE 1024

int main (int argc, char **argv) {
    // fp - file pointer - deskryptor
		int fp, chars, bytes;
    char buff[BUFSIZE], *fname;
	  // buffor buff jest tablica znakow o rozmiarze BUFSIZE=1024

    chars = 10;
    fname= argv[1];

    timestart();
    fp = open(fname, O_RDONLY); // O_RDONLY -> flaga mówiąca, że plik jest tylko do odczytu
    bytes = read(fp, buff, chars);

    timestop("czas wykonania");
    printf("%s: Spróbowano odczytać %d znakow z pliku %s: \"%d\"\n",
	    argv[0], chars, fname, bytes);

    if(errno) {
        perror("Nastąpił błąd programu");
        exit(EXIT_FAILURE);
    } else {
        close(fp);
    }

    return 0;
}
