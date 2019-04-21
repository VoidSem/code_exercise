/*
 * Name: ServerDemo.cpp
 * Description: sock communicate test server
 * Author:liuxueneng@iairfly
 * Date:20190419
 *
 */


#include "Server.h"

using namespace std;

int main()
{
    Server *demoServer = new Server(COMMON_SERVER_PORT, COMMON_SERVER_IP, COMMON_BACK_LOG);

    demoServer->Init();
    demoServer->Start();
    demoServer->Close();

    delete demoServer;

    return 0;
}
