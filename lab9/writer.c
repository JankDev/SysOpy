#include "rw.h"

int main(int argc, char const *argv[])
{
    int semid = get_sem();

    for (int i = 0;; i++)
    {
        increase(semid, 0);
        printf("pid=%d writing...%d\n ", getpid(), i);
        printf("Time is %ld\n", time(NULL));
        sleep(3);
        decrease(semid, 0);
        sleep(1);
    }

    return 0;
}
