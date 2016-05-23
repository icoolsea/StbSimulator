/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "../../include/blccore.h"
#include <string.h>
//#include <stdio.h>

typedef struct {
  int sem_type;
  int data_len;
  char data[5000];
} SemData;

extern void BlcCore_printf(const char *string, ...);
extern int stb_status;

extern char term_id[32];
extern int region_id_;
extern int net_type;
extern int key_standard_id;
extern char gw_ip[32];
extern char stb_soft_version[128];
extern char stb_hw_version[128];
extern char stb_mw_version[128];

extern int all_sem;
extern SemData semdata;

extern int ts_sem;
extern SemData semtsdata;

extern int plug_sem;
extern SemData plugsemdata;

int porting_sys_param_get(const char *key_name, void *key_value,
                          int *key_value_size) {
    if (strcmp(key_name, "stb_id") == 0) {
        strncpy((char *)key_value, term_id, *key_value_size);
        *key_value_size = strlen(term_id);
        return 0;
    } else if (strcmp(key_name, "region_id") == 0) {
        snprintf((char *)key_value, *key_value_size, "%d", region_id_);
        *key_value_size = strlen((char *)key_value);
        return 0;
    } else if (strcmp(key_name, "key_standard_id") == 0) {

        snprintf((char *)key_value, *key_value_size, "%d", key_standard_id);
        *key_value_size = strlen((char *)key_value);
        return 0;
    } else if (strcmp(key_name, "net_type") == 0) {
        snprintf((char *)key_value, *key_value_size, "%d", net_type);
        *key_value_size = strlen((char *)key_value);
        return 0;
    } else if (strcmp(key_name, "cloud_addr") == 0) {
        strncpy((char *)key_value, gw_ip, *key_value_size);
        *key_value_size = strlen(gw_ip);
        return 0;
    } else if (strcmp(key_name, "stb_soft_version") == 0) {
        strncpy((char *)key_value, stb_soft_version, *key_value_size);
        *key_value_size = strlen(stb_soft_version);
        return 0;
    } else if (strcmp(key_name, "stb_hw_version") == 0) {
        strncpy((char *)key_value, stb_hw_version, *key_value_size);
        *key_value_size = strlen(stb_hw_version);
        return 0;
    } else if (strcmp(key_name, "stb_mw_version") == 0) {
        strncpy((char *)key_value, stb_mw_version, *key_value_size);
        *key_value_size = strlen(stb_mw_version);
        return 0;
    }
    return -1;
}

int porting_sys_param_set(const char *key_name, const void *key_value,
                          int key_value_size) {
    return -1;
}

