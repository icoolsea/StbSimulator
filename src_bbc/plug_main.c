#include "../include/blccore.h"
#include "plug_comm.h"
#include "cJSON.h"

#include <stdarg.h>
#include <stdlib.h>

// mouse ctrol
int mouse_in_flag = BlcKeyDevPlugType_MousePlugout;
int mouse_ui_type = BlcMouseUIType_Default;
int full_screen_width = 1280;
int full_screen_higth = 720;

int standardid = 0;

int osd_timeout = 5000;

BlcDemandStatus demand_status = BlcDemandStatus_Quit;
// static int net_status = BlcNetPropertyID_NetFailed;
int connection_status = 0; // 0代表不OK,大于0代表OK

char stbid[32];
int region_id = 1537;
static char term_info[128];
static int net_type = NblTermNetType_BroadCast;
static int portal_id = 0;

int access_sock = 0;
int keygw_sock = 0;
BlcIPAddr accessgw_addr;
BlcIPAddr keygw_addr;

int ipanel_status = 0;    //是否处于ipanel状态
int osdnotice_status = 0; //是否处于osd显示状态
int wait_playstatus = 0;
BBCV_PlugApi_Param_NoticePlayStatus play_status;

char last_ts_buffer[184];
BlcTime last_ts_time;

int thread_id[8];

int auto_reconnect_mode = 1; //自动重连模式，0关闭，1开启
int is_first_connect = 1;
int is_continue_connect = 1; //是否继续连接

extern int bbcv_plug_bbcvkeytomwkey(int standardid, int bbcvkey);
extern int bbcv_plug_mwkeytobbcvkey(int standardid, int mwkey);

void NoticeFail(const char *osd_text) {
    BBCV_PlugApi_Param_OSDDisplay notice_wind;
    notice_wind.title = "异常提醒:";
    notice_wind.title_len = BlcCore_strlen(notice_wind.title);
    notice_wind.text = (char *)osd_text;
    notice_wind.text_len = BlcCore_strlen(notice_wind.text);
    notice_wind.time_out = 60 * 1000;
    notice_wind.dialog_style = 2;

    porting_app_cmd(BBCV_PlugApi_CtrlID_OSDDisplay, &notice_wind,
                    sizeof(notice_wind));
}

static int CreateTCPConnection(int *sockid, BlcIPAddr *addr) {
    if (sockid == NULL || addr == NULL) {
        BlcCore_printf("%s %d parament is error \n", __FUNCTION__, __LINE__);
        return -1;
    }

    if ((*sockid = BlcCore_SocketCreate(BLCNET_SOCK_STREAM)) == -1) {
        BlcCore_printf("%s %d creat socket error \n", __FUNCTION__, __LINE__);
        return -2;
    }

    BlcIPAddr ipaddr;
    BlcCore_strncpy(ipaddr.ip, addr->ip, sizeof(ipaddr.ip));
    ipaddr.port = addr->port;

    int i = 0;
    for (i = 0; i < 3; i++) {
        if (BlcCore_TCPConnect(*sockid, &ipaddr) != -1)
            break;

        if (i == 2) {
            BlcCore_printf("%s %d TCP connect cloud server [%s:%d] fail!\n",
                           __FUNCTION__, __LINE__, addr->ip, addr->port);
            BlcCore_SocketClose(*sockid);
            return -3;
        }
    }

    return 0;
}

static int StbPropertyUpdate() {
    char stb_regionid_str[16];
    char stb_keystandardid_str[16];
    char stb_nettype_str[16];
    char gwaddr[128];

    char stb_soft_version[64];
    char stb_hw_version[64];
    char stb_mw_version[64];

    BlcCore_memset(stbid, 0, sizeof(stbid));
    BlcCore_memset(gwaddr, 0, sizeof(gwaddr));

    BlcCore_memset(stb_regionid_str, 0, sizeof(stb_regionid_str));
    BlcCore_memset(stb_keystandardid_str, 0, sizeof(stb_keystandardid_str));
    BlcCore_memset(stb_nettype_str, 0, sizeof(stb_nettype_str));
    BlcCore_memset(stb_soft_version, 0, sizeof(stb_soft_version));
    BlcCore_memset(stb_hw_version, 0, sizeof(stb_hw_version));
    BlcCore_memset(stb_mw_version, 0, sizeof(stb_mw_version));

    int stbid_size = sizeof(stbid);
    if (0 != porting_sys_param_get("stb_id", stbid, &stbid_size)) {
        BlcCore_printf("%s %d porting_sys_param_get stb_id error\n", __FUNCTION__,
                       __LINE__);
        return -1;
    }

    int region_id_size = sizeof(stb_regionid_str);
    if (0 !=
            porting_sys_param_get("region_id", stb_regionid_str, &region_id_size)) {
        BlcCore_printf("%s %d porting_sys_param_get region_id error\n",
                       __FUNCTION__, __LINE__);
        return -2;
    }
    region_id = BlcCore_atoi(stb_regionid_str);

    int standardid_size = sizeof(stb_keystandardid_str);
    if (0 != porting_sys_param_get("key_standard_id", stb_keystandardid_str,
                                   &standardid_size)) {
        BlcCore_printf("%s %d porting_sys_param_get key_standard_id error\n",
                       __FUNCTION__, __LINE__);
        return -3;
    }
    standardid = BlcCore_atoi(stb_keystandardid_str);

    int net_type_size = sizeof(stb_nettype_str);
    if (0 != porting_sys_param_get("net_type", stb_nettype_str, &net_type_size)) {
        BlcCore_printf("%s %d porting_sys_param_get net_type error\n", __FUNCTION__,
                       __LINE__);
        return -4;
    }
    net_type = BlcCore_atoi(stb_nettype_str);

    int gwaddr_size = sizeof(gwaddr);
    if (0 != porting_sys_param_get("cloud_addr", gwaddr, &gwaddr_size)) {
        BlcCore_printf("%s %d porting_sys_param_get cloud_addr error\n",
                       __FUNCTION__, __LINE__);
        return -5;
    }
    int ret = BlcCore_sscanf(gwaddr, "%32[^:]:%d", accessgw_addr.ip,
                             &accessgw_addr.port);
    if (ret != 2) {
        BlcCore_printf("%s %d parse accessgw_addr error:gwaddr = %s\n",
                       __FUNCTION__, __LINE__, gwaddr);
        return -6;
    }

    int stb_soft_version_size = sizeof(stb_soft_version);
    if (0 != porting_sys_param_get("stb_soft_version", stb_soft_version,
                                   &stb_soft_version_size))
        BlcCore_printf("%s %d porting_sys_param_get stb_soft_version error\n",
                       __FUNCTION__, __LINE__);

    int stb_hw_version_size = sizeof(stb_hw_version);
    if (0 != porting_sys_param_get("stb_hw_version", stb_hw_version,
                                   &stb_hw_version_size))
        BlcCore_printf("%s %d porting_sys_param_get stb_hw_version error\n",
                       __FUNCTION__, __LINE__);

    int stb_mw_version_size = sizeof(stb_mw_version);
    if (0 != porting_sys_param_get("stb_mw_version", stb_mw_version,
                                   &stb_mw_version_size))
        BlcCore_printf("%s %d porting_sys_param_get stb_mw_version error\n",
                       __FUNCTION__, __LINE__);

    BlcCore_printf(
                "%s %d***********************BlcStbInfoSt*************************\n",
                __FUNCTION__, __LINE__);
    BlcCore_printf("%s %d plug_version  : %s\n", __FUNCTION__, __LINE__,
                   BLCPLUG_VERSION);

    BlcCore_printf("%s %d stbid         : %s\n", __FUNCTION__, __LINE__, stbid);
    BlcCore_printf("%s %d regionid      : %d\n", __FUNCTION__, __LINE__,
                   region_id);
    BlcCore_printf("%s %d standardid    : %d\n", __FUNCTION__, __LINE__,
                   standardid);
    BlcCore_printf("%s %d accessgwaddr  : %s\n", __FUNCTION__, __LINE__, gwaddr);
    BlcCore_printf("%s %d net_type      : %d\n", __FUNCTION__, __LINE__,
                   net_type);

    BlcCore_printf("%s %d stb_soft_version: %s\n", __FUNCTION__, __LINE__,
                   stb_soft_version);
    BlcCore_printf("%s %d stb_hw_version  : %s\n", __FUNCTION__, __LINE__,
                   stb_hw_version);
    BlcCore_printf("%s %d stb_mw_version  : %s\n", __FUNCTION__, __LINE__,
                   stb_mw_version);
    BlcCore_printf(
                "%s %d************************************************************\n",
                __FUNCTION__, __LINE__);

    BlcCore_memset(term_info, 0, sizeof(term_info));

    BlcCore_sprintf(term_info, "%s:%s:%s", stb_mw_version, stb_hw_version,
                    stb_soft_version);
    BlcCore_printf("%s %d net_type = %d,term_info = %s\n", __FUNCTION__, __LINE__,
                   net_type, term_info);

    return 0;
}

