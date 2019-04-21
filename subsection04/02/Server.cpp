/*
 * name:Server.cpp
 * Description: sock communicate test server
 * author:liuxueneng@iairfly
 * date:20190419
 *
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
    // epool fd
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

    int ret = bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        perror("bind error");
        return -2;
    }

    ret = listen(socketFd, backLog);
    if(ret < 0) {
        perror("listen error");
        return -3;
    }

    epFd = epoll_create (EPOLL_SIZE);
    if(epFd < 0) {
        perror("epFd error");
        return -4;
    }
    //往事件表里添加监听事件
    AddEpollFd(epFd, socketFd, true);
}

void Server::Close()
{
    list<int>::iterator iter;

    for(iter = clientList.begin(); iter != clientList.end();/*nothing*/) {
        epoll_ctl(epFd, EPOLL_CTL_DEL, *iter, NULL);
        close(*iter);
        iter = clientList.erase(iter);
    }

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

    AddEpollFd(epFd, clientFd, true);
    clientList.push_back(clientFd);
    //cout <<"Add "<< clientFd <<endl;
    return 0;
}

int Server::DeleteClient(int fd)
{
    epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    clientList.remove(fd);
    return 0;
}

int Server::ClientHandle(int fd) {
    char buf[BUFSIZ] = {};
    socketMsg_t msg = {};
    msg.size = BUFSIZ;
    msg.data = buf;

    //cout <<"handle "<< fd <<endl;
    int ret = CommonRecvMsg(fd, &msg);
    if (ret < 0) {
        DeleteClient(fd);
        return -1;
    }

    cout<< msg.data <<endl;

    if (0 == strcmp(msg.data, COMMON_EXIT_STRING)) {
        cout <<"get exit string" <<endl;
        return 1;
    }

    sprintf(msg.data, SEND_CLIENT_STRING);
    int len = strlen(msg.data) + 1;
    msg.size = (unsigned int)len;

    ret = CommonSendMsg(fd, &msg);
    if (ret < len) {
        DeleteClient(fd);
        return -1;
    }
    return 0;
}


int Server::Start()
{
    struct epoll_event epEvents[EPOLL_SIZE] = {};
    if (!(socketFd  > 0 && socketFd > 0)) {
        cerr<<"Please Init it first!"<<endl;
        return -1;
    }
    cout<<"start epoll wait..."<<endl;
    while (1)
    {
        //blocked
        int timeOut = -1;
        int eventNum = epoll_wait(epFd, epEvents, EPOLL_SIZE, timeOut);

        if(eventNum < 0) {
            perror("epoll failure");
            break;
        }

        //cout << "eventNum =\n" << eventNum << endl;
        //handle epEvents
        for(int i = 0; i < eventNum; ++i) {
            int tmpFd = epEvents[i].data.fd;
            if ((epEvents[i].events & EPOLLERR) || (epEvents[i].events & EPOLLRDHUP)) {
                if(tmpFd == socketFd) {
                    cerr << "socket wrong" <<endl;
                    epoll_ctl(epFd, EPOLL_CTL_DEL, tmpFd, NULL);
                    return -1;
                }
                else {
                    DeleteClient(tmpFd);
                    //cout <<"Delete "<< tmpFd <<endl;
                    continue;
                }
            }
            else if (epEvents[i].events & EPOLLIN) {
                if(tmpFd == socketFd) {//client connect
                    int ret = AcceptHandle();
                    if (ret < 0) {
                        cerr<< strerror(errno)<<endl;
                        return -2;
                    }
                }
                else {  //client msg
                    int ret = ClientHandle(tmpFd);
                    if(ret < 0) {
                        cerr<< strerror(errno)<<endl;
                    }
                    else if(ret == 1) {
                        return 0;
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
