#ifndef QUEUE_UTIL
#define QUEUE_UTIL

#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define QUEUE_KEY 42187
#define S_IRWUSRGRP 0660

#define MAX_QUEUE_SIZE 10
#define EMPTY 1
#define FULL 2
#define MAX_TEXT_SIZE 15

typedef struct msq_buf
{
    long mtype;
    char mtext[MAX_TEXT_SIZE];
} msq_buf;

int connect_to_queue()
{
    int msgid;

    if ((msgid = msgget(QUEUE_KEY, IPC_CREAT | S_IRWUSRGRP)) == -1)
    {
        perror("Err connecting to queue");
        exit(-1);
    }

    return msgid;
}

void fill_empty_queue(int msgid)
{
    msq_buf buf;
    buf.mtype = EMPTY;

    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1)
        {
            perror("Error sending message");
            exit(-1);
        }
    }
}

void to_upper_str(char *text)
{
    for (int i = 0; i < sizeof(text); i++)
    {
        text[i] = toupper(text[i]);
    }
    puts("Message transformed");
}

#endif