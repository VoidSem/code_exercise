/*
 * Name:        Common.h
 * Description: include  server and client
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      20190423
 */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <list>

#define         COMMON_SERVER_PORT      (8000)
#define         COMMON_BACK_LOG      	(1024)
#define         COMMON_SERVER_IP        ("127.0.0.1")
#define         EPOLL_SIZE              (65535)

#define         SEND_STRING_PING        ("ping")
#define         SEND_STRING_PONG        ("pong")


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
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#ifdef __cplusplus
}
#endif

#endif /*COMMON_H*/
