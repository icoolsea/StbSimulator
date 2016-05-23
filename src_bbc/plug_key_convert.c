#include "../include/blccore.h"
#include "../include/plug/ipanel_key.h"
//宽云的键值转换成中间件的键值
int bbcv_plug_bbcvkeytomwkey(int standardid, int bbcvkey) // bbcv实现
{
    BlcCore_printf(
                "%s %d bbcv_plug_mwkeytobbcvkey standardid = %d,bbcvkey = %d\n",
                __FUNCTION__, __LINE__, standardid, bbcvkey);

    // return bbcvkey;

    if (standardid == BlcTermStandardID_Ipanel) {
        switch (bbcvkey) {
        case BlcIrrPropertyValue_SELECT:
            return IPANEL_CLOUD_IRKEY_SELECT;
        case BlcIrrPropertyValue_DIGIT1:
            return IPANEL_CLOUD_IRKEY_NUM1;
        case BlcIrrPropertyValue_DIGIT2:
            return IPANEL_CLOUD_IRKEY_NUM2;
        case BlcIrrPropertyValue_DIGIT3:
            return IPANEL_CLOUD_IRKEY_NUM3;
        case BlcIrrPropertyValue_DIGIT4:
            return IPANEL_CLOUD_IRKEY_NUM4;
        case BlcIrrPropertyValue_DIGIT5:
            return IPANEL_CLOUD_IRKEY_NUM5;
        case BlcIrrPropertyValue_DIGIT6:
            return IPANEL_CLOUD_IRKEY_NUM6;
        case BlcIrrPropertyValue_DIGIT7:
            return IPANEL_CLOUD_IRKEY_NUM7;
        case BlcIrrPropertyValue_DIGIT8:
            return IPANEL_CLOUD_IRKEY_NUM8;
        case BlcIrrPropertyValue_DIGIT9:
            return IPANEL_CLOUD_IRKEY_NUM9;
        case BlcIrrPropertyValue_DIGIT0:
            return IPANEL_CLOUD_IRKEY_NUM0;
        case BlcIrrPropertyValue_LANGUAGE:
            return IPANEL_CLOUD_IRKEY_LANGUAGE;
        case BlcIrrPropertyValue_COLLECTION:
            return IPANEL_CLOUD_IRKEY_FAVORITE;
        case BlcIrrPropertyValue_SWITCH:
            return IPANEL_CLOUD_IRKEY_SWITCH;
            // case BlcIrrPropertyValue_MENU:
            //     return IPANEL_CLOUD_IRKEY_MENU;
        case BlcIrrPropertyValue_ARROWUP:
            return IPANEL_CLOUD_IRKEY_UP;
        case BlcIrrPropertyValue_ARROWDOWN:
            return IPANEL_CLOUD_IRKEY_DOWN;
        case BlcIrrPropertyValue_ARROWLEFT:
            return IPANEL_CLOUD_IRKEY_LEFT;
        case BlcIrrPropertyValue_ARROWRIGHT:
            return IPANEL_CLOUD_IRKEY_RIGHT;
        case BlcIrrPropertyValue_VOLUME_UP:
            return IPANEL_CLOUD_IRKEY_VOLUME_UP;
        case BlcIrrPropertyValue_VOLUME_DOWN:
            return IPANEL_CLOUD_IRKEY_VOLUME_DOWN;
        case BlcIrrPropertyValue_BACK:
            return IPANEL_CLOUD_IRKEY_BACK;
        case BlcIrrPropertyValue_EXIT:
            return IPANEL_CLOUD_IRKEY_EXIT;
        case BlcIrrPropertyValue_PREVUE:
            return IPANEL_CLOUD_IRKEY_FORETELL;
        case BlcIrrPropertyValue_PAGEDOWN:
            return IPANEL_CLOUD_IRKEY_PAGE_DOWN;
        case BlcIrrPropertyValue_PAGEUP:
            return IPANEL_CLOUD_IRKEY_PAGE_UP;
        case BlcIrrPropertyValue_MESSAGE:
            return IPANEL_CLOUD_IRKEY_INFO;
        case BlcIrrPropertyValue_F1:
            return IPANEL_CLOUD_IRKEY_RED;
        case BlcIrrPropertyValue_F2:
            return IPANEL_CLOUD_IRKEY_GREEN;
        case BlcIrrPropertyValue_F3:
            return IPANEL_CLOUD_IRKEY_YELLOW;
        default:
            return -1;
        }
    } else if (standardid == BlcTermStandardID_Wasu) {
        return bbcvkey;
    } else {
        return -2;
    }
}