static char *StbCjsonCreate(char *ret_string, const char *string, ...) {
    cJSON *send_cmd;
    send_cmd = cJSON_CreateObject();
    va_list ptr;
    va_start(ptr, string);
    const char *key = string;
    char *value = NULL;

    while (BlcCore_strcmp(key, "end") != 0) {
        value = va_arg(ptr, char *);
        cJSON_AddStringToObject(send_cmd, key, value);
        key = va_arg(ptr, char *);
    }
    va_end(ptr);

    char *cjson_str = cJSON_PrintUnformatted(send_cmd);
    BlcCore_sprintf(ret_string, "%s", cjson_str);
    cJSON_Delete(send_cmd);
    BlcCore_Free(cjson_str);
    cjson_str = NULL;
    return ret_string;
}

int StbLogoutCloud(int is_sendquit) //是否发送退出请求，0不发送，1发送
{
    if (BlcDemandStatus_Quit == demand_status) {
        BlcCore_printf("%s %d It is already quit! \n", __FUNCTION__, __LINE__);
        return 0;
    }

    // int ret = blcplug_set_property(BlcPlayCtl_CtrlID_Recall,NULL, 0);

    BBCV_PlugApi_Param_ReturnMw param_returnmw;
    param_returnmw.service_type = BlcReturnMwServiceType_Default;
    param_returnmw.url = NULL;

    int ret = porting_app_cmd(BBCV_PlugApi_CtrlID_ReturnMw, &param_returnmw,
                              sizeof(param_returnmw));
    if (ret != 0)
        BlcCore_printf("func = %s line = %d recall failed \n", __FUNCTION__,
                       __LINE__);
    else
        BlcCore_printf("%s %d Recall successfull! \n", __FUNCTION__, __LINE__);

    demand_status = BlcDemandStatus_Quit;

    // if (BlcNetPropertyID_NetSuccess == net_status && is_sendquit == 1)
    if (is_sendquit == 1) {
        char send_msg[128];
        StbCjsonCreate(send_msg, "cmd", "logout",  "termid",
                           stbid, "termtype", "1001", "end");
        SendMsgToCloud(keygw_sock, BlcTermMsgType_Logout, send_msg,
                       BlcCore_strlen(send_msg));
    }

    if (is_continue_connect == 0) {
        BlcCore_SocketClose(keygw_sock);
        connection_status = 0;
    }

    return 0;
}

