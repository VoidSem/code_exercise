/*
 * Name:        TestDemo.cpp
 * Description: sock communicate test demo
 * Author:      liuxueneng@iairfly
 * Date:        20190423
 * Modify:      20190424
 */

#include "Server.h"
#include "Client.h"
#include <thread>
#include <sys/timerfd.h>

#define         COMMON_SERVER_PORT      (8000)
#define         COMMON_SERVER_IP        ("127.0.0.1")
#define         COMMON_BACK_LOG         (1024)

#define         MAX_CLIENT_NUM          (1000)

/*wait seconds close client*/
#define         CLIENT_WAIT_SEC         (10)

using namespace std;

int RecvHandle(int epFd, int timerFd);
int TimerFdInit(int epFd);


typedef struct timerCnt_s {
    int fd;
    unsigned char timeCount;
} timerCnt_t;


static vector <Client *> demoClient;
//static Client *demoClient[MAX_CLIENT_NUM] = {};

static void ClientDisConnect(int fd)
{
    for (int i = 0; i < MAX_CLIENT_NUM; ++i) {
        /*match client*/
        if (demoClient[i] && fd == demoClient[i]->GetClient()) {
            /*close sock*/
            demoClient[i]->DisConnect();
        }
    }
}

long long GetNowMs()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}


int main()
{
    Server *demoServer = new Server(COMMON_SERVER_PORT, COMMON_SERVER_IP, COMMON_BACK_LOG);

    if( demoServer->Init() < 0) {
        delete demoServer;
        return -1;
    }


    pid_t pid = fork();

    if (0 == pid) {
        /*this is child */
        delete demoServer;

        /* init epoll */
        int epFd = epoll_create(EPOLL_SIZE);
        if(epFd < 0) {
            perror("epFd error");
            exit(EXIT_FAILURE);
        }

        /* init timer fd */
        int timerFd = TimerFdInit(epFd);
        if(timerFd < 0) {
            exit(EXIT_FAILURE);
        }

        thread *epollThread = new thread(RecvHandle, epFd, timerFd);

        long long startTime = GetNowMs();

        /* init clients*/
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
        int tmpFd = -1;
        char buf[BUFSIZ] =  {};

        for(int i = 0; i < MAX_CLIENT_NUM; ++i) {
            Client *tmpClient = new Client(COMMON_SERVER_PORT, COMMON_SERVER_IP);

            if(tmpClient->Init() < 0) {
                continue;
            }
            //send ping
            size_t len = strlen(SEND_STRING_PING) + 1;
            tmpClient->SendMsg(SEND_STRING_PING, len, 0);

            /*add to epoll wait queue for receive*/
            tmpFd = tmpClient->GetClient();
            ev.data.fd = tmpFd;
            epoll_ctl(epFd, EPOLL_CTL_ADD, tmpFd, &ev);

            /* add to vector */
            demoClient.push_back(tmpClient);
        }

        epollThread->join();
        cout <<"start exit"<<endl;

        delete epollThread;
        close(timerFd);
        close(epFd);

        /*free clients*/
        for(auto c = demoClient.begin(); c != demoClient.end(); ++c) {
            delete *c;
        }
        demoClient.clear();

        long long endTime = GetNowMs();
        cout <<"use "<<endTime - startTime<<"ms"<<endl;
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0) {
        /*this is father*/
        demoServer->Start();

        waitpid(pid, NULL, 0);
        delete demoServer;
    }

    return 0;
}

int RecvHandle(int epFd, int timerFd)
{
    struct epoll_event epEvents[EPOLL_SIZE] = {};
    vector <timerCnt_t> readClient;
    uint64_t disconnectNum = 0;
    uint64_t readNum = 0;
    int timeOut = -1;
    uint64_t totalExp = 0;
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
            if(epEvents[i].events & EPOLLIN) {
                if (timerFd == tmpFd) {
                    uint64_t tmpExp = 0;
                    read(timerFd, &tmpExp, sizeof(uint64_t));
                    auto r = readClient.begin();
                    while (r != readClient.end()) {
                        ++r->timeCount;
                        if(r->timeCount > CLIENT_WAIT_SEC) {
                            /*delete epoll*/
                            epoll_ctl(epFd, EPOLL_CTL_DEL,r->fd, NULL);
                            /*callback client func close*/
                            ClientDisConnect(r->fd);

                            /*note*/
                            r = readClient.erase(r);
                            ++disconnectNum;
                            //cout <<"Disconnect "<<disconnectNum<<endl;
                        }
                        else {
                            ++r;
                        }
                    }

                    if(disconnectNum  >= MAX_CLIENT_NUM) {
                        cout <<"Disconnect all client ok"<<endl;
                        readClient.clear();
                        return 0;
                    }
                    totalExp += tmpExp;
                    cout<<"timer count "<<totalExp<<endl;
                }
                else {
                    char buf[BUFSIZ] = {};
                    if(recv(tmpFd, buf, BUFSIZ, 0) < 0) {
                        cerr<<strerror(errno)<<endl;
                    }
                    else {
                        cout <<buf<<endl;
                        if(strcmp(buf, SEND_STRING_PONG)) {
                            continue;
                        }
                        /* if get pong start disconnect */
                        timerCnt_t tmpCnt = {};
                        tmpCnt.fd = tmpFd;
                        tmpCnt.timeCount = 0;
                        /* add to vector*/
                        readClient.push_back(tmpCnt);
                        if(++readNum >= MAX_CLIENT_NUM) {
                            cout <<"read all client "<<readNum <<endl;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


int TimerFdInit( int epFd)
{
    struct itimerspec new_value;
    /*init time*/
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_nsec = 0;
    /*time interval*/
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;

    int timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerFd < 0) {
        cerr<<strerror(errno)<<endl;
        return -1;
    }

    int ret = timerfd_settime(timerFd, 0, &new_value, NULL);
    if (ret < 0) {
        cerr<<strerror(errno)<<endl;
        close(timerFd);
        return -1;
    }
    /* add to epoll */
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
    ev.data.fd = timerFd;
    epoll_ctl(epFd, EPOLL_CTL_ADD, timerFd, &ev);
    return timerFd;
}
