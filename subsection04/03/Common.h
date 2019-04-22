/*
 * Name: Common.h
 * Description: include  server and client
 * Author: liuxueneng@iairfly
 * Date: 20190419
 *
 */
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <thread>

#define         EPOLL_SIZE              (65535)
#define         COMMON_SERVER_PORT      (8000)
#define         COMMON_BACK_LOG      	(5)
#define         COMMON_SERVER_IP        ("127.0.0.1")

#define         CLIENT_WAIT_SEC         (10)
#define         SEND_SERVER_STRING      ("ping")
#define         SEND_CLIENT_STRING      ("pong")
#define         COMMON_EXIT_STRING      ("ExitRightNow!")

#define         MAX_CLIENT_PER_THREAD    (250)

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>

    typedef struct socketMsg_s {
        unsigned long size;
        char *data;
    } socketMsg_t;


    /*add new fd to the epoll events*/
    static void AddEpollFd(int epFd, int fd, bool enable_et)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
        epoll_ctl(epFd, EPOLL_CTL_ADD, fd, &ev);
        //fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| O_NONBLOCK);
    }

    /*for send msg*/
    static inline int CommonSendMsg(int fd, const socketMsg_t *msg, int mode)
    {
        return send(fd, msg->data,msg->size, mode);
    }

    /*for receive msg*/
    static inline int CommonRecvMsg(int fd, socketMsg_t *msg, int mode)
    {
        int ret = recv(fd, msg->data, msg->size, mode);
        msg->size = ret;
        return ret;
    }

#ifdef __cplusplus
}
#endif

#endif /*COMMON_H*/
