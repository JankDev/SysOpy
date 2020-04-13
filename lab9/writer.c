#include "rw.h"

int main(int argc, char const *argv[])
{
    int semid = get_sem();

    while (1)
    {
        decrease(semid, 0);
        printf("pid=%d writing...\n", getpid());
        increase(semid, 0);
    }

    return 0;
}
