#ifndef BLCCORE_NET
#define BLCCORE_NET

#include "../blctype.h"

#define BLCNET_SOCK_STREAM 0x1 /*流套接字类型为SOCK_STREAM*/
#define BLCNET_SOCK_DGRAM 0x2  /* 数据报套接字类型为SOCK_DGRAM */
#define BLCNET_SOCK_RAW 0x3    /* 原始套接字SOCK_RAW */

#define BLCNET_SOCK_BLOCK 0x1   /*套接字类型为阻塞*/
#define BLCNET_SOCK_NOBLOCK 0x2 /* 套接字类型为非阻塞 */

#define BLCNET_SOCKCHECK_READ 0x1   /*检测套接字是否可读*/
#define BLCNET_SOCKCHECK_WRITE 0x2  /*检测套接字是否可写*/
#define BLCNET_SOCKCHECK_EXCEPT 0x3 /*检测套接字是否异常*/

typedef struct {
    char ip[32];
    int port;
} BlcIPAddr;

int BlcCore_SocketCreate(int sock_type);
int BlcCore_SocketClose(int sock);
int BlcCore_SendTo(int sock, void *msg, int len, BlcIPAddr *to_ipaddr);
int BlcCore_RecvFrom(int sock, void *buf, int len, BlcIPAddr *from_ipaddr);
int BlcCore_SetBlockType(int sock, int block_type);
int BlcCore_SocketSelect(int sock, int checkstatus, BlcTime *time);

int BlcCore_TCPConnect(int sock, BlcIPAddr *to_ipaddr);
int BlcCore_TCPSend(int sock, void *msg, int len);
int BlcCore_TCPRecv(int sock, void *buf, int len);

#endif
