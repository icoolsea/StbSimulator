#ifndef _PLUG_PLUGKEYMAP_H_
#define _PLUG_PLUGKEYMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BlcKeyDevPlugType_KeyboardPlugin = 1,
    BlcKeyDevPlugType_KeyboardPlugout = 2,
    BlcKeyDevPlugType_MousePlugin = 3,
    BlcKeyDevPlugType_MousePlugout = 4,
    BlcKeyDevPlugType_ConsolePlugin = 5,
    BlcKeyDevPlugType_ConsolePlugout = 6,
} BlcKeyDevPlugType;

typedef struct {
    int plugstatus; //键值设备的插拔的值，取值是BlcKeyDevPlugType

} BlcKeyDevPlugStatus;

typedef struct {
    int keydev;   //键值设备类型(红外、键盘、鼠标、手柄使用)
    int keystate; // 键值状态(红外、键盘、鼠标、手柄使用)
    int keyvalue; // 键值(红外、键盘、鼠标使用)
    short x;      //鼠标的x坐标(鼠标使用)
    short y;      //鼠标的y坐标(鼠标使用)
    char console_id[8];   //游戏手柄的标志id(手柄使用)
    char console_key[50]; //游戏手柄的键值(手柄使用)
} BlcKeyValueData;

typedef enum { //标准的标识，例如ipanel、wasu等
    BlcTermStandardID_Ipanel = 1,
    BlcTermStandardID_Wasu = 2,
} BlcTermStandardID;

typedef enum {
    BlcTermKeyDevType_Irr = 1,
    BlcTermKeyDevType_Keyboard = 2,
    BlcTermKeyDevType_Mouse = 3,
    BlcTermKeyDevType_Console = 4,
} BlcTermKeyDevType;

typedef enum {
    BlcTermKeyDevStat_Default = 1, // 所有键值的默认状态或鼠标的坐标移动状态
    BlcTermKeyDevStat_Down = 2,       // 所有键值的按下状态
    BlcTermKeyDevStat_Up = 3,         // 所有键值的弹起状态
    BlcTermKeyDevStat_MouseWheel = 4, // 鼠标滚轮转动
} BlcTermKeyDevStat;

typedef enum {
    BlcIrrPropertyValue_DIGIT1 = 0x11,      /*数字键1*/
    BlcIrrPropertyValue_DIGIT2 = 0x12,      /*数字键2*/
    BlcIrrPropertyValue_DIGIT3 = 0x13,      /*数字键3*/
    BlcIrrPropertyValue_DIGIT4 = 0x14,      /*数字键4*/
    BlcIrrPropertyValue_DIGIT5 = 0x15,      /*数字键5*/
    BlcIrrPropertyValue_DIGIT6 = 0x16,      /*数字键6*/
    BlcIrrPropertyValue_DIGIT7 = 0x17,      /*数字键7*/
    BlcIrrPropertyValue_DIGIT8 = 0x18,      /*数字键8*/
    BlcIrrPropertyValue_DIGIT9 = 0x19,      /*数字键9*/
    BlcIrrPropertyValue_DIGIT0 = 0x10,      /*数字键0*/
    BlcIrrPropertyValue_LANGUAGE = 0x1a,    /*语言切换*/
    BlcIrrPropertyValue_COLLECTION = 0x1b,  /*收藏*/
    BlcIrrPropertyValue_SWITCH = 0x05,      /*切换*/
    BlcIrrPropertyValue_MENU = 0x07,        /*菜单*/
    BlcIrrPropertyValue_ARROWUP = 0x00,     /*箭头上*/
    BlcIrrPropertyValue_ARROWDOWN = 0x01,   /*箭头下*/
    BlcIrrPropertyValue_ARROWLEFT = 0x03,   /*箭头左*/
    BlcIrrPropertyValue_ARROWRIGHT = 0x02,  /*箭头右*/
    BlcIrrPropertyValue_SELECT = 0x1f,      /*确定*/
    BlcIrrPropertyValue_BACK = 0x1c,        /*后退*/
    BlcIrrPropertyValue_EXIT = 0x1d,        /*退出*/
    BlcIrrPropertyValue_PREVUE = 0x09,      /*预告*/
    BlcIrrPropertyValue_PAGEDOWN = 0x08,    /*向前翻页，快进*/
    BlcIrrPropertyValue_PAGEUP = 0x04,      /*向后翻页，快退*/
    BlcIrrPropertyValue_TRACK = 0x0f,       /*声道*/
    BlcIrrPropertyValue_MESSAGE = 0x40,     /*信息*/
    BlcIrrPropertyValue_F1 = 0x0e,          /*功能键1*/
    BlcIrrPropertyValue_F2 = 0x1e,          /*功能键2*/
    BlcIrrPropertyValue_COMMEND = 0x4d,     /*推荐*/
    BlcIrrPropertyValue_F3 = 0x4c,          /*功能键3*/
    BlcIrrPropertyValue_F4 = 0x44,          /*功能键4*/
    BlcIrrPropertyValue_MENU_LONG = 0xfe,   /*菜单长按5秒*/
    BlcIrrPropertyValue_RADIO = 0x3d,       /*电台*/
    BlcIrrPropertyValue_SEARCH = 0x3b,      /*搜索*/
    BlcIrrPropertyValue_PRE_UP = 0xf0,      /*前面板上*/
    BlcIrrPropertyValue_PRE_DOWN = 0xf1,    /*前面板下*/
    BlcIrrPropertyValue_PRE_LEFT = 0xf2,    /*前面板左*/
    BlcIrrPropertyValue_PRE_RIGHT = 0xf3,   /*前面板右*/
    BlcIrrPropertyValue_PRE_OK = 0xf4,      /*前面板选择*/
    BlcIrrPropertyValue_PRE_POWER = 0xf5,   /*前面板电源*/
    BlcIrrPropertyValue_PRE_EXIT = 0xf6,    /*前面板退出*/
    BlcIrrPropertyValue_PRE_MENU = 0xf7,    /*前面板菜单*/
    BlcIrrPropertyValue_VOLUME_UP = 0xf8,   /*新增音量加*/
    BlcIrrPropertyValue_VOLUME_DOWN = 0xf9, /*新增音量减*/
    // 0x100
    // 0x1ff
} BlcIrrKeyValue;

