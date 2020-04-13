#include "queue_util.h"

int main(int argc, char const *argv[])
{
    msq_buf buf;

    int msgid = connect_to_queue();

    while (1)
    {

        if (msgrcv(msgid, &buf, sizeof(buf), EMPTY, 0) == -1)
        {
            perror("Error receiving full message");
        }

        puts("Type your message:");
        scanf("%s", buf.mtext);
        buf.mtype = FULL;

        if (strcmp("quit", buf.mtext) == 0)
        {
            exit(1);
        }

        if (msgsnd(msgid, &buf, sizeof(buf), 0) == -1)
        {
            perror("Error sending message");
            exit(-1);
        }

        if (msgrcv(msgid, &buf, sizeof(buf), FULL, 0) == -1)
        {
            perror("Error reading full");
            exit(-1);
        }

        printf("Transformed message %s\n", buf.mtext);
    }

    return 0;
}
