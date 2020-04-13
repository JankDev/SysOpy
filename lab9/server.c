#include "queue_util.h"

int main(int argc, char const *argv[])
{
    msq_buf buf;

    int msgid = connect_to_queue();

    fill_empty_queue(msgid);

    char cnt_msg[MAX_TEXT_SIZE];

    while (1)
    {
        puts("Waiting for messages");

        if (msgrcv(msgid, &buf, sizeof(buf), FULL, 0) == -1)
        {
            perror("Error receiving full message");
        }

        strcpy(cnt_msg, buf.mtext);

        if (msgrcv(msgid, &buf, sizeof(buf), EMPTY, 0) == -1)
        {
            perror("Error receiving full message");
        }

        to_upper_str(cnt_msg);
        strcpy(buf.mtext, cnt_msg);
        buf.mtype = FULL;

        if ((msgsnd(msgid, &buf, sizeof(buf), 0)) == -1)
        {
            perror("Error sending message");
            exit(-1);
        }
    }

    return 0;
}
