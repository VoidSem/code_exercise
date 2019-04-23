/*
 * Name:        Client.cpp
 * Description: sock communicate test client
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      20190423
 */

#include "Client.h"

using namespace std;

Client::Client(int port, const char *ip)
{
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    sockFd = 0;
}


/*create socket and connet to server*/
int Client::Init()
{
    sockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockFd < 0) {
        perror("sock error");
        return -1;
    }

    int addrLen = sizeof(serverAddr);
    if(connect(sockFd, (struct sockaddr *)&serverAddr, addrLen) < 0) {
        perror("connect error");
        return -1;
    }
    return 0;
}


Client::~Client()
{
    if (sockFd > 0) {
        close(sockFd);
    }
}

int Client::SendMsg(const char *buf, size_t len, int mode)
{
    return send(sockFd, buf, len, mode);
}

int Client::RecvMsg(char *buf, size_t len, int mode)
{
    return recv(sockFd, buf, len, mode);
}
