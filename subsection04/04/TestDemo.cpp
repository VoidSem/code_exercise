/*
 * Name:        TestDemo.cpp
 * Description: sock communicate test demo
 * Author:      liuxueneng@iairfly
 * Date:        20190423
 * Modify:      20190425
 */

#include "Server.h"
#include <thread>
#include <map>

#define         COMMON_SERVER_PORT      (8000)
#define         COMMON_SERVER_IP        ("127.0.0.1")
#define         COMMON_BACK_LOG         (1024)

#define         MAX_CLIENT_NUM          (1000)

/*wait seconds close client*/
#define         CLIENT_WAIT_SEC         (10 * 10)

using namespace std;

int ClientInit(int port, const char *ip);

void ClientDestroy(int fd);

int RecvHandle(int epFd);

typedef struct clientInfo_s {
    int fd;
    uint64_t timeOut;
} clientInfo_t;


static map<int, clientInfo_t > client;

int SetReadyClose(int fd)
{
    auto m = client.find(fd);
    if (m != client.end()) {
        m->second.timeOut = 0;
        //cout <<"set ready "<<m->second.fd<<endl;
        return 0;
    }
    return -1;
}

long long GetNowMs()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}


int main()
{


    pid_t pid = fork();

    if (0 == pid) {
        /*this is child */
        sleep(1);
        /* init epoll */
        int epFd = epoll_create(EPOLL_SIZE);
        if(epFd < 0) {
            perror("epFd error");
            exit(EXIT_FAILURE);
        }

        thread *epollThread = new thread(RecvHandle, epFd);

        long long startTime = GetNowMs();

        /* init clients*/
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
        char buf[BUFSIZ] =  {};
        clientInfo_t tmpInfo = {};

        for(int i = 0; i < MAX_CLIENT_NUM; ++i) {
            tmpInfo.fd = ClientInit(COMMON_SERVER_PORT, COMMON_SERVER_IP);
            if(tmpInfo.fd < 0) {
                exit(EXIT_FAILURE);
            }
            //send ping
            size_t len = strlen(SEND_STRING_PING) + 1;
            if(send(tmpInfo.fd, SEND_STRING_PING, len, 0) < 0) {
                perror("send error");
            }

            ev.data.fd = tmpInfo.fd;
            /*add to epoll wait queue for receive*/
            epoll_ctl(epFd, EPOLL_CTL_ADD, tmpInfo.fd, &ev);
            tmpInfo.timeOut = -1;


            /* add to map */
            client[tmpInfo.fd] = tmpInfo;
        }

        epollThread->join();
        cout <<"start exit"<<endl;

        delete epollThread;
        close(epFd);

        long long endTime = GetNowMs();
        cout <<"use "<<endTime - startTime<<"ms"<<endl;
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0) {
        /*this is father*/
        Server *demoServer = new Server(COMMON_SERVER_PORT, COMMON_SERVER_IP, COMMON_BACK_LOG);

        if( 0 == demoServer->Init() ) {
            demoServer->Start();
        }
        else {
            perror("server init failed");
        }

        waitpid(pid, NULL, 0);

        delete demoServer;
    }

    return 0;
}

int RecvHandle(int epFd)
{
    struct epoll_event epEvents[EPOLL_SIZE] = {};
    uint64_t closeNum = 0;
    uint64_t readNum = 0;
    //blocked
    int timeOutMs = 100;
    while (1)
    {
        int eventNum = epoll_wait(epFd, epEvents, EPOLL_SIZE, timeOutMs);
        if(eventNum < 0) {
            perror("epoll failure");
            return -1;
        }
        else if(eventNum == 0) {
            auto r = client.begin();
            while (r != client.end()) {
                if(r->second.timeOut < 0) {
                    ++r;
                    continue;
                }
                ++r->second.timeOut;
                //cout <<r->second.fd <<" timeOut "<< r->second.timeOut<<endl;
                if(r->second.timeOut > CLIENT_WAIT_SEC) {
                    /*delete epoll*/
                    epoll_ctl(epFd, EPOLL_CTL_DEL,r->second.fd, NULL);
                    /*callback client func close*/
                    ClientDestroy(r->second.fd);
                    /*note*/
                    r = client.erase(r);

                    if(++closeNum  >= MAX_CLIENT_NUM) {
                        cout <<"Disconnect all client ok"<<endl;
                        return 0;
                    }
                    //cout <<"Disconnect "<<closeNum<<endl;
                }
                else {
                    ++r;
                }
            }

            continue;
        }
        //handle epEvents
        for(int i = 0; i < eventNum; ++i) {
            int tmpFd = epEvents[i].data.fd;
            if(epEvents[i].events & EPOLLIN) {
                char buf[BUFSIZ] = {};
                if(recv(tmpFd, buf, BUFSIZ, 0) < 0) {
                    cerr<<strerror(errno)<<endl;
                }
                else {
                    cout <<buf<<endl;
                    if(strcmp(buf, SEND_STRING_PONG)) {
                        continue;
                    }
                    /* if get pong start close */
                    SetReadyClose(tmpFd);
                    if(++readNum >= MAX_CLIENT_NUM) {
                        cout <<"read all client "<<readNum <<endl;
                    }
                }
            }
        }
    }
    return 0;
}

/********************************
* Function:     ClientInit
* Description: create socket and connect to server
* Input: port ip
* OutPut: socket fd
* Return: socket d
* Others:
********************************/
int ClientInit(int port, const char *ip)
{
    struct sockaddr_in svrAddr = {};
    svrAddr.sin_family = PF_INET;
    svrAddr.sin_port = htons(port);
    svrAddr.sin_addr.s_addr = inet_addr(ip);

/*create socket and connet to server*/
    int sockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockFd < 0) {
        perror("sock error");
        return -1;
    }

    int addrLen = sizeof(svrAddr);
    if(connect(sockFd, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) {
        close(sockFd);
        perror("connect error");
        return -2;
    }
    return sockFd;
}

/********************************
* Function:
* Description:
* Input:
* OutPut:
* Return:
* Others:
********************************/
void ClientDestroy(int fd)
{
    struct stat _stat = {};

    /*check the fd*/
    if (!fstat(fd, &_stat)) {
        /*fd hard link nums*/
        if (_stat.st_nlink >= 1) {
            close(fd);
        }
    }
}
