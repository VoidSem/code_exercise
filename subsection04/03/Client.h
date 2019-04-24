/*
 * Name:        Client.h
 * Description: client headers
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      20190423
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "Common.h"

class Client {
    public:
        Client(int port, const char *ip);
        /*create socket and connect to server*/
        int Init();

        /*send msg to the server*/
        int SendMsg(const char *buf, size_t len, int mode);

        /*recv msg from the server*/
        int RecvMsg(char *buf, size_t len, int mode);

        /*get client socket fd*/
        int GetClient(void);

        virtual ~Client();

    private:
        /*socket fd*/
        int sockFd;

        /*socket server addr*/
        struct sockaddr_in serverAddr;
};
#endif /* CLIENT_H*/
