#ifndef SOCK_COMMON_H
#define SOCK_COMMON_H


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

#define         SOCK_PORT       (8000)
#define         MAX_CLIENT      (32)
#define         LOOP_BACK_ADDR  ("127.0.0.1")


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*SOCK_COMMON_H*/
