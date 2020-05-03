#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// ----------------------------------------------------------

#define MAXVAL 100

int globalvariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *increment(void *);
void *printinfo(void *);
// ----------------------------------------------------------

int main()
{

    pthread_t t1, t2, t3;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&t1, &attr, increment, NULL);
    pthread_create(&t2, &attr, increment, NULL);
    pthread_create(&t3, &attr, printinfo, NULL);

    pthread_join(t1, NULL);
    printf("t1 finished!\n");
    pthread_join(t2, NULL);
    printf("t2 finished!\n");
    pthread_join(t3, NULL);
    printf("t3 finished!\n");

    printf("Finishing...\n");
    return 0;
}
// ----------------------------------------------------------

void *increment(void *arg)
{
    while (1)
    {
        if (pthread_mutex_lock(&mutex) != 0)
        {
            fprintf(stderr, "Error locking mutext");
        }
        if (globalvariable == MAXVAL)
        {

            if (pthread_mutex_unlock(&mutex) != 0)
            {
                fprintf(stderr, "Error unlocking mutex");
            }
            if (pthread_cond_signal(&cond) != 0)
            {
                fprintf(stderr, "Error signaling");
            };
            break;
        }
        globalvariable++;
        if (pthread_mutex_unlock(&mutex) != 0)
        {
            fprintf(stderr, "Error unlocking mutex");
        }
    }

    pthread_exit((void *)0);
}
// ----------------------------------------------------------

void *printinfo(void *arg)
{
    if (pthread_mutex_lock(&mutex) != 0)
    {
        fprintf(stderr, "Error locking mutext");
    }
    while (globalvariable != MAXVAL)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    fprintf(stdout, "The variable has reached max value\n");
    if (pthread_mutex_unlock(&mutex) != 0)
    {
        fprintf(stderr, "Error unlocking mutex");
    }
    pthread_exit((void *)0);
}
// ----------------------------------------------------------