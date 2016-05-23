#ifndef _BBCV_PLUGAPI_H_
#define _BBCV_PLUGAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

int bbcv_plug_start();
int bbcv_plug_stop();

int porting_sys_param_get(const char *key_name, void *key_value,
                          int *key_value_size);
int porting_sys_param_set(const char *key_name, const void *key_value,
                          int key_value_size);

typedef enum {
    BBCV_PlugApi_CtrlID_PlayUnBlock_S2 = 1001, //非阻塞切台
    BBCV_PlugApi_CtrlID_PlayUnBlock_P = 1002,  //非阻塞切台
    BBCV_PlugApi_CtrlID_PlayUnBlock_DVB = 1003,
    BBCV_PlugApi_CtrlID_ReturnMw = 1004,
    BBCV_PlugApi_CtrlID_OSDDisplay = 1005,
    BBCV_PlugApi_CtrlID_OSDHide = 1006,
    BBCV_PlugApi_CtrlID_MouseUIShow = 1007,
    BBCV_PlugApi_CtrlID_MouseUIHide = 1008,
    BBCV_PlugApi_CtrlID_KeyDown = 1009,
} BBCV_PlugApi_CtrlID;

typedef struct {
    unsigned int freq; // MHZ
    unsigned int symbol; // Kbps
    unsigned int mode; // 64 32 16 QAM
    unsigned int service_id;
    unsigned int audio_format; //保留
    unsigned int video_format; //保留
} BBCV_PlugApi_Param_ServiceID2;

typedef struct {
    unsigned int freq; // MHZ
    unsigned int symbol; // Kbps
    unsigned int mode; // 64 32 16 QAM
    unsigned int pmt_pid;
    unsigned int audio_format; //保留
    unsigned int video_format; //保留
} BBCV_PlugApi_Param_PMT_PID;

typedef struct {
    unsigned int freq; // MHZ
    unsigned int serviceid;
} BBCV_PlugApi_Param_DVB;

typedef enum {
    BlcReturnMwServiceType_Default =
    0, // Type=0则恢复以前状态（如果中间件未记录轨迹，则进入默认频道）
    BlcReturnMwServiceType_DVB = 1, // type=1进入DVB，参数格式是freq:serviceid
    BlcReturnMwServiceType_Browser = 2 // type=2进入browser，参数是url
} BlcReturnMwServiceType;

typedef struct {
    int service_type; //进入应用的类型 BlcReturnMwServiceType
    char *url;        //进入中间件传递给中间件的url地址
} BBCV_PlugApi_Param_ReturnMw;

typedef struct {
    char *title;   //对话框标题头字符串
    int title_len; //对话框标题头字符串长度
    char *text;    //对话框正文字符串
    int text_len;  //对话框正文字符串长度
    int time_out;  //对话框最多显示时间 单位：毫秒(千分之一秒)
    int
    dialog_style; //对话框风格，为1则是屏幕右下角不显示按钮的对话框，此时title无效，text是显示的字符串；为2则是屏幕中间显示仅含"确认"的对话框，此时title和text有效
} BBCV_PlugApi_Param_OSDDisplay;

typedef enum {
    BlcMouseUIType_Default = 1,   //默认光标（一个箭头）
    BlcMouseUIType_Crosshair = 2, //光标呈现为十字线。
    BlcMouseUIType_Pointer = 3, //光标呈现为指示链接的指针（一只手）
    BlcMouseUIType_Text = 4,    //此光标指示文本。
    BlcMouseUIType_Wait = 5, //此光标指示程序正忙（通常是一只表或沙漏）。
    BlcMouseUIType_Help = 6, //此光标指示可用的帮助（通常是一个问号或一个气球）。
} BlcMouseUIType;

typedef struct {
    int ui_type;           //鼠标显示的ui类型
    int x;                 //显示鼠标的x坐标
    int y;                 //显示鼠标的y坐标
    int full_screen_width; //全屏幕时候x绝对值最大值
    int full_screen_higth; //全屏幕时候y绝对值最大值
} BBCV_PlugApi_Param_MouseUIShow;

typedef struct {
    int keydev;   // 键值设备类型，红外遥控器
    int keystate; // 键值状态;
    int keyvalue; // 键值，逻辑键值
} BBCV_PlugApi_Param_KeyDown;

int porting_app_cmd(int ctrl_id, void *value, int value_size);

typedef enum {
    BBCV_PlugApi_EventlID_Login =
    2001, //用于中间件状态到云服务状态的切换、VOD播放的退出
    BBCV_PlugApi_EventlID_Logout = 2002, //不需要参数，用于用户退出云服务状态
    BBCV_PlugApi_EventlID_KeyValue =
    2003, //云服务状态下，键值传递给云插件(OSD状态下不需要传递键值给插件)
    BBCV_PlugApi_EventlID_KeyPlugEvent =
    2004, //云服务状态下，键值的插拔事件通知给云插件
    BBCV_PlugApi_EventlID_ConnectServer =
    2005, //不需要参数，用户调出二维码信息的时候通知云插件事件
    BBCV_PlugApi_EventlID_NoticePlayStatus =
    2006, //通知切台的状态，调用PlayUnBlock_S2,PlayUnBlock_P之后需要3秒内返回接口(不论是否成功)
    BBCV_PlugApi_EventlID_TSData =
    2007, //在任何状态(云服务状态和非云服务状态下)，ts数据事件通知云插件
} BBCV_PlugApi_EventlID;

typedef struct {
    int service_type; //进入应用的类型,例如：1,2,3
    char url[4096];   //进入应用的标识或路径等
} BBCV_PlugApi_Param_Login;

typedef struct {
    char url[4096];
} BBCV_PlugApi_Param_TSData;

typedef struct {
    int playstatus; //是否成功，<0 失败;>=0成功
} BBCV_PlugApi_Param_NoticePlayStatus;

int bbcv_plug_wait_events(int *event_id, void *value, int *value_size,
                          int timeout_us);

int bbcv_plug_wait_events_ts(int *event_id, void *value, int *value_size,
                             int timeout_us);

int bbcv_plug_wait_events_plug(int *event_id, void *value, int *value_size,
                               int timeout_us);

#ifdef __cplusplus
}
#endif

#endif
