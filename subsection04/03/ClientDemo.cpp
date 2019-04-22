/*
 * Name: ClientDemo.cpp
 * Description: sock communicate test client
 * Author: liuxueneng@iairfly
 * Date: 20190419
 *
 */
#include "Client.h"
#include <ctime>
#include <thread>

using namespace std;

static int ClientFunc(int i);


#define         MAX_CLIENT_NUM  (1000)

/* get now time us */
long long GetNowMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

int main (int argc, char **argv)
{
    int num = MAX_CLIENT_NUM;
    vector<thread> threads;
    if (argc > 1) {
	    num = atoi(argv[1]);
    }

    for(int i = 0; i < num; ++i) {
	//thread td(bind(ClientFunc,i));
	//td.detach();
       threads.push_back(thread(bind(ClientFunc, i)));
    }

    for (auto t = threads.begin(); t != threads.end(); ++t) {
        t->join();
    }
    cout <<"threads done"<<endl;

    return 0;
}

static int ClientFunc(int i)
{
    Client demoClient(COMMON_SERVER_PORT, COMMON_SERVER_IP);
    char buf[BUFSIZ] =  {};

    demoClient.Init();
    long long startTime = GetNowMs();
    if(demoClient.Connect() < 0 ){
	    cerr<< i << strerror(errno)<<endl;
    }

    //send ping
    socketMsg_t msg = {};
    msg.data = buf;
    sprintf(msg.data, SEND_SERVER_STRING);
    msg.size = strlen(msg.data) + 1;
    demoClient.SendMsg(&msg);

    //receive
    demoClient.RecvMsg(&msg);
    long long endTime = GetNowMs();
    //cout << "totalTime: " << endTime - startTime << "ms" <<endl;
    cout<< msg.data << " use "<<(endTime - startTime) <<"ms"<<endl;

    //wait for close
    sleep(CLIENT_WAIT_SEC);
    //close
    demoClient.Close();

    return 0;
}

