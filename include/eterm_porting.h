#ifndef _ETERM_PORTING_H
#define _ETERM_PORTING_H

#include "blccore.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

typedef struct {
    int sem_type;
    int data_len;
    char data[5000];
} SemData;

int stb_status; // 0是直播;1是中间件;2是云状态

char gw_ip[32];
;
char term_id[32];
int region_id_;

int net_type;
int key_standard_id;
char stb_soft_version[128];
char stb_hw_version[128];
char stb_mw_version[128];

int all_sem;
SemData semdata;

int plug_sem;
SemData plugsemdata;

int ts_sem;
SemData semtsdata;

BlcKeyValueData key_value;
BlcKeyDevPlugStatus keydevstatus;

int init();
int config_info(const char *finename);
void main_handel(void);
void BlcCore_printf(const char *string, ...);

int plug_vod_test(void);

int blcplug_video_meeting_set_property(int property_id, void *property,
                                       int size);
int blcplug_video_meeting_get_property(int property_id, void *property,
                                       int size);
int blcplug_2dimensional_barcode_set_property(int property_id, void *property,
                                              int size);
int blcplug_other_bs_cmd_set_property(int property_id, void *property,
                                      int size);
void *recvtsdata_process(void *arg);

#endif
