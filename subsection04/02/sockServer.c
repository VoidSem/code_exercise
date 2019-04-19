/*
 * name:sockServer.c
 * Description: sock communicate test server
 * author:liuxueneng@iairfly
 * date:20190419
 *
 */

#include "sockCommon.h"


int main(int argc, char *argv[])
{
    int fd;
    int newFd;
    int addrLen = sizeof(struct sockaddr_in);
    int nBytes = 0;
    int i = 0;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char buff[BUFSIZ];

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SOCK_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serverAddr.sin_zero), 8);
    addrLen = sizeof(struct sockaddr_in);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("socket failed\n");
        return -1;

    }
    if (bind(fd, (struct sockaddr *)&serverAddr, addrLen) < 0) {
        printf("bind failed %s\n",strerror(errno));
        goto ERROR1;
    }

    printf("Bind Success!\n");
    if (listen(fd, MAX_CLIENT) < 0) {
        printf("listen failed\n");
        goto ERROR1;
    }
    printf("Ready for Accept\n");

    do {
        newFd = accept(fd, (struct sockaddr *)&clientAddr, &addrLen);
        printf("Get the Client.\n");
        while((nBytes = recv(newFd, buff, BUFSIZ, 0)) > 0)
        {
            buff[nBytes] = '\0';
            printf("%s\n",buff);
            sprintf(buff,"pong");
            if(send(newFd,buff,nBytes,0)<0)
            {
                printf("write failed\n");
                break;
            }
        }
    } while(0);

    close(newFd);
    close(fd);
    return 0;

ERROR1:
    close(fd);
ERROR:
    return -1;
}
