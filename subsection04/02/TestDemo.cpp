/*
 * Name:        TestDemo.cpp
 * Description: sock communicate test demo
 * Author:      liuxueneng@iairfly
 * Date:        20190423
 */

#include "Server.h"
#include "Client.h"

using namespace std;

int ClientDemo();

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
        ClientDemo();
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

int ClientDemo()
{
    Client *demoClient = new Client(COMMON_SERVER_PORT, COMMON_SERVER_IP);

    char buf[BUFSIZ] =  {};

    demoClient->Init();

    //send ping
    size_t len = strlen(SEND_STRING_PING) + 1;
    demoClient->SendMsg(SEND_STRING_PING, len, 0);

    //receive
    demoClient->RecvMsg(buf, BUFSIZ, 0);
    cout <<buf<<endl;

    //free
    delete demoClient;

    return 0;
}
