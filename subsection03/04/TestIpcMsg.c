/*
 * file:        TestIpcMsg.c
 * func:        system msg ipc
 * author:      liuxueneng@airfly
 * date:        20190417
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <wait.h>
#include <errno.h>

#if 0

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */
};

#endif

#define         IPC_KEY         (0x66)
#define         IPC_PATH        "/tmp"
#define         BUF_SIZE        (128)

#define         FATHER_TYPE        (0x88)
#define         CHILD_TYPE         (0x99)

typedef struct ipcMsg_s {
    long type;
    char buf[BUF_SIZE];
}ipcMsg_t;

#if 0
static int CheckFile(const char *file)
{
    if(NULL == file)
        return -1;

    if(0 == access(file, F_OK))
        return -2;

    int fd = open(file, O_CREATE | O_RDWR);
    if (fd < 0) {
        return -3;
    }
    close(fd);

    return 0;
}
#endif

static void SendAndRecv(int msgId)
{
    ipcMsg_t tmpMsg;
    tmpMsg.type = FATHER_TYPE;

        snprintf(tmpMsg.buf, BUF_SIZE, "%s","Hello, My boy");
        if (msgsnd(msgId,&tmpMsg,BUF_SIZE,0) < 0) {
            printf("msgsnd failed\n");
        }
        if (msgrcv(msgId,&tmpMsg,BUF_SIZE,CHILD_TYPE,0) < 0) {
            printf("msgrcv failed \n");
        }
        else {
            printf("parent success\n");
        }
}

static void RecvAndSend(int msgId)
{
    ipcMsg_t tmpMsg;
    tmpMsg.type = CHILD_TYPE;

    if (msgrcv(msgId,&tmpMsg,BUF_SIZE,FATHER_TYPE,0) < 0) {
        printf("msgrcv failed \n");
    }
    else {
        printf("child success\n");
    }

    tmpMsg.type = CHILD_TYPE;
    snprintf(tmpMsg.buf, BUF_SIZE, "%s","Hello, this is child");
    if (msgsnd(msgId,&tmpMsg,BUF_SIZE,0) < 0) {
        printf("msgsnd failed\n");
    }
}


int main()
{
    key_t ipcKey = ftok(IPC_PATH,IPC_KEY);
    if (ipcKey < 0) {
        printf("ftok failed:%s\n",strerror(errno));
        return -1;
    }

    int msgId = msgget(ipcKey, IPC_CREAT|IPC_EXCL|0666);
    if(msgId < 0){
        printf("msgget failed:%s\n",strerror(errno));
        return -2;
    }

    pid_t pid = fork();
    if(pid < 0){
        printf("fork failed\n");
        goto ERROR1;
    }

    if (pid > 0) {
        //this is father
        SendAndRecv(msgId);

        waitpid(pid,NULL,0);
    }
    else if (0 == pid) {
        //this is child 
        RecvAndSend(msgId);
        msgctl(msgId,IPC_RMID,NULL);

        exit(EXIT_SUCCESS);
    }

    msgctl(msgId,IPC_RMID,NULL);

    return 0;
ERROR1:
    msgctl(msgId,IPC_RMID,NULL);
ERROR:
    return -1;
}

