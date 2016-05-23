#ifndef PLUG_COMM_H_
#define PLUG_COMM_H_

#include "../include/blccore.h"

#define BLCPLUG_VERSION "10900"
#define BLCPLUG_RECV_TIMEOUT 3

#define MSG_HEAD_LEN 16
#define MAX_MSG_LEN 4096

typedef struct {
    char type[8];     //消息类型
    char len[8];      //数据长度
    char value[4096]; //数据内容
} NblMsg;

typedef struct {
    char term_id[32];      //终端标志，32字节字符串
    char term_net_type[8]; //终端网络类型
    char region_id[8];     //区域编码，通常为10进制4位数
    char plug_version[8];  //插件版本号
} NblMsgHead;

//------------- 下行透传 -----------------
typedef struct {
    char type[8];     //消息类型
    char len[8];      //数据长度
    char value[4096]; //数据内容
} NblDownCmdBody;

typedef struct {
    NblMsgHead head;
    NblDownCmdBody body;
} NblDownCmd;

//------------- 上行透传 -----------------
typedef struct {
    char type[8];     //消息类型
    char len[8];      //数据长度
    char value[4096]; //数据内容
} NblUpCmdBody;

typedef struct {
    NblMsgHead head;
    NblUpCmdBody body;
} NblUpCmd;

//------------- 终端和ip信令接口 -------------------
typedef struct {
    char append[8];
}NblTermMsgAccessBody;

typedef struct {
    NblMsgHead head;
    NblTermMsgAccessBody body;
}NblTermMsgAccess;

typedef struct {
    char connect_flag[4]; //连接标签，0是开机连接，1是打开二维码连接
} NblTermMsgRegisterBody;

typedef struct {
    NblMsgHead head;
    NblTermMsgRegisterBody body;
} NblTermMsgRegister;

typedef struct {
    char type[8];     //消息类型
    char len[8];      //数据长度
    char value[4096]; //数据内容
} NblTermMsgCommBody;

typedef struct {
    NblMsgHead head;
    NblTermMsgCommBody body;
} NblTermMsgComm;

typedef struct {
    int dev_type;   //键设备类型
    int key_status; //当前键的状态
    int key;        //键值
    short x; // x坐标值
    short y; // y坐标值
} NblTermMsgTCPSendkeyCommon;

typedef struct {
    int dev_type;   //键设备类型
    int key_status; //当前键的状态
    int key;        //键值
    short x; // x坐标值
    short y; // y坐标值
    char console_id[15]; //手柄类型字段
    char is_passback;    //回传标志
} NblTermMsgTCPSendkeyConsole;

//////////////////////////////////////////////////

typedef enum {
    NblTermNetType_BroadCast = 0,  //纯广播,无交互的网络
    NblTermNetType_RF = 1,         //外交互RF方式
    NblTermNetType_NarrowBand = 2, //机顶盒窄带方式
    NblTermNetType_CM = 3,         //机顶盒具有CM网络能力
    NblTermNetType_Lan = 4,        //机顶盒具有局域网能力
} NblTermNetType;

typedef enum {
    BlcTermMsgType_Sign = 2001,   //
    BlcTermMsgType_Over = 2007,   //页面退出包(下发到机顶盒)
    BlcTermMsgType_Switch = 2009, //切台(下发到机顶盒)
    BlcTermMsgType_ScreenNotice = 2011, // OSD对话框显示(下发到机顶盒)
    BlcTermMsgType_StbKeyStatus = 2014, //机顶盒键值状态改变包(下发到机顶盒)
    BlcTermMsgType_GroupOrder = 2015, //组合指令包(下发到机顶盒)
    BlcTermMsgType_Access = 3000,     //终端发起接入请求(上发到平台)
    BlcTermMsgType_Login = 3001,      //终端发起登录(上发到平台)
    BlcTermMsgType_Logout = 3002,     //终端发起退出(上发到平台)
    BlcTermMsgType_SendKey = 3003,    //终端发起键值发送(上发到平台)
    BlcTermMsgType_KeepAlive = 3004, //终端发起心跳(上发到平台下发到终端)
    BlcTermMsgType_FeedBack2 = 3005, //终端操作反馈包(上发到平台)
    BlcTermMsgType_SwitchApp = 3006, //切台(下发到终端)
    BlcTermMsgType_ExitOver = 3007,  //页面退出包(下发到终端)
    BlcTermMsgType_KeyStatus = 3008, //机顶盒键值状态改变包(下发到终端)
    BlcTermMsgType_OSDNotice = 3009, // OSD对话框显示(下发到终端)
    BlcTermMsgType_GroupOrder2 = 3010,  //组合指令包(下发到终端)
    BlcTermMsgType_DataTransfer = 3011, //数据透传指令包(下发到终端)
    BlcTermMsgType_PortalSet = 3012,    //个性化门户设置(下发到终端)
    BlcTermMsgType_ParamSet = 3013, //机顶盒各种参数设置(下发到终端)
    BlcTermMsgType_ParamGet = 3014, //获取机顶盒各种参数(下发到终端)
    BlcTermMsgType_ParamUp = 3015,  //机顶盒上传各种参数(上发到平台)
    BlcTermMsgType_IpanelLogin = 3016,   //机顶盒上传ipanel返回消息
    BlcTermMsgType_Userno = 3025,        //用户号读写
    BlcTermMsgType_VideoMeeting = 100,   //视频会议类型
    BlcTermMsgType_2DimensionCode = 101, //二维码处理接口
    BlcTermMsgType_OtherCmd = 200,       //第三方应用接口
} BlcTermMsgType;

