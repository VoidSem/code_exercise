/*
 * Name: Client.h
 * Description: client headers
 * Author: liuxueneng@iairfly
 * Date: 20190419
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "Common.h"

class Client {
public:
    Client(int SERVER_PORT, const char *SERVER_IP);
    /*create socket*/
    int Init();
    /*connet to the server*/
    int Connect();
    /*close the socket*/
    void Close();

    /*send msg to the server*/
    int SendMsg(const socketMsg_t *msg);

    /*recv msg from the server*/
    int RecvMsg(socketMsg_t *msg);

private:

    /*socket fd*/
    int sockFd;
    /*epoll fd*/
    int epFd;
    /*socket server addr*/
    struct sockaddr_in serverAddr;
};
#endif /* CLIENT_H*/
