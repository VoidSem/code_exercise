/*
 * Name:        TestDemo.cpp
 * Description: sock communicate test demo
 * Author:      liuxueneng@iairfly
 * Date:        20190423
 *
 */


#include "Server.h"
#include "Client.h"

#define         COMMON_SERVER_PORT      (8000)
#define         COMMON_SERVER_IP        ("127.0.0.1")
#define         COMMON_BACK_LOG         (1024)

#define         MAX_CLIENT_NUM          (1000)

/*wait seconds close client*/
#define         CLIENT_WAIT_SEC         (10)

using namespace std;


void SignalHandler(int signo)
{
    if(SIGALRM == signo)
        cout<<"wake up"<<endl;
}

long long GetNowMs()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void InitTimer()
{
    signal(SIGALRM, SignalHandler);

    struct itimerval itv;
    struct itimerval oldTv;
    itv.it_interval.tv_sec = CLIENT_WAIT_SEC;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = CLIENT_WAIT_SEC;
    itv.it_value.tv_usec = 0;

    /*init timer*/
    setitimer(ITIMER_REAL, &itv, &oldTv);
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
        /******************************/
        Client *demoClient[MAX_CLIENT_NUM] = {};
        long long startTime = GetNowMs();
        for(int i = 0; i < MAX_CLIENT_NUM; ++i) {
            demoClient[i] = new Client(COMMON_SERVER_PORT, COMMON_SERVER_IP);
            char buf[BUFSIZ] =  {};

            if(demoClient[i]->Init() < 0) {
                continue;
            }
            //send ping
            size_t len = strlen(SEND_STRING_PING) + 1;
            demoClient[i]->SendMsg(SEND_STRING_PING, len, 0);

            //receive
            demoClient[i]->RecvMsg(buf, BUFSIZ, 0);
            cout <<buf<<endl;
        }

        /*set timer*/
        InitTimer();

        /*wait for signal alarm */
        pause();
        cout <<"start exit"<<endl;
        for(int i = 0; i < MAX_CLIENT_NUM; ++i) {
                delete demoClient[i];
        }
        long long endTime = GetNowMs();
        cout <<"use "<<endTime - startTime<<"ms"<<endl;
        /******************************/
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