typedef enum {
    BlcStbTermKeyDevType_IrrControl = 1001,
    BlcStbTermKeyDevType_WifiControl = 1002,
    BlcStbTermKeyDevType_Console = 1003,
    BlcStbTermKeyDevType_Keyboard = 1004,
    BlcStbTermKeyDevType_Mouse = 1005,
} BlcStbTermKeyDevType;

typedef enum {
    BlcKeygwSwitchType_SP = 0x01, // SERVICE ID + 频点
    BlcKeygwSwitchType_PMT_PID = 0x02, // PMT PID + 频点
    BlcKeygwSwitchType_S = 0x03, // SERVICE ID
    BlcKeygwSwitchType_Ipanel = 0x04, //切到中间件浏览器
} BlcKeygwSwitchType;

typedef struct {
    int switch_type;
    int freq;
    int serviceid_pmtpid;
    int audio_format;
    int video_format;
} stb_switch_info;

/*****************************消息结构体定义开始***********************************/
//上行消息头
typedef struct {
    int msg_type;    //消息类型
    int data_len;    //后面变长数据长度
    char data[4096]; //变长数据内容
} YunTermMsg;

//终端获取接入网关地址
typedef struct {
    char term_id[32];  //终端标志，32字节字符串
    int term_net_type; //终端网络类型
    int region_id;     //区域编码，通常为10进制4位数
    int plug_version;  //插件版本号
    char append[12];   //扩展字段，用于填充64字节，备用
} YunTermMsg_Access;

//终端注册、心跳发送到平台
typedef struct {
    char term_id[32];  //终端标志，32字节字符串
    int term_net_type; //终端网络类型
    int region_id;     //区域编码，通常为10进制4位数
    int plug_version;  //插件版本号
    int connect_flag; //连接标签，0是开机连接，1是打开二维码连接
} YunTermMsg_KeepAlive;

//终端登录平台
typedef struct {
    char term_id[32];    //终端标志，32字节字符串
    int term_net_type;   //终端网络类型
    int region_id;       //区域编码，通常为10进制4位数
    int portal_id;       //个性化门户标志
    int plug_version;    //插件版本号
    char term_info[128]; //机顶盒版本
    char param[2048];    //变长字符串参数内容
} YunTermMsg_Login;

//终端登录平台,ipanel返回
typedef struct {
    char term_id[32];    //终端标志，32字节字符串
    int term_net_type;   //终端网络类型
    int region_id;       //区域编码，通常为10进制4位数
    int portal_id;       //个性化门户标志
    int plug_version;    //插件版本号
    char term_info[128]; //机顶盒版本
    char param[2048];    //变长字符串参数内容
} YunTermMsg_IpanelLogin;

//终端退出平台
typedef struct {
    char term_id[32];  //终端标志，32字节字符串
    int term_net_type; //终端网络类型
} YunTermMsg_Logout;

//终端发送红外、键盘、鼠标键值TCP
typedef struct {
    int dev_type;   //键设备类型
    int key_status; //当前键的状态
    int key;        //键值
    short x; // x坐标值
    short y; // y坐标值
} YunTermMsg_Sendkey_Common;

//终端发送游戏手柄键值TCP
typedef struct {
    int dev_type;   //键设备类型
    int key_status; //当前键的状态
    int key;        //键值
    short x; // x坐标值
    short y; // y坐标值
    char console_id[15];       //手柄类型字段
    char is_passback;          //回传标志
    char console_keyvalue[50]; //游戏手柄的键值(手柄使用)
} YunTermMsg_Sendkey_Console;

//终端指令反馈平台
typedef struct {
    char term_id[32]; //终端标志，32字节字符串
    int
    order_type;      //反馈的指令类型(该位与下行指令的退出相等，则表示退出成功或失败；其它类同)
    int return_code; //返回值(为大于等于0则表示操作成功；为小于0 表示操作失败)
} YunTermMsg_FeedBack;

//终端上发参数指令
typedef struct {
    char term_id[32]; //终端标志，32字节字符串
    char param[4096]; //变长字符串参数内容
} YunTermMsg_ParamUp;

//下行消息
typedef struct {
    char term_id[32];
    int msg_type;
    char data[4096];
} YunTermMsg_Recv;

/*****************************消息结构体定义结束***********************************/

typedef enum {
    BlcDemandStatus_Common = 0,
    BlcDemandStatus_Sign = 1,
    BlcDemandStatus_Signing = 2,
    BlcDemandStatus_Quit = 3,
} BlcDemandStatus;

void NoticeFail(const char *osd_text);
int GetCloudAddr(BlcIPAddr *addr, int time_out);
void BlcStb_RecvIPOrderProcess();
int StbOrderParse(char *rcvMsg, int isInteractive);
int SendMsgToCloud(int sockid, int msgType, const char *msg, int msgLen);
int StbLogoutCloud(int is_sendquit);

#endif
