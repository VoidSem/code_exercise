/*
 * name:Client.cpp
 * Description: sock communicate test client
 * Author:liuxueneng@iairfly
 * Date:20190419
 *
 */


#include "Client.h"

using namespace std;

Client::Client(int port, const char *ip)
{
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    sockFd = 0;
    epFd = 0;
}

int Client::Init()
{
    sockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockFd < 0) {
        perror("sock error");
        return -1;
    }
    return 0;
}

//connect server
int Client::Connect()
{
    if (0 == sockFd) {
        cerr<<"Please init  first"<<endl;
        return -1;
    }

    int addrLen = sizeof(serverAddr);
    if(connect(sockFd, (struct sockaddr *)&serverAddr, addrLen) < 0) {
        perror("connect error");
        return -1;
    }
    return 0;
}

void Client::Close()
{
    close(sockFd);
}

int Client::SendMsg(const socketMsg_t *msg)
{
    return CommonSendMsg(sockFd, msg);
}

int Client::RecvMsg(socketMsg_t *msg)
{
    return CommonRecvMsg(sockFd, msg);
}
