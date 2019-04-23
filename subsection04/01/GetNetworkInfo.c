/*
 * Name:        GetNetworkInfo.c
 * Description: get system ip hwAddr
 * Author:      liuxueneng@iairfly
 * Date:        20190419
 * Modify:      20190423
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <ifaddrs.h>
#include <string.h>
#include <netdb.h>
#include <sys/param.h>
#include <net/if.h>
#include <netinet/in.h>

#define MAX_INTERFACE 8

int  main(int argc, char **argv)
{
    int fd = 0;
    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket error");
        return -1;
    }

    struct ifreq buf[MAX_INTERFACE];
    struct ifconf ifc;
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    if (ioctl(fd, SIOCGIFCONF, (char *)&ifc) < 0) {
        perror("ioctl error");
        close(fd);
        return -2;
    }
    //printf("interface num %d\n",nets);
    int nets = ifc.ifc_len / sizeof (struct ifreq);
    while ((nets--) > 0) {
        //loop back ignore
        if(0 == strcmp(buf[nets].ifr_name, "lo")) {
            continue;
        }

        if ((ioctl(fd, SIOCGIFFLAGS, (char *) &buf[nets]))){
            continue;
        }

        if (!(buf[nets].ifr_flags & IFF_UP)) {
            //if down  ignore
            printf("the interface status is DOWN\n");
            continue;
        }

        printf ("\nName:\n[%s]\n", buf[nets].ifr_name);
        if ( (ioctl(fd, SIOCGIFADDR, (char *) &buf[nets])))
        {
            continue;
        }
        printf("\nIp:\n[%s]\n",
                (char *)inet_ntoa(((struct sockaddr_in*)(&buf[nets].ifr_addr))->sin_addr));

        if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[nets]))) {
            printf("\nMacAddr:\n[%02x:%02x:%02x:%02x:%02x:%02x]\n",
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[0],
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[1],
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[2],
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[3],
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[4],
                    (unsigned char)buf[nets].ifr_hwaddr.sa_data[5]);
        }

        if ( (ioctl(fd, SIOCGIFNETMASK, (char *) &buf[nets]))) {
            continue;
        }
        printf("\nAddrMask:\n[%s]\n",
                (char*)inet_ntoa(((struct sockaddr_in*) (&buf[nets].ifr_addr))->sin_addr));

        if ( (ioctl(fd, SIOCGIFBRDADDR, (char *) &buf[nets]))) {
            continue;
        }
        printf("\nBroadcastAddr:\n[%s]\n",
                (char*)inet_ntoa(((struct sockaddr_in*) (&buf[nets].ifr_addr))->sin_addr));
    }

    close(fd);

    return 0;
}
