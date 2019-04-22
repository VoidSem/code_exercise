/*
 * Name: Server.h
 * Description: Server header
 * Author:liuxueneng@iairfly
 * Date:20190419
 */

#ifndef SERVER_H
#define SERVER_H

#include "Common.h"
#include "Client.h"
#include <mutex>
#include <condition_variable>

#define THREAD_READ 1

class Server {
    public:
        Server(int SERVER_PORT, const char *SERVER_IP, int  backLog);
        /*create socket and bind it*/
        int Init();

        /*start monitor accept event and clients events*/
        int Start();

        /*close socket and epoll*/
        void Close();
	int acceptRun;
	virtual ~Server();

    private:
	std::thread *acceptThread;
	Client *notifyClient;
	int notifyFd;
	/*accept thread*/
        static int AcceptThread(void *arg);

        /*handle accept events*/
        int AcceptHandle(void);

	std::thread *clientThread;
        /**/
        std::condition_variable     handleCond;

        /*mutext*/
        std::mutex mtx;
        std::mutex handleMutex;

        /*need handle clients list*/
        std::list<int> readyList;

        void AddHandleList(int fd);

        int GetHandleList();

	/*client thread*/
        static int ClientThread(void *arg);

        /*handle client events read msg..*/
        int ClientHandle(int fd);

        int DoClientHandle();

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