typedef enum {
    BlcKeyboardPropertyValue_BACKSPACE = 0x08,
    BlcKeyboardPropertyValue_TAB = 0x09,
    BlcKeyboardPropertyValue_CLEAR = 0x0c,
    BlcKeyboardPropertyValue_ENTER = 0x0d,
    BlcKeyboardPropertyValue_SHIFT = 0x10,
    BlcKeyboardPropertyValue_CTRL = 0x11,
    BlcKeyboardPropertyValue_ALT = 0x12,
    BlcKeyboardPropertyValue_PAUSE = 0x13,
    BlcKeyboardPropertyValue_CAPSLOC = 0x14,
    BlcKeyboardPropertyValue_ESC = 0x1b,
    BlcKeyboardPropertyValue_SPACEBAR = 0x20,
    BlcKeyboardPropertyValue_PAGEUP = 0x21,
    BlcKeyboardPropertyValue_PAGEDOWN = 0x22,
    BlcKeyboardPropertyValue_END = 0x23,
    BlcKeyboardPropertyValue_HOME = 0x24,
    BlcKeyboardPropertyValue_LEFTARROW = 0x25,
    BlcKeyboardPropertyValue_UPARROW = 0x26,
    BlcKeyboardPropertyValue_RIGHTARROW = 0x27,
    BlcKeyboardPropertyValue_DOWNARROW = 0x28,
    BlcKeyboardPropertyValue_SELECT = 0x29,
    BlcKeyboardPropertyValue_PRINT = 0x2a,
    BlcKeyboardPropertyValue_EXECUTE = 0x2b,
    BlcKeyboardPropertyValue_PRINTSCREEN = 0x2c,
    BlcKeyboardPropertyValue_INS = 0x2d,
    BlcKeyboardPropertyValue_DEL = 0x2e,
    BlcKeyboardPropertyValue_HELP = 0x2f,
    BlcKeyboardPropertyValue_0 = 0x30,
    BlcKeyboardPropertyValue_1 = 0x31,
    BlcKeyboardPropertyValue_2 = 0x32,
    BlcKeyboardPropertyValue_3 = 0x33,
    BlcKeyboardPropertyValue_4 = 0x34,
    BlcKeyboardPropertyValue_5 = 0x35,
    BlcKeyboardPropertyValue_6 = 0x36,
    BlcKeyboardPropertyValue_7 = 0x37,
    BlcKeyboardPropertyValue_8 = 0x38,
    BlcKeyboardPropertyValue_9 = 0x39,
    BlcKeyboardPropertyValue_A = 0x41,
    BlcKeyboardPropertyValue_B = 0x42,
    BlcKeyboardPropertyValue_C = 0x43,
    BlcKeyboardPropertyValue_D = 0x44,
    BlcKeyboardPropertyValue_E = 0x45,
    BlcKeyboardPropertyValue_F = 0x46,
    BlcKeyboardPropertyValue_G = 0x47,
    BlcKeyboardPropertyValue_H = 0x48,
    BlcKeyboardPropertyValue_I = 0x49,
    BlcKeyboardPropertyValue_J = 0x4a,
    BlcKeyboardPropertyValue_K = 0x4b,
    BlcKeyboardPropertyValue_L = 0x4c,
    BlcKeyboardPropertyValue_M = 0x4d,
    BlcKeyboardPropertyValue_N = 0x4e,
    BlcKeyboardPropertyValue_O = 0x4f,
    BlcKeyboardPropertyValue_P = 0x50,
    BlcKeyboardPropertyValue_Q = 0x51,
    BlcKeyboardPropertyValue_R = 0x52,
    BlcKeyboardPropertyValue_S = 0x53,
    BlcKeyboardPropertyValue_T = 0x54,
    BlcKeyboardPropertyValue_U = 0x55,
    BlcKeyboardPropertyValue_V = 0x56,
    BlcKeyboardPropertyValue_W = 0x57,
    BlcKeyboardPropertyValue_X = 0x58,
    BlcKeyboardPropertyValue_Y = 0x59,
    BlcKeyboardPropertyValue_Z = 0x5a,
    BlcKeyboardPropertyValue_LEFTWIN = 0x5b,
    BlcKeyboardPropertyValue_RIGHTWIN = 0x5c,
    BlcKeyboardPropertyValue_APPLICATION = 0x5d,
    BlcKeyboardPropertyValue_COMPUTERSLEEP = 0x5f,
    BlcKeyboardPropertyValue_NUMERICKEYPAD0 = 0x60,
    BlcKeyboardPropertyValue_NUMERICKEYPAD1 = 0x61,
    BlcKeyboardPropertyValue_NUMERICKEYPAD2 = 0x62,
    BlcKeyboardPropertyValue_NUMERICKEYPAD3 = 0x63,
    BlcKeyboardPropertyValue_NUMERICKEYPAD4 = 0x64,
    BlcKeyboardPropertyValue_NUMERICKEYPAD5 = 0x65,
    BlcKeyboardPropertyValue_NUMERICKEYPAD6 = 0x66,
    BlcKeyboardPropertyValue_NUMERICKEYPAD7 = 0x67,
    BlcKeyboardPropertyValue_NUMERICKEYPAD8 = 0x68,
    BlcKeyboardPropertyValue_NUMERICKEYPAD9 = 0x69,
    BlcKeyboardPropertyValue_MULTIPLY = 0x6a,
    BlcKeyboardPropertyValue_ADD = 0x6b,
    BlcKeyboardPropertyValue_SEPARATOR = 0x6c,
    BlcKeyboardPropertyValue_SUBTRACT = 0x6d,
    BlcKeyboardPropertyValue_DECIMAL = 0x6e,
    BlcKeyboardPropertyValue_DIVIDE = 0x6f,
    BlcKeyboardPropertyValue_F1 = 0x70,
    BlcKeyboardPropertyValue_F2 = 0x71,
    BlcKeyboardPropertyValue_F3 = 0x72,
    BlcKeyboardPropertyValue_F4 = 0x73,
    BlcKeyboardPropertyValue_F5 = 0x74,
    BlcKeyboardPropertyValue_F6 = 0x75,
    BlcKeyboardPropertyValue_F7 = 0x76,
    BlcKeyboardPropertyValue_F8 = 0x77,
    BlcKeyboardPropertyValue_F9 = 0x78,
    BlcKeyboardPropertyValue_F10 = 0x79,
    BlcKeyboardPropertyValue_F11 = 0x7a,
    BlcKeyboardPropertyValue_F12 = 0x7b,
    BlcKeyboardPropertyValue_F13 = 0x7c,
    BlcKeyboardPropertyValue_F14 = 0x7d,
    BlcKeyboardPropertyValue_F15 = 0x7e,
    BlcKeyboardPropertyValue_F16 = 0x7f,
    BlcKeyboardPropertyValue_F17 = 0x80,
    BlcKeyboardPropertyValue_F18 = 0x81,
    BlcKeyboardPropertyValue_F19 = 0x82,
    BlcKeyboardPropertyValue_F20 = 0x83,
    BlcKeyboardPropertyValue_F21 = 0x84,
    BlcKeyboardPropertyValue_F22 = 0x85,
    BlcKeyboardPropertyValue_F23 = 0x86,
    BlcKeyboardPropertyValue_F24 = 0x87,
    BlcKeyboardPropertyValue_NUMLOCK = 0x90,
    BlcKeyboardPropertyValue_SCROLLLOCK = 0x91,
    BlcKeyboardPropertyValue_LEFTSHIFT = 0xa0,
    BlcKeyboardPropertyValue_RIGHTSHIFT = 0xa1,
    BlcKeyboardPropertyValue_LEFTCONTROL = 0xa2,
    BlcKeyboardPropertyValue_RIGHTCONTROL = 0xa3,
    BlcKeyboardPropertyValue_LEFTMENU = 0xa4,
    BlcKeyboardPropertyValue_RIGHTMENU = 0xa5,
    BlcKeyboardPropertyValue_OEM_1 = 0xba,
    BlcKeyboardPropertyValue_OEM_PLUS = 0xbb,
    BlcKeyboardPropertyValue_OEM_COMMA = 0xbc,
    BlcKeyboardPropertyValue_OEM_MINUS = 0xbd,
    BlcKeyboardPropertyValue_OEM_PERIOD = 0xbe,
    BlcKeyboardPropertyValue_OEM_2 = 0xbf,
    BlcKeyboardPropertyValue_OEM_3 = 0xc0,
    BlcKeyboardPropertyValue_OEM_4 = 0xdb,
    BlcKeyboardPropertyValue_OEM_5 = 0xdc,
    BlcKeyboardPropertyValue_OEM_6 = 0xdd,
    BlcKeyboardPropertyValue_OEM_7 = 0xde,
} BlcKeyboardKeyValue;

