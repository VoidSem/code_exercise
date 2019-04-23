/*
 * Name:        Server.h
 * Description: Server header
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      2190423
 */

#ifndef SERVER_H
#define SERVER_H

#include "Common.h"

class Server {
    public:
        Server(int port, const char *ip, int  backLog);
        /*create socket and bind it*/
        int Init();

        /*start monitor accept event and clients events*/
        int Start();

        virtual ~Server();
    private:
        /*handle accept events*/
        int AcceptHandle();

        /*handle client events read msg..*/
        int ClientHandle(int fd);

        /*when the client is wrong do this*/
        void DeleteClient(int fd);

        struct sockaddr_in serverAddr;

        /*socket*/
        int socketFd;

        /*for listen*/
        int backLog;

        /*epoll*/
        int epFd;
};
#endif /* SERVER_H*/
