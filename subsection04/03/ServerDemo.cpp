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

    if(demoServer->Init() < 0) {
        cerr<<"server init failed"<<endl;
        delete demoServer;
        return -1;
    }

    if(demoServer->Start() < 0) {
        cerr<<"ERROR"<<endl;
    }

    demoServer->Close();

    delete demoServer;

    return 0;
}
