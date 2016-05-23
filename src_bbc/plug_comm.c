#include "../include/blccore.h"
#include "plug_comm.h"

#include <stdarg.h>

extern int connection_status;
extern int demand_status;
extern char stbid[32];
extern int region_id;
extern int access_sock;
extern int keygw_sock;
extern int net_type;
extern BlcIPAddr accessgw_addr;
extern BlcIPAddr keygw_addr;

int packAccessReqMsg(NblMsg *msg) {
    if (msg == NULL) {
        BlcCore_printf("%s %d error paramter !\n", __FUNCTION__, __LINE__);
        return -1;
    }

    NblTermMsgAccess accessMsg;
    BlcCore_memset(&accessMsg, 0, sizeof(accessMsg));

    BlcCore_memcpy(accessMsg.head.term_id, stbid, sizeof(accessMsg.head.term_id));
    BlcCore_sprintf(accessMsg.head.term_net_type, "%d", net_type);
    BlcCore_sprintf(accessMsg.head.region_id, "%d", region_id);
    BlcCore_sprintf(accessMsg.head.plug_version, "%s", BLCPLUG_VERSION);

    BlcCore_sprintf(msg->type, "%s", "1000");
    BlcCore_sprintf(msg->len, "%d", sizeof(accessMsg));
    BlcCore_memcpy(msg->value, &accessMsg, sizeof(accessMsg));

    return BlcCore_atoi(msg->len) + MSG_HEAD_LEN;
}

