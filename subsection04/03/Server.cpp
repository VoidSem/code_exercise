/*
 * name:Server.cpp
 * Description: sock communicate test server
 * author:liuxueneng@iairfly
 * date:20190419
 *
 */

#include "Server.h"
#include "Client.h"

using namespace std;
#define	NO_INIT_FD	(-8)

Server::Server(int port, const char *ip, int backLog)
{
    //init socket addr
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    this->backLog = backLog;
    socketFd = 0;
    epFd = 0;
    acceptRun = 0;
    acceptThread = NULL;
    notifyFd = NO_INIT_FD;
    notifyClient = NULL;
    clientThread = NULL;
}

//init socket
int Server::Init()
{
    socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        perror("socketFd");
        return -1;
    }

    /* ignore sigpipe */
    /*else we may crash*/
    signal(SIGPIPE, SIG_IGN);

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

    notifyClient = new Client(COMMON_SERVER_PORT, COMMON_SERVER_IP);
    // notify client init
    notifyClient->Init();
    //notify client Connect
    notifyClient->Connect();

    //first get notify connect
    AcceptHandle();

    //AddEpollFd(epFd, notifyFd, true);
}

int Server::GetHandleList()
{
    int tmpFd = -1;
    list<int>::iterator fd;


    handleMutex.lock();
    if(readyList.size()) {
        fd = readyList.begin();
        tmpFd = *fd;
        readyList.erase(fd);
    }
    handleMutex.unlock();
    return tmpFd;
}

void Server::AddHandleList(int fd)
{
    handleMutex.lock();
    readyList.push_back(fd);
    handleMutex.unlock();
    unique_lock<mutex> lck(mtx);
    handleCond.notify_all();
}


void Server::Close()
{
    list<int>::iterator iter;

    for(iter = clientList.begin(); iter != clientList.end();/*nothing*/) {
        epoll_ctl(epFd, EPOLL_CTL_DEL, *iter, NULL);
        close(*iter);
        iter = clientList.erase(iter);
    }
    acceptRun = 0;

    if (epFd > 0) {
        close(epFd);
    }
    epFd = 0;
    if(socketFd > 0) {
        close(socketFd);
    }
    socketFd = 0;
    if (acceptThread) {
        acceptThread->join();
        delete acceptThread;
    }
    if (clientThread) {
        clientThread->join();
        delete clientThread;
    }
    if (notifyClient) {
        notifyClient->Close();
        delete notifyClient;
    }
}

int Server::ClientThread(void *arg)
{
    Server *svr = (Server *)arg;
    while (svr->acceptRun) {
                svr->DoClientHandle();
    }
    return 0;
}

int Server::DoClientHandle()
{
    int fd = 0;
    unique_lock<mutex> lck(mtx);
    handleCond.wait(lck);
    fd = GetHandleList();
    if (fd > 0) {
        ClientHandle(fd);
    }
}

int Server::AcceptThread(void *arg)
{
    Server *svr = (Server *)arg;
    while(svr->acceptRun) {
        svr->AcceptHandle();
    }
}

int Server::AcceptHandle()
{
    struct sockaddr_in client_address;
    socklen_t len = 0;

    int clientFd = accept(socketFd, (struct sockaddr*)&client_address, &len);
    if (clientFd < 0) {
        perror(" accept error");
        return -1;
    }

    //get first notifyFd
    if(NO_INIT_FD == notifyFd) {
        notifyFd = clientFd;
        AddEpollFd(epFd, notifyFd, true);
        return 0;
    }

    clientList.push_back(clientFd);

    char buf[BUFSIZ] = {};
    socketMsg_t msg = {};
    *((int *)buf) = clientFd;
    msg.size = sizeof(clientFd);
    msg.data = buf;
    notifyClient->SendMsg(&msg);
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

    int ret =  0;
    while ((ret = CommonRecvMsg(fd, &msg, MSG_DONTWAIT)) > 0) {
        if (ret < 0) {
            DeleteClient(fd);
            return -1;
        }
        cout<< msg.data<<endl;
    }

    if (0 == strcmp(msg.data, COMMON_EXIT_STRING)) {
        cout <<"get exit string" <<endl;
        return 1;
    }

    sprintf(msg.data, SEND_CLIENT_STRING);
    int len = strlen(msg.data) + 1;
    msg.size = (unsigned int)len;

    ret = CommonSendMsg(fd, &msg, 0);
    if (ret < len) {
        DeleteClient(fd);
        return -1;
    }
    return 0;
}



int Server::Start()
{
    struct epoll_event epEvents[EPOLL_SIZE] = {};
    if (!(socketFd  > 0 && epFd > 0)) {
        cerr<<"Please Init it first!"<<endl;
        return -1;
    }
    acceptRun = 1;
    acceptThread = new thread(AcceptThread, (void *)this);
#ifdef THTEAD_READ
    clientThread = new thread(ClientThread, (void *)this);
#endif
    static int cSum = 0;
    cout<<"start epoll wait..."<<endl;
    while (1)
    {
        //blocked
        int timeOut = -1;
        int eventNum = epoll_wait(epFd, epEvents, EPOLL_SIZE, timeOut);

        if(eventNum < 0) {
            perror("epoll failure");
            return -1;
        }

        //handle epEvents
        for(int i = 0; i < eventNum; ++i) {
            int tmpFd = epEvents[i].data.fd;
            if ((epEvents[i].events & EPOLLERR)
                    || (epEvents[i].events & EPOLLRDHUP)) {
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
                /*new client data from accept thread*/
                if(tmpFd == notifyFd) {
                    char buf[BUFSIZ] = {};
                    socketMsg_t msg = {};
                    msg.size = sizeof(tmpFd);
                    msg.data = buf;
                    //cout <<"recv notify client" <<endl;
                    int ret =  0;
                    while((ret = CommonRecvMsg(tmpFd, &msg, MSG_DONTWAIT)) > 0){
                        if (ret >= sizeof(tmpFd)) {
                            //cout <<"Add: "<< ++cSum <<endl;
                            AddEpollFd(epFd, *(int  *)buf, true);
                        }
                        else {
                            cerr<< "recv notify " << ret <<endl;
                            break;
                        }
                    }
                }
                else {
                    /*client data comming*/
#ifdef THTEAD_READ
                    if (i % 2) {
                        AddHandleList(tmpFd);
                    }
                    else
#endif
                    {
                        int ret = ClientHandle(tmpFd);
                        if(ret < 0) {
                            cerr<< strerror(errno)<<endl;
                        }
                    }
                }
            }
            else {
                cerr<< "!!UNKNOWN events"<<endl;
            }
        }
    }
    return 0;
}

Server::~Server()
{
    if(acceptRun) {
        acceptRun = 0;
    }
    if (epFd > 0) {
        close(epFd);
    }
    epFd = 0;
    if(socketFd > 0) {
        close(socketFd);
    }
    socketFd = 0;
    if (acceptThread) {
        acceptThread->join();
        delete acceptThread;
    }

    if (clientThread) {
        clientThread->join();
        delete clientThread;
    }
    if (notifyClient) {
        notifyClient->Close();
        delete notifyClient;
    }
}
