#ifndef _PLUG_IPANELKEY_H_
#define _PLUG_IPANELKEY_H_

typedef enum __keyValues {
    IPANEL_CLOUD_IRKEY_BACK = 0x08,          //后退
    IPANEL_CLOUD_IRKEY_EXIT = 0x1b,          //退出
    IPANEL_CLOUD_IRKEY_PAGE_UP = 0x21,       //向前翻页
    IPANEL_CLOUD_IRKEY_PAGE_DOWN = 0x22,     //向后翻页
    IPANEL_CLOUD_IRKEY_LEFT = 0x25,          //箭头左
    IPANEL_CLOUD_IRKEY_UP = 0x26,            //箭头上
    IPANEL_CLOUD_IRKEY_RIGHT = 0x27,         //箭头右
    IPANEL_CLOUD_IRKEY_DOWN = 0x28,          //箭头下
    IPANEL_CLOUD_IRKEY_SELECT = 0x0d,        //确定
    IPANEL_CLOUD_IRKEY_NUM0 = 0x30,          //数字键0
    IPANEL_CLOUD_IRKEY_NUM1,                 //数字键1
    IPANEL_CLOUD_IRKEY_NUM2,                 //数字键2
    IPANEL_CLOUD_IRKEY_NUM3,                 //数字键3
    IPANEL_CLOUD_IRKEY_NUM4,                 //数字键4
    IPANEL_CLOUD_IRKEY_NUM5,                 //数字键5
    IPANEL_CLOUD_IRKEY_NUM6,                 //数字键6
    IPANEL_CLOUD_IRKEY_NUM7,                 //数字键7
    IPANEL_CLOUD_IRKEY_NUM8,                 //数字键8
    IPANEL_CLOUD_IRKEY_NUM9,                 //数字键9
    IPANEL_CLOUD_IRKEY_VOLUME_UP = 0x100d,   /* 音量+ */
    IPANEL_CLOUD_IRKEY_VOLUME_DOWN = 0x100e, /* 音量- */
    IPANEL_CLOUD_IRKEY_RED = 0x901,          //功能键1
    IPANEL_CLOUD_IRKEY_GREEN,                //功能键2
    IPANEL_CLOUD_IRKEY_YELLOW,               //功能键3

    // IPANEL_CLOUD_IRKEY_MENU                 = 0x1001, //菜单
    IPANEL_CLOUD_IRKEY_INFO = 0x1004,      //信息
    IPANEL_CLOUD_IRKEY_FAVORITE = 0x1006,  //收藏
    IPANEL_CLOUD_IRKEY_LANGUAGE = 0x1007,  //语言切换
    IPANEL_CLOUD_IRKEY_RECOMMEND = 0x1062, //推荐
    IPANEL_CLOUD_IRKEY_FORETELL = 0x1086,  //预告
    IPANEL_CLOUD_IRKEY_SWITCH = 0x1087     //切换
    // IPANEL_CLOUD_IRKEY_TV_SYSTEM            = 0x1071  //切换
} keyValues;

#endif
