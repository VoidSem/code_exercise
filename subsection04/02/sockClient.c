/*
 * name:sockClient.c
 * Description: sock communicate test client
 * author:liuxueneng@iairfly
 * date:20190419
 *
 */


#include "sockCommon.h"


#if 1

int main(int argc,char *argv[])
{
    int sockFd = -1;
    int numBytes = 0;
    char buf[BUFSIZ] = {};
    struct sockaddr_in serverAddr;
    int len =  sizeof(struct sockaddr);
    int ret = 0;

    sockFd = socket(AF_INET,SOCK_STREAM,0);
    if (sockFd < 0) {
        printf("socket failed %s", strerror(errno));
        return -1;
    }

    printf("client get the sockFd\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SOCK_PORT);
    serverAddr.sin_addr.s_addr=inet_addr(LOOP_BACK_ADDR);
    bzero(&(serverAddr.sin_zero), 8);

    ret = connect(sockFd,(struct sockaddr*)&serverAddr,len);
    if (ret < 0) {
        printf("connect failed %s\n",strerror(errno));
        goto ERROR1;
    }
    printf("Connect to Server ok !\n");

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "ping");
    numBytes = send(sockFd, buf, strlen(buf), 0);
    if(numBytes < 0) {
        printf("send failed %s\n",strerror(errno));
    }

    numBytes = recv(sockFd, buf, BUFSIZ,0);
    if(numBytes < 0) {
        printf("recv failed %s\n",strerror(errno));
        goto ERROR1;
    }
    else
    {
        buf[numBytes]='\0';
        printf("Client get:%s\n",buf);
    }
    close(sockFd);
    return 0;

ERROR1:
    close(sockFd);
ERROR:
    return -1;
}
#endif