typedef enum {
    BlcMousePropertyValue_DEFAULT = 0,
    BlcMousePropertyValue_BUTTONLEFT = 1,
    BlcMousePropertyValue_BUTTONMIDDLE = 2,
    BlcMousePropertyValue_BUTTONRIGHT = 3,
} BlcMouseKeyValue;

//标准的6轴13键游戏手柄
typedef enum {
    BlcConsolePropertyValue_BTN_RightArrow = 0x0,
    BlcConsolePropertyValue_BTN_LeftArrow = 0x1,
    BlcConsolePropertyValue_BTN_DownArrow = 0x2,
    BlcConsolePropertyValue_BTN_UpArrow = 0x3,
    BlcConsolePropertyValue_BTN_L1 = 0x4,
    BlcConsolePropertyValue_BTN_R1 = 0x5,
    BlcConsolePropertyValue_BTN_L2 = 0x6,
    BlcConsolePropertyValue_BTN_R2 = 0x7,
    BlcConsolePropertyValue_BTN_SELECT = 0x8,
    BlcConsolePropertyValue_BTN_START = 0x9,
    BlcConsolePropertyValue_BTN_LA = 0xa,
    BlcConsolePropertyValue_BTN_RA = 0xb,
    BlcConsolePropertyValue_BTN_MODE = 0xc,
    BlcConsolePropertyValue_BTN_TURBO = 0xd,
    BlcConsolePropertyValue_BTN_CLEAR = 0xe,
    BlcConsolePropertyValue_AXIS_POV0 = 0xf,
    BlcConsolePropertyValue_AXIS_X = 0x10,
    BlcConsolePropertyValue_AXIS_Y = 0x11,
    BlcConsolePropertyValue_AXIS_Z = 0x12,
    BlcConsolePropertyValue_AXIS_Z_ROTATION = 0x13,
    BlcConsolePropertyValue_BTN_1 = 0x14,
    BlcConsolePropertyValue_BTN_2 = 0x15,
    BlcConsolePropertyValue_BTN_3 = 0x16,
    BlcConsolePropertyValue_BTN_4 = 0x17,
} BlcConsoleKeyValue;

#ifdef __cplusplus
}
#endif

#endif
