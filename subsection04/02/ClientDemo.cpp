/*
 * Name: ClientDemo.cpp
 * Description: sock communicate test client
 * Author: liuxueneng@iairfly
 * Date: 20190419
 *
 */


#include "Client.h"

using namespace std;

#define         MAX_CLIENT_NUM  (1000)

int main (int argc, char **argv)
{
    Client *demoClient = new Client(COMMON_SERVER_PORT, COMMON_SERVER_IP);

    char buf[BUFSIZ] =  {};


    demoClient->Init();
    demoClient->Connect();


    //send ping
    socketMsg_t msg = {};
    msg.data = buf;
    sprintf(msg.data, SEND_SERVER_STRING);
    msg.size = strlen(msg.data) + 1;
    demoClient->SendMsg(&msg);

    //receive
    demoClient->RecvMsg(&msg);
    cout <<msg.data<<endl;

#if 0
    //for server exit
    sprintf(msg.data, COMMON_EXIT_STRING);
    msg.size = strlen(msg.data) + 1;
    //send Exit
    demoClient->SendMsg(&msg);
#endif
    //close
    demoClient->Close();

    //free
    delete demoClient;

    return 0;
}
