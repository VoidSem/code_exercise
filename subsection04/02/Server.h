/*
 * Name: Server.h
 * Description: Server header
 * Author:liuxueneng@iairfly
 * Date:20190419
 */

#ifndef SERVER_H
#define SERVER_H

#include "Common.h"

class Server {
    public:
        Server(int SERVER_PORT, const char *SERVER_IP, int  backLog);
        /*create socket and bind it*/
        int Init();

        /*start monitor accept event and clients events*/
        int Start();

        /*close socket and epoll*/
        void Close();

    private:
        /*handle accept events*/
        int AcceptHandle();

        /*handle client events read msg..*/
        int ClientHandle(int fd);

        /*when the client is wrong do this*/
        int DeleteClient(int fd);

        struct sockaddr_in serverAddr;

        /*socket*/
        int socketFd;

        /*for listen*/
        int backLog;

        /*epoll*/
        int epFd;

        /*save the clients*/
        std::list<int> clientList;
};
#endif /* SERVER_H*/