static int StbStartLoginCloud(char *para, int time_out,
                              int is_ipanel_return) //参数单位秒
{
    BlcCore_printf("%s %d\n", __FUNCTION__, __LINE__);

    if (connection_status == 0) {
        if ((access_sock = BlcCore_SocketCreate(BLCNET_SOCK_DGRAM)) == -1) {
            BlcCore_printf("%s %d creat socket error \n", __FUNCTION__, __LINE__);
            goto label;
        }

        if (GetCloudAddr(&keygw_addr, time_out) < 0) {
            BlcCore_printf("%s %d GetCloudAddr error \n", __FUNCTION__, __LINE__);
            goto label;
        }

        if (CreateTCPConnection(&keygw_sock, &keygw_addr) < 0) {
            BlcCore_printf("%s %d TCP connect intergw error \n", __FUNCTION__,
                           __LINE__);
            BlcCore_SocketClose(keygw_sock);
            goto label;
        }

        connection_status = 1;
    }

label:
    //开始登陆
    if (connection_status != 0) {
        BlcCore_printf("%s %d Net is OK, start interactive demand!\n", __FUNCTION__,
                       __LINE__);
        BlcCore_printf("%s %d Start Login stb_id = %s region id = %d \n",
                       __FUNCTION__, __LINE__, stbid, region_id);

        int msg_type;
        char login_str[512];
        char tmp_region_id[8] = {0};
        char tmp_net_tye[8] = {0};
        char tmp_plugversion[8]={0};
        BlcCore_sprintf(tmp_region_id, "%d", region_id);
        BlcCore_sprintf(tmp_net_tye, "%d", net_type);
        BlcCore_sprintf(tmp_plugversion,"%d",BLCPLUG_VERSION);
        if (is_ipanel_return == 0) {
            msg_type = BlcTermMsgType_Login;
            StbCjsonCreate(login_str, "cmd", "login", "logintype", "1", "termid",
                           stbid, "plugversion", tmp_plugversion, "portalid", "0", "regionid",
                           tmp_region_id, "terminfo", term_info, "termnet",
                           tmp_net_tye, "termtype", "1001", "url", para, "aurl", "",
                           "expandinfo", "", "end");
            BlcCore_printf("%s %d BlcTermMsgType_Login...!\n", __FUNCTION__,
                           __LINE__);
        } else {
            msg_type = BlcTermMsgType_IpanelLogin;
            StbCjsonCreate(login_str, "cmd", "login", "logintype", "2", "termid",
                           stbid, "plugversion", tmp_plugversion, "portalid", "0", "regionid",
                           tmp_region_id, "terminfo", term_info, "termnet",
                           tmp_net_tye, "termtype", "1001", "url", para, "aurl", "",
                           "expandinfo", "", "end");
            BlcCore_printf("%s %d BlcTermMsgType_IpanelLogin...!\n", __FUNCTION__,
                           __LINE__);
        }

        if (SendMsgToCloud(keygw_sock, msg_type, login_str,
                           BlcCore_strlen(login_str)) >= 0) {
            demand_status = BlcDemandStatus_Signing;
            return 0;
        } else {
            BlcCore_printf("%s %d TCP send fail...\n", __FUNCTION__, __LINE__);
            NoticeFail("网络消息发送失败!请确保您的网线已正确连接后重新登录。");
        }
    } else {
        BlcCore_printf("%s %d TCP link is not OK...\n", __FUNCTION__, __LINE__);
        NoticeFail("网络连接失败!请确保您的网线已正确连接后重新登录。");
    }

    StbLogoutCloud(0); //如果登录失败就退出

    return -1;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief StbOSDNotice
/// \param p_data
/// \param size
/// \return
///
/// //////////////////////////////////////////////////////////////////////////

static int StbWaitPlayStatus(int switch_type ,void* arg) {

    BlcCore_printf("$$$$$$$$$$$$$$$$%s %d IN$$$$$$$$$$$$$$$$\n",__FUNCTION__,__LINE__);
    BBCV_PlugApi_Param_ServiceID2  *property2 = NULL;
    BBCV_PlugApi_Param_PMT_PID     *property_p = NULL;

    if(switch_type == BlcKeygwSwitchType_SP)
       property2 = (BBCV_PlugApi_Param_ServiceID2 *)arg;
    else if(switch_type == BlcKeygwSwitchType_PMT_PID)
       property_p = (BBCV_PlugApi_Param_PMT_PID *)arg;
    else {
       BlcCore_printf("%s %d OUT no switch_type:%d",__FUNCTION__,__LINE__,switch_type); return -3;}

    int wait_total_time = 5000;
    int temp_time = 0;
    while (1) {
        if (temp_time > wait_total_time) {
            // time_out
            wait_playstatus = 0;
            if(switch_type == BlcKeygwSwitchType_SP )
                BlcCore_printf("%s %d BBCV_PlugApi_CtrlID_PlayUnBlock_S2 timeout  freq = "
                               "%d,service_id = %d\n",
                               __FUNCTION__, __LINE__, property2->freq,
                               property2->service_id);
            else if(switch_type == BlcKeygwSwitchType_PMT_PID)
                BlcCore_printf("%s %d BBCV_PlugApi_CtrlID_PlayUnBlock_P timeout  freq = "
                               "%d,pmt_pid = %d\n",
                               __FUNCTION__, __LINE__, property_p->freq,
                               property_p->pmt_pid);
            BlcCore_printf("$$$$$$$$$$$$$$$$%s %d OUT$$$$$$$$$$$$$$$$\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (wait_playstatus == 2) {
            wait_playstatus = 0;

            if (play_status.playstatus >= 0) {
                ipanel_status = 0;
            if(switch_type == BlcKeygwSwitchType_SP )
                BlcCore_printf(
                            "BBCV_PlugApi_CtrlID_PlayUnBlock_S2 successfull! freq = "
                            "%d,service_id = %d####^_^^_^#####\n",
                            property2->freq, property2->service_id);
             else if(switch_type == BlcKeygwSwitchType_PMT_PID)
                BlcCore_printf(
                            "BBCV_PlugApi_CtrlID_PlayUnBlock_P successfull! freq = %d,pmt_pid "
                            "= %d$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
                            property_p->freq, property_p->pmt_pid);
            BlcCore_printf("$$$$$$$$$$$$$$$$%s %d OUT$$$$$$$$$$$$$$$$\n",__FUNCTION__,__LINE__);
                return 0;
            } else {
                 if(switch_type == BlcKeygwSwitchType_SP )
                     BlcCore_printf("%s %d BBCV_PlugApi_CtrlID_PlayUnBlock_S2 fail  freq = "
                                    "%d,service_id = %d\n",
                                    __FUNCTION__, __LINE__, property2->freq,
                                    property2->service_id);
                 else if(switch_type == BlcKeygwSwitchType_PMT_PID)
                     BlcCore_printf(
                                 "%s %d BBCV_PlugApi_CtrlID_PlayUnBlock_P failed  freq = %d,pmt_pid "
                                 "= %d\n",
                                 __FUNCTION__, __LINE__, property_p->freq, property_p->pmt_pid);
                BlcCore_printf("$$$$$$$$$$$$$$$$%s %d OUT$$$$$$$$$$$$$$$$\n",__FUNCTION__,__LINE__);
                return -2;
            }
        }

        BlcCore_ThreadDelay(20);
        temp_time += 20;
    }
}

static int StbStrParse(char* p_data,BBCV_PlugApi_Param_ServiceID2*arg,BBCV_PlugApi_Param_PMT_PID*arg2,char*ret_url)
{
    if(p_data == NULL)return -1;
    cJSON *root = cJSON_Parse(p_data);
    if (!root) {
        BlcCore_printf("%s %d This string not json\n", __FUNCTION__, __LINE__);
        return -2;
    }
    cJSON *info = cJSON_GetObjectItem(root, "switchinfo");
    cJSON *type = cJSON_GetObjectItem(root, "switchtype");
    cJSON *url =  cJSON_GetObjectItem(root,"url");//暂时不可用，VOD暂时不可用
    if (!info || !type) {
        cJSON_Delete(root);
        return -3;
    }
    stb_switch_info switch_info;
    BlcCore_memset(&switch_info, 0, sizeof(stb_switch_info));


    BlcCore_printf("%s %d Recv Switch && Switch info =%s %s....\n", __FUNCTION__, __LINE__,
                   type->valuestring, info->valuestring);

    //解析切台信息
    int app_level = 0;
    char data[2048];

    BlcCore_memset(&data, 0, sizeof(data));

    int ret = BlcCore_sscanf(type->valuestring, "%d", &switch_info.switch_type);
    if (ret != 1) {
        BlcCore_printf("%s %d parse switch type error \n", __FUNCTION__, __LINE__);
        cJSON_Delete(root);
        return -4;
    }
    if (BlcKeygwSwitchType_Ipanel != switch_info.switch_type) {
        int ret =
                BlcCore_sscanf(info->valuestring, "%d:%d:%u:%u:%d", &switch_info.freq,
                               &switch_info.serviceid_pmtpid, &switch_info.video_format,
                               &switch_info.audio_format, &app_level);
        if (ret != 5) {
            BlcCore_printf("%s %d parse switch error \n", __FUNCTION__, __LINE__);
            cJSON_Delete(root);
            return -5;
        }
    }
    if(switch_info.switch_type == BlcKeygwSwitchType_SP){



         arg->freq = switch_info.freq;
         arg->service_id = switch_info.serviceid_pmtpid;
         arg->video_format = switch_info.video_format;
         arg->audio_format = switch_info.audio_format;

         cJSON_Delete(root);
         return BlcKeygwSwitchType_SP;

    }
    else if(switch_info.switch_type == BlcKeygwSwitchType_PMT_PID){


        arg2->freq = switch_info.freq;
        arg2->pmt_pid = switch_info.serviceid_pmtpid;
        arg2->video_format = switch_info.video_format;
        arg2->audio_format = switch_info.audio_format;

        cJSON_Delete(root);
        return BlcKeygwSwitchType_PMT_PID;
    }
    else if (switch_info.switch_type == BlcKeygwSwitchType_Ipanel){

        BlcCore_sprintf(ret_url,"%s",url->valuestring);
        cJSON_Delete(root);
        return BlcKeygwSwitchType_Ipanel;
    }
    else{
        cJSON_Delete(root);
        return -6;
    }


}

static int StbSwitchCloud(char *p_data, int size) {
    if (p_data == NULL || size < 0) {
        BlcCore_printf("%s %d parameter is error!\n", __FUNCTION__, __LINE__);
        return -1;
    }
    char ret_url[512]={0};
    BBCV_PlugApi_Param_ServiceID2 property2 ;
    BBCV_PlugApi_Param_PMT_PID property_p;
    int switch_type = StbStrParse(p_data,&property2,&property_p,ret_url);
    switch (switch_type) {

    case BlcKeygwSwitchType_SP: {

        porting_app_cmd(BBCV_PlugApi_CtrlID_PlayUnBlock_S2, &property2,
                        sizeof(property2));
        wait_playstatus = 1;

        int opt_ret = StbWaitPlayStatus(BlcKeygwSwitchType_SP,(void*)&property2);
        if (opt_ret == 0)

            return 0;

    } break;
    case BlcKeygwSwitchType_PMT_PID: {

        porting_app_cmd(BBCV_PlugApi_CtrlID_PlayUnBlock_P, &property_p,
                        sizeof(property_p));
        wait_playstatus = 1;

        int opt_ret = StbWaitPlayStatus(BlcKeygwSwitchType_PMT_PID,(void*)&property_p);
        if (opt_ret == 0)

            return 0;


    } break;

    case BlcKeygwSwitchType_Ipanel: {
        int url_len =BlcCore_strlen(ret_url);
        if (url_len == 0) {
            BlcCore_printf("wrong url\n");

            return -6;
        }
        BBCV_PlugApi_Param_ReturnMw param_returnmw;
        param_returnmw.service_type = BlcReturnMwServiceType_Browser;
        param_returnmw.url = &ret_url;

        porting_app_cmd(BBCV_PlugApi_CtrlID_ReturnMw, &param_returnmw,
                        sizeof(param_returnmw));

        ipanel_status = 1;
        BlcCore_printf("BBCV_PlugApi_CtrlID_ReturnMw successfull! url = %s "
                       "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
                       ret_url);

        return 1; //修改为1，之前是0，主要为了区分中间件与其他切台方式做判断

    } break;
    default: {

        BlcCore_printf("%s %d error switch_type = %d\n", __FUNCTION__, __LINE__,
                       switch_type);
        return switch_type;
    } break;
    }

    BlcCore_printf("SwitchApp fail... \n");
    NoticeFail("抱歉，服务目前繁忙，请您稍后再试，谢谢！");

    return -8;
}

static int StbOSDNotice(char *p_data, int size) {
    if (p_data == NULL || size < 0) {
        BlcCore_printf("%s %d parameter is error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    BlcCore_printf("Screen OSD $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    BlcCore_printf("%s %d notice = %s....\n", __FUNCTION__, __LINE__, p_data);

    char *p_index1 = NULL;
    char *p_index2 = NULL;
    char buffer[2048];
    BlcCore_memset(buffer, 0, sizeof(buffer));
    BlcCore_strncpy(buffer, p_data, sizeof(buffer));

    BBCV_PlugApi_Param_OSDDisplay notice_wind;
    notice_wind.title = NULL;
    notice_wind.title_len = 0;
    notice_wind.text = NULL;
    notice_wind.text_len = 0;
    notice_wind.time_out = 0;
    notice_wind.dialog_style = 0;

    /////////////////解析开始///////////////////////////
    // dialog_style
    p_index1 = buffer;
    p_index2 = BlcCore_strchr(p_index1, '#');
    if (p_index2 == NULL) {
        BlcCore_printf("%s %d recvice notice error,no '#'....\n", __FUNCTION__,
                       __LINE__);
        return -2;
    }
    *p_index2 = 0;
    notice_wind.dialog_style = BlcCore_atoi(p_index1);
    if (notice_wind.dialog_style != 1 && notice_wind.dialog_style != 2) {
        BlcCore_printf("%s %d dialog_style error %d \n", __FUNCTION__, __LINE__,
                       notice_wind.dialog_style);
        return -3;
    }

    // title
    p_index1 = p_index2 + 1;
    p_index2 = BlcCore_strchr(p_index1, '#');
    if (p_index2 == NULL) {
        BlcCore_printf("%s %d recvice notice error,no '#'....\n", __FUNCTION__,
                       __LINE__);
        return -2;
    }
    *p_index2 = 0;
    notice_wind.title = p_index1;
    notice_wind.title_len = BlcCore_strlen(p_index1);
    // text
    p_index1 = p_index2 + 1;
    p_index2 = BlcCore_strchr(p_index1, '#');
    if (p_index2 == NULL) {
        BlcCore_printf("%s %d recvice notice error,no '#'....\n", __FUNCTION__,
                       __LINE__);
        return -2;
    }
    *p_index2 = 0;
    notice_wind.text = p_index1;
    notice_wind.text_len = BlcCore_strlen(p_index1);
    // time_out
    p_index1 = p_index2 + 1;
    notice_wind.time_out = BlcCore_atoi(p_index1);
    osd_timeout = notice_wind.time_out;
    if (notice_wind.time_out <= 0) {
        BlcCore_printf("%s %d time_out error %d \n", __FUNCTION__, __LINE__,
                       notice_wind.time_out);
        return -3;
    }
    notice_wind.time_out = notice_wind.time_out * 1000;

    /////////////////解析结束///////////////////////////

    if (0 != porting_app_cmd(BBCV_PlugApi_CtrlID_OSDDisplay, &notice_wind,
                             sizeof(notice_wind))) {
        BlcCore_printf("%s %d notice fail....\n", __FUNCTION__, __LINE__);
        return -4;
    }

    BlcCore_printf("%s %d notice success,notice_wind.dialog_style = %d....\n",
                   __FUNCTION__, __LINE__, notice_wind.dialog_style);

    if (notice_wind.dialog_style == 2)
        osdnotice_status = 1;
    else if (notice_wind.dialog_style == 1) {
        BlcCore_ThreadDelay(5000);
        porting_app_cmd(BBCV_PlugApi_CtrlID_OSDHide, NULL, 0);

        BlcCore_printf("%s %d notice hide success....\n", __FUNCTION__, __LINE__);
    }

    return 0;
}

static int StbConnectServer() {
    BlcCore_printf("%s %d recv connect server sem...\n", __FUNCTION__, __LINE__);

    //发起和服务器的连接请求
    if (connection_status == 0) {
        is_first_connect = 0;
        auto_reconnect_mode = 1;
        is_continue_connect = 1;
    } else {
        BlcCore_printf("%s %d connection is already OK!\n", __FUNCTION__, __LINE__);
    }

    return 0;
}

static int StbKeepConnectResp(char *p_data, int size) {
    int flag;

    if (p_data == NULL || size < 0) {
        BlcCore_printf("%s %d parameter is error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    BlcCore_printf(
                "Recv KeepConnectResp $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    BlcCore_printf("%s %d KeepConnectResp = %s....\n", __FUNCTION__, __LINE__,
                   p_data);

    flag = BlcCore_atoi(p_data);
    if (flag == 0) {
        if (demand_status != BlcDemandStatus_Sign) {
            BlcCore_SocketClose(keygw_sock);
            connection_status = 0;
            auto_reconnect_mode = 0;
            is_continue_connect = 0;
        }
    } else if (flag == 1) {
        is_continue_connect = 1;
    } else {
    }

    return 0;
}

static int StbOpsFeedback(char *msgtype, int ret_code) {
    BlcCore_printf("%s %d send feedback, msgtype = %s, return_code = %d!\n",
                   __FUNCTION__, __LINE__, msgtype, ret_code);

    char feedback_str[512] = {0};
    char ret_str[10] = {0};

    BlcCore_sprintf(ret_str, "%d", ret_code);

    StbCjsonCreate(feedback_str, "cmd", "feedback", "retcmd", msgtype, "retcode",
                   ret_str, "termtype", "1001", "end");

    return SendMsgToCloud(keygw_sock, BlcTermMsgType_FeedBack2, feedback_str,
                          BlcCore_strlen(feedback_str));
}

int StbOrderParse(char *recv_string,
                  int is_interactive) // is_feedback是否交互型0 or 1
{
    cJSON *root = cJSON_Parse(recv_string);
    if (!root) {
        BlcCore_printf("This string not cJSON\n");
        return -1;
    }

    cJSON *get_cmd = cJSON_GetObjectItem(root, "cmd");
    if (!get_cmd) {
        BlcCore_printf("This cJSON no the key of cmd\n");
        cJSON_Delete(root);
        return -2;
    }
    char *cmd = cJSON_GetObjectItem(root, "cmd")->valuestring;
    int ops_ret = -1;
    if (BlcCore_strcmp("SwitchKeyApp", cmd) == 0) {
        ops_ret = StbSwitchCloud(recv_string, BlcCore_strlen(recv_string));
        if (is_interactive == 1)
            StbOpsFeedback(cmd, ops_ret);

        if (ops_ret >= 0) {
            if (0 == ops_ret) //最新修改2014--05-08
            {
                demand_status = BlcDemandStatus_Sign;
            }
            auto_reconnect_mode = 1;
        } else {
            StbLogoutCloud(1); //如果切台失败就退出
        }
        cJSON_Delete(root);
        return ops_ret;
    } else if (BlcCore_strcmp("NoticeOver", cmd) == 0) {
        ops_ret = StbLogoutCloud(0);

        if (is_interactive == 1)
            StbOpsFeedback(cmd, ops_ret);
        cJSON_Delete(root);
        return ops_ret;

    } else if (BlcCore_strcmp("switchdvb", cmd) == 0) {
        cJSON *type = cJSON_GetObjectItem(root, "switchtype");
        cJSON *freq = cJSON_GetObjectItem(root, "freq");
        cJSON *serviceid = cJSON_GetObjectItem(root, "serviceid");
        cJSON *pid = cJSON_GetObjectItem(root, "pid");
        int i_freq = BlcCore_atoi(freq->valuestring);
        int i_serviceid = BlcCore_atoi(serviceid->valuestring);
        if (!type || !freq || !serviceid || !pid) {
            cJSON_Delete(root);
            return -4;
        }
        if (demand_status != BlcDemandStatus_Sign) {
            BBCV_PlugApi_Param_DVB property;
            BlcCore_memset(&property, 0, sizeof(property));
            property.freq = i_freq;
            property.serviceid = i_serviceid;

            if (0 == porting_app_cmd(BBCV_PlugApi_CtrlID_PlayUnBlock_DVB, &property,
                                     sizeof(property))) {
                demand_status = BlcDemandStatus_Quit;

                BlcCore_printf(
                            "BBCV_PlugApi_CtrlID_PlayUnBlock_DVB successfull! freq = "
                            "%d,service_id = %d$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
                            property.freq, property.serviceid);
                cJSON_Delete(root);
                return 0;
            }
        } else {
            char switch_dvb_info[128];
            BlcCore_memset(switch_dvb_info, 0, sizeof(switch_dvb_info));
            BlcCore_sprintf(switch_dvb_info, "%d:%d", i_freq, i_serviceid);

            BBCV_PlugApi_Param_ReturnMw param_returnmw;
            param_returnmw.service_type = BlcReturnMwServiceType_DVB;
            param_returnmw.url = switch_dvb_info;
            int ret = porting_app_cmd(BBCV_PlugApi_CtrlID_ReturnMw, &param_returnmw,
                                      sizeof(param_returnmw));

            if (ret != 0) {
                BlcCore_printf(
                            "func = %s line = %d BBCV_PlugApi_CtrlID_ReturnMw failed \n",
                            __FUNCTION__, __LINE__);
                cJSON_Delete(root);
                return ret;
            } else
                BlcCore_printf(
                            "BBCV_PlugApi_CtrlID_ReturnMw dvb successfull! switch_dvb_info = "
                            "%s $$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
                            switch_dvb_info);

            demand_status = BlcDemandStatus_Quit;

            if (is_continue_connect == 0) {
                BlcCore_SocketClose(keygw_sock);
                connection_status = 0;
            }
            cJSON_Delete(root);
            return 0;
        }
    } else if (BlcCore_strcmp("screennotice", cmd) == 0) {
        cJSON *msg = cJSON_GetObjectItem(root, "noticemsg");
        if (!msg) {
            BlcCore_printf("This json no noticemsg\n");
            cJSON_Delete(root);
            return -5;
        }
        ops_ret = StbOSDNotice(msg->valuestring, BlcCore_strlen(msg->valuestring));

        if (is_interactive == 1)
            StbOpsFeedback(cmd, ops_ret);
        cJSON_Delete(root);
        return ops_ret;
    }

    return 0;
}

static unsigned int blc_key_encryption(unsigned int key) { return (key + 1); }

//功能:发送键值给平台
static int BlcStb_KeySend(BlcKeyValueData *keyvalue) {
    YunTermMsg_Sendkey_Common common_key;
    YunTermMsg_Sendkey_Console console_key;

    BlcCore_memset(&common_key, 0, sizeof(common_key));
    BlcCore_memset(&console_key, 0, sizeof(console_key));

    int keybuf[128];
    int data_len;

    BlcCore_memset(keybuf, 0, sizeof(keybuf));

    if (NULL != keyvalue) {
        switch (keyvalue->keydev) {
        case BlcTermKeyDevType_Irr: {
            common_key.dev_type = BlcStbTermKeyDevType_IrrControl;
            common_key.key = blc_key_encryption(keyvalue->keyvalue);
            common_key.key_status = keyvalue->keystate;
            common_key.x = -1;
            common_key.y = -1;

            data_len = sizeof(common_key);
            BlcCore_memcpy(keybuf, &common_key, data_len);
        } break;
        case BlcTermKeyDevType_Keyboard: {
            common_key.dev_type = BlcStbTermKeyDevType_Keyboard;
            common_key.key = blc_key_encryption(keyvalue->keyvalue);
            common_key.key_status = keyvalue->keystate;
            common_key.x = -1;
            common_key.y = -1;

            data_len = sizeof(common_key);
            BlcCore_memcpy(keybuf, &common_key, data_len);
        } break;
        case BlcTermKeyDevType_Mouse: {
            common_key.dev_type = BlcStbTermKeyDevType_Mouse;
            common_key.key = blc_key_encryption(keyvalue->keyvalue);
            common_key.key_status = keyvalue->keystate;
            common_key.x = keyvalue->x;
            common_key.y = keyvalue->y;

            data_len = sizeof(common_key);
            BlcCore_memcpy(keybuf, &common_key, data_len);
        } break;
        case BlcTermKeyDevType_Console: {
            console_key.dev_type = BlcStbTermKeyDevType_Console;
            console_key.key = blc_key_encryption(keyvalue->keyvalue);
            console_key.key_status = keyvalue->keystate;
            console_key.x = keyvalue->x;
            console_key.y = keyvalue->y;
            BlcCore_memcpy(console_key.console_id, keyvalue->console_id,
                           sizeof(keyvalue->console_id));

            console_key.is_passback = 0;

            BlcCore_memcpy(console_key.console_keyvalue, keyvalue->console_key,
                           sizeof(keyvalue->console_key));

            data_len = sizeof(console_key);
            BlcCore_memcpy(keybuf, &console_key, data_len);
        } break;
        default: {
            BlcCore_printf("%s %d dev_type = %d \n", __FUNCTION__, __LINE__,
                           keyvalue->keydev);
            return -1;
        }
        }
    }

    return SendMsgToCloud(keygw_sock, BlcTermMsgType_SendKey, keybuf, data_len);
}

static int StblKeyDeal(BlcKeyValueData *keyvalue) {
    if (keyvalue == NULL) {
        BlcCore_printf("%s %d param error \n", __FUNCTION__, __LINE__);
        return -1;
    }
    BlcCore_printf("+++++++++++++++++++++++++++++ StblKeyDeal %d, %d, %d "
                   "+++++++++++++++++++++++++++++\n",
                   keyvalue->keydev, keyvalue->keyvalue, keyvalue->keystate);

    if (keyvalue->keydev == BlcTermKeyDevType_Irr) {
        BlcCore_printf("before key :%d\n", keyvalue->keyvalue);
        keyvalue->keyvalue =
                bbcv_plug_mwkeytobbcvkey(standardid, keyvalue->keyvalue);
        BlcCore_printf("after  key :%d\n", keyvalue->keyvalue);
        if (osdnotice_status == 1) {
            if (keyvalue->keyvalue == BlcIrrPropertyValue_SELECT &&
                    keyvalue->keystate == BlcTermKeyDevStat_Up) {
                porting_app_cmd(BBCV_PlugApi_CtrlID_OSDHide, NULL, 0);
                osdnotice_status = 0;
                BlcCore_printf("%s %d notice hide success....\n", __FUNCTION__,
                               __LINE__);
            }
            return 0;
        }
    } else if (keyvalue->keydev == BlcTermKeyDevType_Mouse) {
        if (mouse_in_flag == BlcKeyDevPlugType_MousePlugout) {
            BBCV_PlugApi_Param_MouseUIShow mouse_show;
            mouse_show.ui_type = mouse_ui_type;
            mouse_show.x = keyvalue->x;
            mouse_show.y = keyvalue->y;
            mouse_show.full_screen_width = full_screen_width;
            mouse_show.full_screen_higth = full_screen_higth;
            porting_app_cmd(BBCV_PlugApi_CtrlID_MouseUIShow, &mouse_show,
                            sizeof(mouse_show));

            mouse_in_flag = BlcKeyDevPlugType_MousePlugin;
            return 0;
        }

        if (keyvalue->keystate == BlcTermKeyDevStat_Default) {
            BBCV_PlugApi_Param_MouseUIShow mouse_show;
            mouse_show.ui_type = mouse_ui_type;
            mouse_show.x = keyvalue->x;
            mouse_show.y = keyvalue->y;
            mouse_show.full_screen_width = full_screen_width;
            mouse_show.full_screen_higth = full_screen_higth;
            porting_app_cmd(BBCV_PlugApi_CtrlID_MouseUIShow, &mouse_show,
                            sizeof(mouse_show));
        }
    }

    return BlcStb_KeySend(keyvalue);
}

static void BlcStb_WaitEventProcess(void *p) {
    int event_ret = -1;
    int event_id = -1;
    char event_value[4224];
    int event_value_size = -1;

    while (1) {
        BlcCore_memset(event_value, 0, sizeof(event_value));
        event_value_size = sizeof(event_value);

        event_ret = bbcv_plug_wait_events(&event_id, event_value, &event_value_size, -1);
        if (event_ret < 0) {
            BlcCore_printf("%s %d blcplug_wait_events error, event_ret = %d!\n",
                           __FUNCTION__, __LINE__, event_ret);
            continue;
        }

        if (event_id == BBCV_PlugApi_EventlID_KeyValue) {
            if (event_value_size < sizeof(BlcKeyValueData)) {
                BlcCore_printf("%s %d event_value_size is invalid ,send key fail!\n",
                               __FUNCTION__, __LINE__);
                continue;
            }
            BlcKeyValueData *keyvalue = (BlcKeyValueData *)event_value;
            StblKeyDeal(keyvalue);
        } else if (event_id == BBCV_PlugApi_EventlID_KeyPlugEvent) {
            if (event_value_size != sizeof(BlcKeyDevPlugStatus)) {
                BlcCore_printf("%s %d event_value_size is invalid!\n", __FUNCTION__,
                               __LINE__);
                continue;
            }

            BlcKeyDevPlugStatus *keydevstatus = (BlcKeyDevPlugStatus *)event_value;

            if (keydevstatus->plugstatus == BlcKeyDevPlugType_KeyboardPlugin) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_KeyboardPlugin event!\n",
                               __FUNCTION__, __LINE__);
            } else if (keydevstatus->plugstatus ==
                       BlcKeyDevPlugType_KeyboardPlugout) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_KeyboardPlugout event!\n",
                               __FUNCTION__, __LINE__);
            } else if (keydevstatus->plugstatus == BlcKeyDevPlugType_MousePlugin) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_MousePlugin event!\n",
                               __FUNCTION__, __LINE__);
                mouse_in_flag = BlcKeyDevPlugType_MousePlugin;
                if (demand_status == BlcDemandStatus_Sign) {
                    BBCV_PlugApi_Param_MouseUIShow mouse_show;
                    mouse_show.ui_type = mouse_ui_type;
                    mouse_show.x = 0;
                    mouse_show.y = 0;
                    mouse_show.full_screen_width = full_screen_width;
                    mouse_show.full_screen_higth = full_screen_higth;
                    porting_app_cmd(BBCV_PlugApi_CtrlID_MouseUIShow, &mouse_show,
                                    sizeof(mouse_show));
                }
            } else if (keydevstatus->plugstatus == BlcKeyDevPlugType_MousePlugout) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_MousePlugout event!\n",
                               __FUNCTION__, __LINE__);
                mouse_in_flag = BlcKeyDevPlugType_MousePlugout;
                if (demand_status == BlcDemandStatus_Sign) {
                    porting_app_cmd(BBCV_PlugApi_CtrlID_MouseUIHide, NULL, 0);
                }
            } else if (keydevstatus->plugstatus == BlcKeyDevPlugType_ConsolePlugin) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_ConsolePlugin event!\n",
                               __FUNCTION__, __LINE__);
            } else if (keydevstatus->plugstatus == BlcKeyDevPlugType_ConsolePlugout) {
                BlcCore_printf("%s %d BlcKeyDevPlugType_ConsolePlugout event!\n",
                               __FUNCTION__, __LINE__);
            } else {
                BlcCore_printf("%s %d Unknow BlcTermKeyDevPlug event %d!\n",
                               __FUNCTION__, __LINE__, keydevstatus->plugstatus);
            }
        } else if (event_id == BBCV_PlugApi_EventlID_Login) {
            BBCV_PlugApi_Param_Login *loginparam =
                    (BBCV_PlugApi_Param_Login *)event_value;
            BlcCore_printf(
                        "%s %d BBCV_PlugApi_EventlID_Login service_type = %d,url = %s!\n",
                        __FUNCTION__, __LINE__, loginparam->service_type, loginparam->url);

            if (demand_status == BlcDemandStatus_Sign) {
                if (ipanel_status == 0) {
                    BlcCore_printf("%s %d Warning it is already logined!\n", __FUNCTION__,
                                   __LINE__);
                    continue;
                } else {
                    BlcCore_printf("%s %d MwReturn.....!\n", __FUNCTION__, __LINE__);
                    StbStartLoginCloud(loginparam->url, BLCPLUG_RECV_TIMEOUT, 1);
                    continue;
                }
            } else if (demand_status == BlcDemandStatus_Signing) {
                BlcCore_printf("%s %d Warning it is logining!\n", __FUNCTION__,
                               __LINE__);
                continue;
            }

            BlcCore_printf("%s %d Start login.....!\n", __FUNCTION__, __LINE__);

            StbStartLoginCloud(loginparam->url, BLCPLUG_RECV_TIMEOUT, 0);
        } else if (event_id == BBCV_PlugApi_EventlID_Logout) {
            if (demand_status == BlcDemandStatus_Quit) {
                BlcCore_printf("%s %d Warning it is already quited!\n", __FUNCTION__,
                               __LINE__);
            }
            StbLogoutCloud(1);
        } else if (event_id == BBCV_PlugApi_EventlID_ConnectServer) {
            BlcCore_printf("%s %d TODO BBCV_PlugApi_EventlID_ConnectServer!\n",
                           __FUNCTION__, __LINE__);

            StbConnectServer();
        } else if (event_id == BBCV_PlugApi_EventlID_NoticePlayStatus) {
            BBCV_PlugApi_Param_NoticePlayStatus *play_statusparam =
                    (BBCV_PlugApi_Param_NoticePlayStatus *)event_value;
            // play_status = *play_statusparam;
            BlcCore_printf(
                        "%s %d BBCV_PlugApi_EventlID_NoticePlayStatus event wait_playstatus "
                        "= %d   play_statusparam = %d!\n",
                        __FUNCTION__, __LINE__, wait_playstatus,
                        play_statusparam->playstatus);
            if (wait_playstatus == 0) {
                if (play_statusparam->playstatus >= 0) {
                    BlcCore_printf(
                                "%s %d BBCV_PlugApi_EventlID_NoticePlayStatus reset status!\n",
                                __FUNCTION__, __LINE__);
                    BBCV_PlugApi_Param_ReturnMw param_returnmw;
                    param_returnmw.service_type = BlcReturnMwServiceType_Default;
                    param_returnmw.url = NULL;
                    porting_app_cmd(BBCV_PlugApi_CtrlID_ReturnMw, &param_returnmw,
                                    sizeof(param_returnmw));
                }
            } else if (wait_playstatus == 1 || wait_playstatus == 2) {
                play_status = *play_statusparam;
                wait_playstatus = 2;
            }

        } else {
            BlcCore_printf("%s %d unknow event_id = %d!\n", __FUNCTION__, __LINE__,
                           event_id);
        }
    }
}

