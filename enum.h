#ifndef ENUM_H
#define ENUM_H

#define MSG_HEAD_LEN  16
#define MAX_MSG_LEN 4096

typedef struct {
    char type[8];			//消息类型
    char len[8];			//数据长度
    char value[4096];		//数据内容
}NblMsg;

typedef struct {
    char term_id[32];		//终端标志，32字节字符串
    char term_net_type[8];	//终端网络类型
    char region_id[8];		//区域编码，通常为10进制4位数
    char plug_version[8];	//插件版本号
}NblMsgHead;

//------------- 下行透传 -----------------
typedef struct {
    char type[8];			//消息类型
    char len[8];			//数据长度
    char value[4096];		//数据内容
}NblDownCmdBody;

typedef struct {
    NblMsgHead head;
    NblDownCmdBody body;
}NblDownCmd;


//------------- 上行透传 -----------------
typedef struct {
    char type[8];			//消息类型
    char len[8];			//数据长度
    char value[4096];		//数据内容
}NblUpCmdBody;

typedef struct {
    NblMsgHead head;
    NblUpCmdBody body;
}NblUpCmd;

//------------- 终端和ip信令接口 -------------------
typedef struct {
    char connect_flag[4];	//连接标签，0是开机连接，1是打开二维码连接
}NblTermMsgRegisterBody;

typedef struct {
    NblMsgHead head;
    NblTermMsgRegisterBody body;
}NblTermMsgRegister;

typedef struct {
    char type[8];			//消息类型
    char len[8];			//数据长度
    char value[4096];		//数据内容
}NblTermMsgCommBody;

typedef struct {
    NblMsgHead head;
    NblTermMsgCommBody body;
}NblTermMsgComm;

typedef struct {
    int dev_type;					//键设备类型
    int key_status;					//当前键的状态
    int key;						//键值
    short x;						//x坐标值
    short y;						//y坐标值
}NblTermMsgTCPSendkeyCommon;

typedef struct {
    int dev_type;						//键设备类型
    int key_status;						//当前键的状态
    int key;							//键值
    short x;                        	//x坐标值
    short y;                        	//y坐标值
    char console_id[15];			  	//手柄类型字段
    char is_passback;				  	//回传标志
}NblTermMsgTCPSendkeyConsole;

#endif // ENUM_H
