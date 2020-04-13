#include "rw.h"

int main(int argc, char const *argv[])
{
    int semid = get_sem();

    for(int i=0;;i++)
    {
        if (is_readable(semid, 0))
        {
            printf("pid=%d reading... %d\n",getpid(),i);
        }
    }

    return 0;
}