static void BlcStb_ChangeOSDStatus(void *p) {
    int wait_total_time = 0;
    int temp_time = 0;

    while (1) {
        wait_total_time = osd_timeout;
        temp_time = 0;
        while (osdnotice_status == 1) {
            if (temp_time < wait_total_time) {
                BlcCore_ThreadDelay(20);
                temp_time += 20;

            } else
                osdnotice_status = 0;
        }

        BlcCore_ThreadDelay(10);
        continue;
    }
}

static void BlcStb_WaitEventProcess_ts(void *p) { return; }

//功能:机顶盒注册心跳线程
static void BlcStb_KeepAliveProcess(void *p) {
    char login_str[4096];
    char region_id_str[10];
    char net_type_str[10];

    BlcCore_sprintf(region_id_str, "%d", region_id);
    BlcCore_sprintf(net_type_str, "%d", net_type);

    StbCjsonCreate(login_str, "cmd", "login", "logintype", "1", "plugversion",
                   "10900", "portalid", "0", "regionid", region_id_str,
                   "terminfo", term_info, "termnet", net_type_str, "termtype",
                   "1001", "url", "", "aurl", "", "expandinfo", "", "end");

    //创建接入网关套接字
    if ((access_sock = BlcCore_SocketCreate(BLCNET_SOCK_DGRAM)) == -1) {
        BlcCore_printf("%s %d creat access_sock socket error !!!!!!!!\n",
                       __FUNCTION__, __LINE__);
        return;
    }

    while (1) {
        if (connection_status != 0 || auto_reconnect_mode != 1) {
            BlcCore_ThreadDelay(100);
            continue;
        }

        if (GetCloudAddr(&keygw_addr, BLCPLUG_RECV_TIMEOUT) < 0) {
            BlcCore_printf("%s %d GetCloudAddr error \n", __FUNCTION__, __LINE__);
            BlcCore_ThreadDelay(6000);
            continue;
        }

        if (CreateTCPConnection(&keygw_sock, &keygw_addr) < 0) {
            BlcCore_printf("%s %d TCP connect intergw error \n", __FUNCTION__,
                           __LINE__);
            BlcCore_ThreadDelay(6000);
            continue;
        }

        if (demand_status == BlcDemandStatus_Sign) {
            if (SendMsgToCloud(keygw_sock, BlcTermMsgType_Login, login_str,
                               BlcCore_strlen(login_str)) < 0) {
                BlcCore_printf("%s %d TCP Send login msg to intergw error \n",
                               __FUNCTION__, __LINE__);
                BlcCore_ThreadDelay(6000);
                continue;
            }
        } else if (is_continue_connect == 1) {
            if (is_first_connect)
                ; //               msg.connect_flag = 0;
            else
                ; //            msg.connect_flag = 1;

            if (SendMsgToCloud(keygw_sock, BlcTermMsgType_KeepAlive, NULL, 0) < 0) {
                BlcCore_printf("%s %d TCP Send register msg to intergw error \n",
                               __FUNCTION__, __LINE__);
                BlcCore_ThreadDelay(6000);
                continue;
            }
        }

        connection_status = 1;
    }
}