//中间件的键值转换成宽云的键值
int bbcv_plug_mwkeytobbcvkey(int standardid, int mwkey) // bbcv实现
{
    BlcCore_printf("%s %d bbcv_plug_mwkeytobbcvkey standardid = %d,mwkey = %d\n",
                   __FUNCTION__, __LINE__, standardid, mwkey);

    // return mwkey;

    if (standardid == BlcTermStandardID_Ipanel) {
        switch (mwkey) {

        case IPANEL_CLOUD_IRKEY_BACK:
            return BlcIrrPropertyValue_BACK;
        case IPANEL_CLOUD_IRKEY_EXIT:
            return BlcIrrPropertyValue_EXIT;
        case IPANEL_CLOUD_IRKEY_PAGE_UP:
            return BlcIrrPropertyValue_PAGEDOWN;
        case IPANEL_CLOUD_IRKEY_PAGE_DOWN:
            return BlcIrrPropertyValue_PAGEUP;
        case IPANEL_CLOUD_IRKEY_LEFT:
            return BlcIrrPropertyValue_ARROWLEFT;
        case IPANEL_CLOUD_IRKEY_UP:
            return BlcIrrPropertyValue_ARROWUP;
        case IPANEL_CLOUD_IRKEY_RIGHT:
            return BlcIrrPropertyValue_ARROWRIGHT;
        case IPANEL_CLOUD_IRKEY_DOWN:
            return BlcIrrPropertyValue_ARROWDOWN;
        case IPANEL_CLOUD_IRKEY_SELECT:
            return BlcIrrPropertyValue_SELECT;
        case IPANEL_CLOUD_IRKEY_NUM0:
            return BlcIrrPropertyValue_DIGIT0;
        case IPANEL_CLOUD_IRKEY_NUM1:
            return BlcIrrPropertyValue_DIGIT1;
        case IPANEL_CLOUD_IRKEY_NUM2:
            return BlcIrrPropertyValue_DIGIT2;
        case IPANEL_CLOUD_IRKEY_NUM3:
            return BlcIrrPropertyValue_DIGIT3;
        case IPANEL_CLOUD_IRKEY_NUM4:
            return BlcIrrPropertyValue_DIGIT4;
        case IPANEL_CLOUD_IRKEY_NUM5:
            return BlcIrrPropertyValue_DIGIT5;
        case IPANEL_CLOUD_IRKEY_NUM6:
            return BlcIrrPropertyValue_DIGIT6;
        case IPANEL_CLOUD_IRKEY_NUM7:
            return BlcIrrPropertyValue_DIGIT7;
        case IPANEL_CLOUD_IRKEY_NUM8:
            return BlcIrrPropertyValue_DIGIT8;
        case IPANEL_CLOUD_IRKEY_NUM9:
            return BlcIrrPropertyValue_DIGIT9;
        case IPANEL_CLOUD_IRKEY_RED:
            return BlcIrrPropertyValue_F1;
        case IPANEL_CLOUD_IRKEY_GREEN:
            return BlcIrrPropertyValue_F2;
        case IPANEL_CLOUD_IRKEY_YELLOW:
            return BlcIrrPropertyValue_F3;
            // case IPANEL_CLOUD_IRKEY_MENU:
            //	return BlcIrrPropertyValue_MENU;
        case IPANEL_CLOUD_IRKEY_INFO:
            return BlcIrrPropertyValue_MESSAGE;
        case IPANEL_CLOUD_IRKEY_FAVORITE:
            return BlcIrrPropertyValue_COLLECTION;
        case IPANEL_CLOUD_IRKEY_LANGUAGE:
            return BlcIrrPropertyValue_LANGUAGE;
        case IPANEL_CLOUD_IRKEY_RECOMMEND:
            return BlcIrrPropertyValue_COMMEND;
        case IPANEL_CLOUD_IRKEY_FORETELL:
            return BlcIrrPropertyValue_PREVUE;
        case IPANEL_CLOUD_IRKEY_SWITCH:
            return BlcIrrPropertyValue_SWITCH;
        default:
            return -1;
        }
    } else if (standardid == BlcTermStandardID_Wasu) {
        return mwkey;
    } else {
        return -2;
    }
}