int porting_app_cmd(int ctrl_id, void *value, int value_size) {
    switch (ctrl_id) {
    case BBCV_PlugApi_CtrlID_PlayUnBlock_S2:
        //	TODO Play 根据频点和service_id 切台。
        stb_status = 2;
        BBCV_PlugApi_Param_ServiceID2 serid2 =
                *(BBCV_PlugApi_Param_ServiceID2 *)value;
        BlcCore_printf("Freq:%d,ServiceID:%d,Symbol:%d,Mode:%d,Audio_format:%d,"
                       "Video_format:%d,PlayUnBlock success switch in cloud.\n",
                       serid2.freq, serid2.service_id, serid2.symbol, serid2.mode,
                       serid2.audio_format, serid2.video_format);

    {
        BBCV_PlugApi_Param_NoticePlayStatus play_status;
        play_status.playstatus = 1;

        memset(&semdata, 0, sizeof(semdata));
        semdata.sem_type = BBCV_PlugApi_EventlID_NoticePlayStatus;
        semdata.data_len = sizeof(play_status);
        memcpy(semdata.data, &play_status, semdata.data_len);

        BlcCore_SemPost(all_sem);
    }
    {
        char buf[1024] = {0};
        BlcCore_sprintf(buf, "Freq:%d,ServiceID:%d,Symbol:%d,Mode:%d,Audio_"
                        "format:%d,Video_format:%d",
                        serid2.freq, serid2.service_id, serid2.symbol,
                        serid2.mode, serid2.audio_format, serid2.video_format);
        memset(&plugsemdata, 0, sizeof(plugsemdata));
        plugsemdata.sem_type = BBCV_PlugApi_CtrlID_PlayUnBlock_S2;
        plugsemdata.data_len = BlcCore_strlen(buf);
        memcpy(plugsemdata.data, buf, plugsemdata.data_len);

        BlcCore_SemPost(plug_sem);
    }

        break;
    case BBCV_PlugApi_CtrlID_PlayUnBlock_P:
        //	TODO Play 根据频点和PMT_PID切台。
        stb_status = 2;
        BBCV_PlugApi_Param_PMT_PID pmtpid = *(BBCV_PlugApi_Param_PMT_PID *)value;
        BlcCore_printf("Freq:%d,ServiceID:%d,Symbol:%d,Mode:%d,Audio_format:%d,"
                       "Video_format:%d,PlayUnBlock success switch in cloud.\n",
                       pmtpid.freq, pmtpid.pmt_pid, pmtpid.symbol, pmtpid.mode,
                       pmtpid.audio_format, pmtpid.video_format);

    {
        BBCV_PlugApi_Param_NoticePlayStatus play_status;
        play_status.playstatus = 1;

        memset(&semdata, 0, sizeof(semdata));
        semdata.sem_type = BBCV_PlugApi_EventlID_NoticePlayStatus;
        semdata.data_len = sizeof(play_status);
        memcpy(semdata.data, &play_status, semdata.data_len);

        BlcCore_SemPost(all_sem);
    }

    {
        char buf[1024] = {0};
        BlcCore_sprintf(buf, "Freq:%d,ServiceID:%d,Symbol:%d,Mode:%d,Audio_"
                        "format:%d,Video_format:%d",
                        pmtpid.freq, pmtpid.pmt_pid, pmtpid.symbol, pmtpid.mode,
                        pmtpid.audio_format, pmtpid.video_format);
        memset(&plugsemdata, 0, sizeof(plugsemdata));
        plugsemdata.sem_type = BBCV_PlugApi_CtrlID_PlayUnBlock_P;
        plugsemdata.data_len = BlcCore_strlen(buf);
        memcpy(plugsemdata.data, buf, plugsemdata.data_len);

        BlcCore_SemPost(plug_sem);
    }
        break;
    case BBCV_PlugApi_CtrlID_ReturnMw:
        //	TODO Play 根据URL切台到中间件。
        stb_status = 1;
        BBCV_PlugApi_Param_ReturnMw parammw = *(BBCV_PlugApi_Param_ReturnMw *)value;
        BlcCore_printf(
                    "Service_type:[%d],Url:[%s],PlayUnBlock success switch in midware.\n",
                    parammw.service_type, parammw.url);
        if (parammw.service_type == BlcReturnMwServiceType_Browser)
            stb_status = 1;
        else
            stb_status = 0;

    {
        char buf[1024] = {0};
        if (stb_status == 1)
            BlcCore_sprintf(buf, "Service_type:[%d],Url:[%s]", parammw.service_type,
                            parammw.url);
        else
            BlcCore_sprintf(buf, "%s", "fail");

        memset(&plugsemdata, 0, sizeof(plugsemdata));
        plugsemdata.sem_type = BBCV_PlugApi_CtrlID_ReturnMw;
        plugsemdata.data_len = BlcCore_strlen(buf);
        memcpy(plugsemdata.data, buf, plugsemdata.data_len);

        BlcCore_SemPost(plug_sem);
    }

        break;

    case BBCV_PlugApi_CtrlID_OSDDisplay:
        //	TODO Play 根据property缓存中的字符串显示size长度的字符串
    {
        BBCV_PlugApi_Param_OSDDisplay *notice =
                (BBCV_PlugApi_Param_OSDDisplay *)value;

        BlcCore_printf(
                    "OSD Notice,dialog_style:%d|title:%s|text:%s|time_out:%d \n",
                    notice->dialog_style, notice->title, notice->text, notice->time_out);

        char buf[1024] = {0};
        BlcCore_sprintf(
                    buf, "OSD Notice, dialog_style:%d|title:%s|text:%s|time_out:%d",
                    notice->dialog_style, notice->title, notice->text, notice->time_out);
        memset(&plugsemdata, 0, sizeof(plugsemdata));
        plugsemdata.sem_type = BBCV_PlugApi_CtrlID_OSDDisplay;
        plugsemdata.data_len = BlcCore_strlen(buf);
        memcpy(plugsemdata.data, buf, plugsemdata.data_len);

        BlcCore_SemPost(plug_sem);
    }

        break;
    case BBCV_PlugApi_CtrlID_OSDHide:
        //	TODO Play 立即清除刚刚的OSD显示的内容
        BlcCore_printf("BBCV_PlugApi_CtrlID_OSDHide.....\n");

    {
        char buf[1024] = {0};
        BlcCore_sprintf(buf, "BBCV_PlugApi_CtrlID_OSDHide");
        memset(&plugsemdata, 0, sizeof(plugsemdata));
        plugsemdata.sem_type = BBCV_PlugApi_CtrlID_OSDHide;
        plugsemdata.data_len = BlcCore_strlen(buf);
        memcpy(plugsemdata.data, buf, plugsemdata.data_len);

        BlcCore_SemPost(plug_sem);
    }

        break;

    case BBCV_PlugApi_CtrlID_MouseUIShow:
        BlcCore_printf("BBCV_PlugApi_CtrlID_MouseUIShow.....\n");
        break;
    case BBCV_PlugApi_CtrlID_MouseUIHide:
        BlcCore_printf("BBCV_PlugApi_CtrlID_MouseUIHide.....\n");
        break;
    case BBCV_PlugApi_CtrlID_KeyDown:
        BlcCore_printf("BBCV_PlugApi_CtrlID_KeyDown.....\n");
        break;
    default:
        return -2;
    }

    return 0;
}