int bbcv_plug_start() {
    BlcCore_printf("%s %d **********bbcv_plug_start**********[in]!\n",
                   __FUNCTION__, __LINE__);

    if (StbPropertyUpdate() < 0) {
        BlcCore_printf("%s %d Get StbProperty fail!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int arg[8];
    BlcCore_memset(thread_id, 0, sizeof(thread_id));

    if (0 != BlcCore_ThreadCreate(&thread_id[0], BlcStb_WaitEventProcess, &arg[0],
                                  128 * 1024, BLCTHREAD_DEFAULT_PRIORITY)) {
        BlcCore_printf("%s %d BlcCore_ThreadCreate BlcStb_WaitEventProcess fail!\n",
                       __FUNCTION__, __LINE__);
        return -2;
    }
    if (0 != BlcCore_ThreadCreate(&thread_id[1], BlcStb_RecvIPOrderProcess,
                                  &arg[1], 128 * 1024,
                                  BLCTHREAD_DEFAULT_PRIORITY)) {
        BlcCore_printf(
                    "%s %d BlcCore_ThreadCreate BlcStb_RecvIPOrderProcess fail!\n",
                    __FUNCTION__, __LINE__);
        return -3;
    }
    if (0 != BlcCore_ThreadCreate(&thread_id[2], BlcStb_KeepAliveProcess, &arg[2],
                                  128 * 1024, BLCTHREAD_DEFAULT_PRIORITY)) {
        BlcCore_printf("%s %d BlcCore_ThreadCreate BlcStb_ReconnectProcess fail!\n",
                       __FUNCTION__, __LINE__);
        return -3;
    }
    if (0 != BlcCore_ThreadCreate(&thread_id[3], BlcStb_WaitEventProcess_ts,
                                  &arg[3], 128 * 1024,
                                  BLCTHREAD_DEFAULT_PRIORITY)) {
        BlcCore_printf(
                    "%s %d BlcCore_ThreadCreate BlcStb_WaitEventProcess_ts fail!\n",
                    __FUNCTION__, __LINE__);
        return -4;
    }
    if (0 != BlcCore_ThreadCreate(&thread_id[4], BlcStb_ChangeOSDStatus, &arg[4],
                                  128 * 1024, BLCTHREAD_DEFAULT_PRIORITY)) {
        BlcCore_printf(
                    "%s %d BlcCore_ThreadCreate BlcStb_WaitEventProcess_ts fail!\n",
                    __FUNCTION__, __LINE__);
        return -5;
    }

    BlcCore_printf("%s %d ********** bbcv_plug_start **********[out]!\n",
                   __FUNCTION__, __LINE__);

    return 0;
}

int bbcv_plug_stop() {
    BlcCore_printf("%s %d ********** bbcv_plug_stop **********!\n", __FUNCTION__,
                   __LINE__);

    if (thread_id[0])
        BlcCore_ThreadDelete(thread_id[0]);

    if (thread_id[1])
        BlcCore_ThreadDelete(thread_id[1]);

    if (thread_id[2])
        BlcCore_ThreadDelete(thread_id[2]);

    return 0;
}
