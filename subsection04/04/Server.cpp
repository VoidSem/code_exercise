/*
 * Name:        Server.cpp
 * Description: sock communicate test server
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      20190423
 */

#include "Server.h"

using namespace std;

Server::Server(int port, const char *ip, int backLog)
{
    //init socket addr
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    this->backLog = backLog;
    socketFd = 0;
    epFd = 0;
}

//init socket
int Server::Init()
{
    socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        perror("socketFd");
        return -1;
    }

    //ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    int ret = bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        perror("bind error");
        return -2;
    }

    if(ret < listen(socketFd, backLog)) {
        perror("listen error");
        return -3;
    }

    epFd = epoll_create(EPOLL_SIZE);
    if(epFd < 0) {
        perror("epFd error");
        return -4;
    }

    //add fd to epoll
    struct epoll_event ev;
    ev.data.fd = socketFd;
    ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
    epoll_ctl(epFd, EPOLL_CTL_ADD, socketFd, &ev);

    return 0;
}

Server::~Server()
{
    if (epFd > 0) {
        close(epFd);
    }
    epFd = 0;
    if(socketFd > 0) {
        close(socketFd);
    }
    socketFd = 0;
}

int Server::AcceptHandle()
{
    struct sockaddr_in client_address;
    socklen_t len = 0;

    int clientFd = accept(socketFd, ( struct sockaddr* )&client_address, &len);
    if (clientFd < 0) {
        return -1;
    }

    //add fd to epoll
    struct epoll_event ev;
    ev.data.fd = clientFd;
    ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
    epoll_ctl(epFd, EPOLL_CTL_ADD, clientFd, &ev);

#ifdef  TEST_DEBUG
    static int c = 0;
    cout <<"Add "<< ++c<<endl;
#endif
    return 0;
}

void Server::DeleteClient(int fd)
{
    epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

int Server::ClientHandle(int fd) {
    char buf[BUFSIZ] = {};

    int ret = recv(fd, buf, BUFSIZ, 0);
    if (ret < 0) {
        DeleteClient(fd);
        return -1;
    }

    cout<< buf<<endl;

    /* if get ping send pong  */
    if (0 == strcmp(buf, SEND_STRING_PING)) {
        int len = strlen(SEND_STRING_PONG) + 1;
        ret = send(fd, SEND_STRING_PONG, len, 0);
        if (ret < len) {
            DeleteClient(fd);
            return -1;
        }
    }
    return 0;
}


int Server::Start()
{
    struct epoll_event epEvents[EPOLL_SIZE] = {};
    int timeOut = -1;
    cout<<"start epoll wait..."<<endl;
    while (1)
    {
        //blocked
        int eventNum = epoll_wait(epFd, epEvents, EPOLL_SIZE, timeOut);
        if(eventNum < 0) {
            perror("epoll failure");
            return -1;
        }

        //handle epEvents
        for(int i = 0; i < eventNum; ++i) {
            int tmpFd = epEvents[i].data.fd;
            if ((epEvents[i].events & EPOLLERR) || (epEvents[i].events & EPOLLRDHUP)) {
                DeleteClient(tmpFd);
                if(tmpFd == socketFd) {
                    return -1;
                }
            }
            else if (epEvents[i].events & EPOLLIN) {
                if(tmpFd == socketFd) {//client connect
                    if (AcceptHandle() < 0) {
                        cerr<< strerror(errno)<<endl;
                        return -2;
                    }
                }
                else {  //client msg
                    if(ClientHandle(tmpFd) < 0) {
                        cerr<< strerror(errno)<<endl;
                    }
                }
            }
            else {
                cout<< "!!UNKNOWN events"<<endl;
            }
        }
    }
    return 0;
}