int bbcv_plug_wait_events(int *event_id, void *value, int *value_size,
                          int timeout_us) {
    if (event_id == NULL || value == NULL || value_size == NULL)
        return -1;

    if (timeout_us == -1) {
        BlcCore_SemWait(all_sem);
        // todo
        *event_id = semdata.sem_type;
        memcpy(value, semdata.data, *value_size);
        *value_size = semdata.data_len;

        return 0;
    } else {
        int ret = -2;

        ret = BlcCore_SemWaitTime(all_sem, timeout_us);
        // todo

        *event_id = semdata.sem_type;
        memcpy(value, semdata.data, *value_size);
        *value_size = semdata.data_len;

        if (ret == 0)
            return 0;
        else
            return -1;
    }

    return -2;
}

int bbcv_plug_wait_events_ts(int *event_id, void *value, int *value_size,
                             int timeout_us) {
    if (event_id == NULL || value == NULL || value_size == NULL)
        return -1;

    if (timeout_us == -1) {
        BlcCore_SemWait(ts_sem);
        // todo
        *event_id = semtsdata.sem_type;
        memcpy(value, semtsdata.data, *value_size);
        *value_size = semtsdata.data_len;

        return 0;
    } else {
        int ret = -2;

        ret = BlcCore_SemWaitTime(ts_sem, timeout_us);
        // todo

        *event_id = semtsdata.sem_type;
        memcpy(value, semtsdata.data, *value_size);
        *value_size = semtsdata.data_len;

        if (ret == 0)
            return 0;
        else
            return -1;
    }

    return -2;
}

int bbcv_plug_wait_events_plug(int *event_id, void *value, int *value_size,
                               int timeout_us) {
    if (event_id == NULL || value == NULL || value_size == NULL)
        return -1;

    if (timeout_us == -1) {
        BlcCore_SemWait(plug_sem);
        // todo
        *event_id = plugsemdata.sem_type;
        memcpy(value, plugsemdata.data, *value_size);
        *value_size = plugsemdata.data_len;

        return 0;
    } else {
        int ret = -2;

        ret = BlcCore_SemWaitTime(plug_sem, timeout_us);
        // todo

        *event_id = plugsemdata.sem_type;
        memcpy(value, plugsemdata.data, *value_size);
        *value_size = plugsemdata.data_len;

        if (ret == 0)
            return 0;
        else
            return -1;
    }

    return -2;
}
