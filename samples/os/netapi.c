/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "../../include/blccore.h"

#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#ifdef WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
//    #include <psdk_inc/_ip_types.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int BlcCore_SocketCreate(int sock_type) {
    if (sock_type == BLCNET_SOCK_STREAM) {
        return socket(AF_INET, SOCK_STREAM, 0);
    } else if (sock_type == BLCNET_SOCK_DGRAM) {
        return socket(AF_INET, SOCK_DGRAM, 0);
    } else if (sock_type == BLCNET_SOCK_RAW) {
        return socket(AF_INET, SOCK_RAW, 0);
    } else {
        return -1;
    }
}

int BlcCore_SocketClose(int sock) { return close(sock); }

int BlcCore_SendTo(int sock, void *msg, int len, BlcIPAddr *to_ipaddr) {
    if (msg == NULL || to_ipaddr == NULL)
        return -1;

    struct sockaddr_in servaddr;

    int addr_len;
    int ret = -1;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(to_ipaddr->port);
    servaddr.sin_addr.s_addr = inet_addr(to_ipaddr->ip);
    addr_len = sizeof(servaddr);

    ret = sendto(sock, msg, len, 0, (struct sockaddr *)&servaddr, addr_len);

    return ret;
}

int BlcCore_RecvFrom(int sock, void *buf, int len, BlcIPAddr *from_ipaddr) {
    errno = 0;
    if (buf == NULL)
        return -1;

    struct sockaddr_in c_addr;
    int addr_len = sizeof(c_addr);
    int ret = -1;

    ret = recvfrom(sock, buf, len, 0, (struct sockaddr *)&c_addr,
                   (socklen_t *)&addr_len);
    //    ret = recvfrom(sock, buf, len, 0, ((SOCKADDR *))&c_addr,
    //    (socklen_t*)&addr_len);

    if (ret == SOCKET_ERROR) {
        printf(L"recvfrom failed with error %d\n", WSAGetLastError());
    }

    if (from_ipaddr != NULL) {
        from_ipaddr->port = ntohs(c_addr.sin_port);
        strncpy(from_ipaddr->ip, inet_ntoa(c_addr.sin_addr),
                sizeof(from_ipaddr->ip));
    }

    return ret;
}

int32_t BlcCore_SetBlockType(int sock, int block_type) {
    if (block_type == BLCNET_SOCK_NOBLOCK) {

        int iMode = 0;
#ifdef _WIN32
        ioctlsocket(sock, FIONBIO, (unsigned long *)&iMode);
#else
        iMode = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, iMode | O_NONBLOCK); //设置为非阻塞模式
#endif

        return 0;
    } else if (block_type == BLCNET_SOCK_BLOCK) {
        return -1;
    } else {
        return -1;
    }
}

int32_t BlcCore_SocketSelect(int sock, int checkstatus, BlcTime *time) {
    fd_set fdset;
    struct timeval tv;
    struct timeval *p_tv = NULL;

    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);

    if (time != NULL) {
        tv.tv_sec = time->seconds;
        tv.tv_usec = time->microsecs;
        p_tv = &tv;
    } else {
        p_tv = NULL;
    }

    int ret = -1;
    if (checkstatus == BLCNET_SOCKCHECK_READ)
        ret = select(sock + 1, &fdset, NULL, NULL, p_tv);
    else if (checkstatus == BLCNET_SOCKCHECK_WRITE)
        ret = select(sock + 1, NULL, &fdset, NULL, p_tv);
    else if (checkstatus == BLCNET_SOCKCHECK_EXCEPT)
        ret = select(sock + 1, NULL, NULL, &fdset, p_tv);
    else
        return -3;

    if (ret <= 0) //出错超时
    {
        return -1;
    }

    if (!FD_ISSET(sock, &fdset)) {
        return -2;
    }

    return 0;
}

int BlcCore_TCPConnect(int sock, BlcIPAddr *to_ipaddr) {
    if (to_ipaddr == NULL)
        return -1;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(to_ipaddr->ip);
    ;
    servaddr.sin_port = htons(to_ipaddr->port);

    return connect(sock, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
}

int BlcCore_TCPSend(int sock, void *msg, int len) {
    if (msg == NULL)
        return -1;

    return send(sock, msg, len, 0);
}

int BlcCore_TCPRecv(int sock, void *buf, int len) {
    if (buf == NULL)
        return -1;

    return recv(sock, buf, len, 0);
}
