/*
 * name: GetNetworkInfo.c
 * Description:
 * author: liuxueneng@airfly
 * date: 20190419
 *
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
#include <string.h>


#define		RUNNING_STR		("UP BROADCAST RUNNING")
#define		HW_ADDR			("HWaddr ")
#define		INET_ADDR		("inet addr:")
#define         BCAST_ADDR              ("Bcast:")
#define         ADDR_MASK               ("Mask:")
#define		ADDR_LEN		(32)
#define         MAX_CARD_NUM            (8)

typedef	struct netCard_s {
	char macAddr[ADDR_LEN];
	char inetAddr[ADDR_LEN];
	char bcastAddr[ADDR_LEN];
	char addrMask[ADDR_LEN];
}netCard_t;

int GetNetInfo(char *buffer, int bufSize);
static int GetItem(const char *buf, const char *key,
        int len, char c, char *str);
int GetCardInfo(const char *buffer, netCard_t *netCard);

int GetNetInfo(char *buffer, int bufSize)
{
	char  cmd[128];
	FILE  *fp;
	int    readByte;
	int    ret =0;

	memset( buffer, 0, BUFSIZ );
	memset( cmd, 0, 100 );
	sprintf(cmd, "ifconfig | grep -v -i \"LoopBack\""
                "| grep \"HWaddr\" | awk '{print $1}'");

	fp = popen(cmd, "r");
	if (NULL == fp){
		printf("popen:%s failed\n", cmd);
		return -1;
	}
	readByte = fread(buffer, 1, bufSize - 1, fp);
	pclose(fp);
	if (readByte <= 0) {
		fprintf(stderr, "%s: NO FOUND\r\n",cmd);
		return -2;
	}
	return 0;
}

static int GetRunningCards(const char *cardName, char *buffer)
{
	//char  buffer[BUFSIZ];
	char  cmd[100];
	FILE  *fp;
	int    readByte;

	sprintf(cmd, "ifconfig %s", cardName);
	fp = popen(cmd, "r");
	if (NULL == fp){
		printf("popen:%s failed\n", cmd);
		return -1;
	}
	readByte = fread(buffer, 1, BUFSIZ-1, fp);
	pclose(fp);
	if (readByte > 0 && strstr(buffer, RUNNING_STR)) {
		//printf("%s:Link\n", cardName);
		return 0;
	}
	return -1;
}


static int GetItem(const char *buf, const char *key, int len, char c, char *str)
{
    char *p = strstr(buf, key);
    int keyLen = strlen(key);

    if(NULL == p) {
        printf("%s get %s from %s failed\n",__func__, key, buf);
        return -1;
    }

    memcpy(str, p+keyLen, len);
    p = strchr(str+keyLen, c);
    if (NULL == p) {
        printf("%s get %s from %s find no:[%c]\n",__func__, key, buf, c);
    }
    else {
        *p = '\0';
    }

    return 0;
}

int GetCardInfo(const char *buffer, netCard_t *netCard)
{
    if (NULL == buffer || NULL == netCard) {
        return -1;
    }

    //get hw addr
    const char *key = HW_ADDR;
    int len = ADDR_LEN;
    char endChar = '\n';
    char *str = netCard->macAddr;
    int ret = 0;
    int count = 0;
    ret = GetItem(buffer, key, len, endChar, str);
    if ( ret < 0) {
        printf("%s get %s failed\n",__func__, key);
    }
    ++count;

    //get inet addr
    key = INET_ADDR;
    endChar = ' ';
    str = netCard->inetAddr;
    ret = GetItem(buffer, key, len, endChar, str);
    if ( ret < 0) {
        printf("%s get %s failed\n",__func__, key);
    }
    ++count;

    //get bcast addr
    key = BCAST_ADDR;
    str = netCard->bcastAddr;
    ret = GetItem(buffer, key, len, endChar, str);
    if ( ret < 0) {
        printf("%s get %s failed\n",__func__, key);
    }
    ++count;

    //get addr mask
    key = ADDR_MASK;
    str = netCard->addrMask;
    ret = GetItem(buffer, key, len, endChar, str);
    if ( ret < 0) {
        printf("%s get %s failed\n",__func__, key);
    }
    ++count;

    return count;
}

int main(int argc, char **argv)
{
	char card[MAX_CARD_NUM][ADDR_LEN] = {};
        char netInfo[BUFSIZ] = {};
	netCard_t netCard = {};
        int ret = -1;

	if (GetNetInfo(netInfo, BUFSIZ) < 0) {
            printf("get netinfo failed\n");
            return -1;
        }

        char *p = strchr(netInfo, '\n');
        char *pStart = netInfo;
        int cardNum = 0;
        while (p = strchr(pStart, '\n')) {
            memcpy(card[cardNum], pStart, (p - pStart));
            ++cardNum;
            pStart = ++p;
        }

        int i = 0;
	for (i = 0; i < cardNum; ++i) {
		if(GetRunningCards(card[i], netInfo) == 0 ) {
                    break;
		}
                else
                {
                    ret = -2;
                    goto ERROR;
                }
	}

        if (GetCardInfo(netInfo, &netCard) > 0) {
            printf("card: %s\nmacAddr: %s\n", card[i], netCard.macAddr);
                printf("inet_addr: %s\nbcastAddr: %s\naddrMask: %s\n",
                    netCard.inetAddr, netCard.bcastAddr, netCard.addrMask);
        }
        else
        {
            ret = -3;
            goto ERROR;
        }

        return 0;
ERROR:
        printf("get network infomation failed:%d\n",ret);
        return ret;
}