int unPackAccessRespMsg(const char *respMsg, BlcIPAddr *addr) {
    if (respMsg == NULL || addr == NULL) {
        BlcCore_printf("%s %d error paramter !\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (BlcCore_strncmp((char *)stbid, respMsg, 32) != 0) {
        BlcCore_printf("%s %d stb id error, receive respMsg = %s \n", __FUNCTION__,
                       __LINE__, respMsg);
        return -2;
    }

    char keygw_url[32];
    BlcCore_memset(keygw_url, 0, sizeof(keygw_url));
    BlcCore_memcpy(keygw_url, respMsg + 32, 32);
    BlcCore_printf("%s %d keygw_url = %s \n", __FUNCTION__, __LINE__, keygw_url);

    int ret = BlcCore_sscanf(keygw_url, "%[^:]:%d", addr->ip, &addr->port);
    if (ret != 2) {
        BlcCore_printf("%s %d get addr_info error url =  %s \n", __FUNCTION__,
                       __LINE__, keygw_url);
        return -2;
    }

    BlcCore_printf("%s %d get keygw_addr =  %s:%d \n", __FUNCTION__, __LINE__,
                   addr->ip, addr->port);

    return 0;
}

int sendAccessReq2Cloud(NblMsg *msg, int msgLen) {
    if (msg == NULL || msgLen <= 0) {
        BlcCore_printf("%s %d error paramter !\n", __FUNCTION__, __LINE__);
        return -1;
    }

    BlcIPAddr ipaddr;
    BlcCore_memset(&ipaddr, 0, sizeof(ipaddr));
    BlcCore_strncpy(ipaddr.ip, accessgw_addr.ip, sizeof(ipaddr.ip));
    ipaddr.port = accessgw_addr.port;

    /* 发送UDP消息 */
    BlcCore_printf("%s %d start sendto CAG ..\n", __FUNCTION__, __LINE__);
    int len = BlcCore_SendTo(access_sock, (char *)msg, msgLen, &ipaddr);
    if (len != msgLen) {
        BlcCore_printf(
                    "%s %d sendto CAG error, msg len = %d, real send len = %d \n",
                    __FUNCTION__, __LINE__, msgLen, len);
        return -1;
    }

    return 0;
}

int recvAccessRespFromCloud(char *buf, int bufSize, int timeOut) {
    if (buf == NULL || bufSize <= 0 || timeOut < 0) {
        BlcCore_printf("%s %d error paramter !\n", __FUNCTION__, __LINE__);
        return -1;
    }
    BlcCore_printf("%s %d start recvfrom CAG\n", __FUNCTION__, __LINE__);

    BlcTime time;

    time.seconds = timeOut;
    time.microsecs = 0;

    int ret = BlcCore_SocketSelect(access_sock, BLCNET_SOCKCHECK_READ, &time);
    if (ret < 0) {
        BlcCore_printf("%s %d BlcCore_SocketSelect is error or timeout\n",
                       __FUNCTION__, __LINE__);
        return -2;
    }

    BlcIPAddr ipaddr;
    int len = BlcCore_RecvFrom(access_sock, buf, bufSize, &ipaddr);
    if (len <= 0) {
        BlcCore_printf("%s %d BlcCore_RecvFrom error, len = %d \n", __FUNCTION__,
                       __LINE__, len);
        return -3;
    }

    return 0;
}

//功能:获取交互网关地址
int GetCloudAddr(BlcIPAddr *addr, int time_out) //参数单位秒
{
    int msgLen;
    NblMsg msg;
    BlcCore_memset(&msg, 0, sizeof(msg));
    if ((msgLen = packAccessReqMsg(&msg)) <= 0)
        return -1;

    if (sendAccessReq2Cloud(&msg, msgLen) != 0)
        return -2;

    char respMsg[128];
    BlcCore_memset(respMsg, 0, sizeof(respMsg));
    if (recvAccessRespFromCloud(respMsg, sizeof(respMsg), time_out) != 0)
        return -3;

    if (unPackAccessRespMsg(respMsg, addr) != 0)
        return -4;

    return 0;
}

int packKeySendReqMsg(NblMsg *sendMsg, const char *msg, int msgLen)
{
    BlcCore_sprintf(sendMsg->type, "1003");
    BlcCore_sprintf(sendMsg->len, "%d", msgLen);
    BlcCore_memcpy(sendMsg->value, msg, msgLen);

    return msgLen;
}

int packRegisterReqMsg(NblMsg *sendMsg, int msgLen)
{
    int sendLen;

    NblTermMsgRegister keepalive;
    BlcCore_sprintf(keepalive.head.plug_version, "%s", BLCPLUG_VERSION);
    BlcCore_sprintf(keepalive.head.region_id, "%d", region_id);
    BlcCore_sprintf(keepalive.head.term_id, "%s", stbid);
    BlcCore_sprintf(keepalive.head.term_net_type, "%d", net_type);
    BlcCore_sprintf(sendMsg->type, "1001");
    BlcCore_sprintf(sendMsg->len, "%d", sizeof(NblTermMsgRegister));
    sendLen = BlcCore_atoi(sendMsg->len);
    BlcCore_memcpy(sendMsg->value, &keepalive, sendLen);

    return sendLen;
}

int packGenericReqMsg(NblMsg *sendMsg, const char *msg, int msgLen ,int msgType)
{
    int sendLen;

    NblTermMsgComm cmd;
    BlcCore_memset(&cmd, 0, sizeof(cmd));

    BlcCore_sprintf(cmd.head.plug_version, "%s", BLCPLUG_VERSION);
    BlcCore_sprintf(cmd.head.region_id, "%d", region_id);
    BlcCore_sprintf(cmd.head.term_id, "%s", stbid);
    BlcCore_sprintf(cmd.head.term_net_type, "%d", net_type);

    BlcCore_sprintf(cmd.body.len, "%d", msgLen);
    BlcCore_sprintf(cmd.body.type, "%d", msgType);
    BlcCore_memcpy(cmd.body.value, msg, msgLen);

    BlcCore_sprintf(sendMsg->type, "1002");
    BlcCore_sprintf(sendMsg->len, "%d", sizeof(cmd.head) + msgLen + 16);
    sendLen = BlcCore_atoi(sendMsg->len);
    BlcCore_memcpy(sendMsg->value, &cmd, sendLen);

    return sendLen;
}

int StbMsgPack(NblMsg *sendMsg, int msgType, const char *msg, int msgLen) {
    int sendLen;

    if (msgType == 3003) {
        sendLen = packKeySendReqMsg(sendMsg, msg, msgLen);
    } else if (msgType == 3004) {
        sendLen = packRegisterReqMsg(sendMsg, msgLen);
    } else {
        sendLen = packGenericReqMsg(sendMsg, msg, msgLen, msgType);
    }

    return sendLen + MSG_HEAD_LEN;
}

//功能:发送消息给平台
int SendMsgToCloud(int sockid, int msgType, const char *msg, int msgLen) {
    NblMsg sendMsg;
    BlcCore_memset(&sendMsg, 0, sizeof(sendMsg));
    int dataLen = StbMsgPack(&sendMsg, msgType, msg, msgLen);

    int ret = BlcCore_TCPSend(sockid, &sendMsg, dataLen);
    if (dataLen != ret) {
        BlcCore_printf("%s %d TCP send to Cloud error, real send len = %d \n",
                       __FUNCTION__, __LINE__, ret);
        if (ret <= 0) {
            // 断线重连
            BlcCore_SocketClose(keygw_sock);
            connection_status = 0;
        }
        return -1;
    }

    BlcCore_printf("%s %d  TCP send to [%s:%d] , msg_type = %d data_len = %d "
                   "send_len = %d\n",
                   __FUNCTION__, __LINE__, keygw_addr.ip, keygw_addr.port,
                   msgType, ret, dataLen);
    return 0;
}

int StbMsgParse(NblMsg *recvMsg) {
    if (BlcCore_strncmp((char *)recvMsg->type, "2000", 4) == 0) {
        NblDownCmd *msg = (NblDownCmd *)recvMsg->value;
        StbOrderParse(msg->body.value, 1);

        if (BlcDemandStatus_Signing == demand_status) {
            BlcCore_printf("%s %d login fail and quit!\n", __FUNCTION__, __LINE__);
            StbLogoutCloud(1);
        }
    } else
        return -1;

    return 0;
}

//功能:机顶盒接收tcp指令线程
void BlcStb_RecvIPOrderProcess() {
    BlcCore_printf("%s %d Starting BlcStb_RecvIPOrderProcess ...", __FUNCTION__,
                   __LINE__);

    NblMsg recvMsg;
    BlcTime time;

    int sum = 0;
    int recvLen = -1;
    int msgLen = 0;
    int recvMsgFlag = 1;

    BlcCore_memset(&recvMsg, 0, sizeof(recvMsg));

    while (1) {
        if (connection_status == 0) {
            BlcCore_ThreadDelay(100);
            continue;
        }

        time.seconds = 0;
        time.microsecs = 100 * 1000;

        if (BlcDemandStatus_Signing == demand_status) {
            time.seconds = BLCPLUG_RECV_TIMEOUT;
            time.microsecs = 0;
        }

        int ret = BlcCore_SocketSelect(keygw_sock, BLCNET_SOCKCHECK_READ, &time);
        if (ret < 0) {
            BlcCore_printf("%s %d BlcCore_SocketSelect is error or timeout\n",
                           __FUNCTION__, __LINE__);

            if (BlcDemandStatus_Signing == demand_status &&
                    time.microsecs == 100 * 1000)
                continue;

            if (BlcDemandStatus_Signing == demand_status &&
                    time.seconds == BLCPLUG_RECV_TIMEOUT) {
                BlcCore_printf("%s %d TCP recv msg timeout %d s...\n", __FUNCTION__,
                               __LINE__, BLCPLUG_RECV_TIMEOUT);
                NoticeFail("本次请求失败!服务器或网络忙，请重新连接。");
                StbLogoutCloud(1);
            }

            continue;
        }

        while (1) {
            if (recvMsgFlag == 1) {
                recvLen = BlcCore_TCPRecv(keygw_sock, &recvMsg + sum, 16 - sum);
                if (recvLen == 0) {
                    // 断线重连
                    BlcCore_printf("%s %d Connection is broken and reconnect...!\n",
                                   __FUNCTION__, __LINE__);
                    BlcCore_SocketClose(keygw_sock);
                    connection_status = 0;
                    sum = 0;
                    BlcCore_memset(&recvMsg, 0, sizeof(recvMsg));
                    break;
                } else if (recvLen < 0) {
                    BlcCore_printf("%s %d recv error!\n", __FUNCTION__, __LINE__);
                    break;
                }

                sum += recvLen;
                if (sum == 16) {
                    msgLen = BlcCore_atoi(recvMsg.len);
                    if (msgLen == MAX_MSG_LEN) {
                        BlcCore_SocketClose(keygw_sock);
                        connection_status = 0;
                        sum = 0;
                        BlcCore_memset(&recvMsg, 0, sizeof(recvMsg));
                        break;
                    } else {
                        sum = 0;
                        recvMsgFlag = 2;
                    }
                }
            } else if (recvMsgFlag == 2) {
                recvLen =
                        BlcCore_TCPRecv(keygw_sock, &recvMsg.value + sum, msgLen - sum);
                if (recvLen == 0) {
                    // 断线重连
                    BlcCore_printf("%s %d Connection is broken and reconnect...!\n",
                                   __FUNCTION__, __LINE__);
                    BlcCore_SocketClose(keygw_sock);
                    connection_status = 0;
                    sum = 0;
                    BlcCore_memset(&recvMsg, 0, sizeof(recvMsg));
                    break;
                } else if (recvLen < 0) {
                    BlcCore_printf("%s %d recv error!\n", __FUNCTION__, __LINE__);
                    break;
                }

                sum += recvLen;
                if (sum == msgLen) {
                    StbMsgParse(&recvMsg);
                    recvMsgFlag = 1;
                    sum = 0;
                    BlcCore_memset(&recvMsg, 0, sizeof(recvMsg));
                }
            }
        }
    }
}
