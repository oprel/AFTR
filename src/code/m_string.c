#include "m_string.h"
#include "m_std_dma.h"
#include "m_lib.h"

#include "m_msg_main.h"
#include "m_font.h"

#ifdef AFTR
#define M_STRING_WEEKDAY_START 9
#define M_STRING_MONTH_START 1562
#define M_STRING_DAY_START 1574
#define M_STRING_MAX_STR 1605
#else
#define M_STRING_MAX_STR 0x61A
#endif


extern u32 D_D18000[];
extern u32 D_D16000;
void mString_Get_StringDataAddressAndSize(s32 idx, uintptr_t* vramp, size_t* size) {
    s32 sized;
    uintptr_t offsetPtrs[5];

    if (idx >= 0 && idx < M_STRING_MAX_STR) {
        uintptr_t vrom;
        if (idx & 1) {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_D18000[idx - 1], 8, "../m_string.c", 62);
            vrom = offsetPtrs[0];
            sized = offsetPtrs[1] - vrom;
        } else if (idx == 0) {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_D18000, 8, "../m_string.c", 69);
            vrom = 0;
            sized = offsetPtrs[0] - vrom;
        } else {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_D18000[idx - 2], 16, "../m_string.c", 75);
            vrom = offsetPtrs[1];
            sized = offsetPtrs[2] - vrom;
        }
        if (sized < 65) {
            *vramp = (uintptr_t)vrom + (u32)&D_D16000;
            *size = sized;
        } else {
            *vramp = NULL;
            *size = 0;
        }
    } else {
        *vramp = NULL;
        *size = 0;
    }
}

void mString_Load_StringFromRom(char* dst, s32 dstLen, s32 strIdx) {
    uintptr_t stringAddr;
    size_t stringSize;

    if (strIdx >= 0 && strIdx < M_STRING_MAX_STR) {
        mString_Get_StringDataAddressAndSize(strIdx, &stringAddr, &stringSize);

        if (stringSize == 0) {
            mem_clear((u8*)dst, dstLen, ' ');
        } else if (stringAddr != 0) {
            char buff[80];
            u32 baseAddr = stringAddr & ~7;
            s32 ofs = stringAddr - baseAddr;
            s32 i;
            char* strSrc;
            s32 sizeCopy;
            char* strDst;
            UNUSED s32 pad;

            DmaMgr_RequestSyncDebug(buff, baseAddr, ALIGN8(ofs + stringSize), "../m_string.c", 153);
            if (dstLen < (s32)stringSize) {
                stringSize = dstLen;
            }

            strDst = dst;
            sizeCopy = stringSize;
            strSrc = &buff[ofs];
            for (i = 0; i < sizeCopy; i++) {
                *strDst++ = *strSrc++;
            }

            for (; i < dstLen; i++) {
                *strDst++ = ' ';
            }
        }
    }
}

#ifdef AFTR
static s32 mString_UnintToString2Digit(char* dst, u32 num) {
    s32 len = 0;

    if (num < 10) {
        dst[0] = '0';
        len = 1;
    }

    len += mFont_UnintToString(dst + len, num, 2 - len, TRUE, FALSE);
    return len;
}
#endif

s32 mString_Load_YearStringFromRom(char* dst, lbRTC_year_t year) {
#ifdef AFTR
    if (year < lbRTC_YEAR_MIN || year > lbRTC_YEAR_MAX) {
        year = GAME_YEAR_MIN;
    }

    //dst size: 4
    return mFont_UnintToString(dst, year, 4, TRUE, FALSE);
#else
    s32 idx;

    if (year < lbRTC_YEAR_MIN || year > lbRTC_YEAR_MAX) {
        year = GAME_YEAR_MIN;
    }

    idx = mFont_UnintToString(dst, year, 6, TRUE, FALSE);
    mString_Load_StringFromRom(&dst[idx], 6 - idx, 3);
    return mMsg_Get_Length_String(dst, 6);
#endif
}

s32 mString_Load_MonthStringFromRom(char* dst, lbRTC_month_t month) {
#ifdef AFTR
    if (month < lbRTC_JANUARY || month > lbRTC_DECEMBER) {
        month = lbRTC_JANUARY;
    }

    //dst size: 9
    mString_Load_StringFromRom(dst, 9, M_STRING_MONTH_START + month - 1);
    return mMsg_Get_Length_String(dst, 9);
#else
    s32 idx;

    if (month < lbRTC_JANUARY || month > lbRTC_DECEMBER) {
        month = lbRTC_JANUARY;
    }

    idx = mFont_UnintToString(dst, month, 4, TRUE, FALSE);
    mString_Load_StringFromRom(&dst[idx], 4 - idx, 4);
    return mMsg_Get_Length_String(dst, 4);
#endif
}

s32 mString_Load_WeekStringFromRom(char* dst, lbRTC_weekday_t weekday) {
    if (weekday > lbRTC_SATURDAY) {
        weekday = 0;
    }

#ifdef AFTR
    //dst size: 9
    mString_Load_StringFromRom(dst, 9, weekday + M_STRING_WEEKDAY_START);
    return mMsg_Get_Length_String(dst, 9);
#else
    mString_Load_StringFromRom(dst, 5, weekday + 9);
    return mMsg_Get_Length_String(dst, 4);
#endif
}

s32 mString_Load_DayStringFromRom(char* dst, lbRTC_day_t day) {
#ifdef AFTR
    if (day < 1 || day > 31) {
        day = 1;
    }
    //dst size: 4
    mString_Load_StringFromRom(dst, 4, M_STRING_DAY_START + day - 1);
    return mMsg_Get_Length_String(dst, 4);
#else
    s32 idx;

    if (day < 1 || day > 31) {
        day = 1;
    }

    idx = mFont_UnintToString(dst, day, 4, TRUE, FALSE);
    mString_Load_StringFromRom(&dst[idx], 4 - idx, 5);
    return mMsg_Get_Length_String(dst, 4);
#endif
}

s32 mString_Load_HourStringFromRom(char* dst, lbRTC_hour_t hour) {
#ifdef AFTR
    s32 hour12;

    //dst size: 2 (no p.m. specifier)

    if (hour >= lbRTC_HOURS_PER_DAY) {
        hour = 0;
    }
    hour12 = hour % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }

    return mFont_UnintToString(dst, hour12, 2, TRUE, FALSE);
#else
    UNUSED s32 pad;
    s32 specifier;
    s32 len;

    if (hour >= lbRTC_HOURS_PER_DAY) {
        hour = 0;
    }

    specifier = hour < 12 ? 1 : 2;

    mString_Load_StringFromRom(dst, 6, specifier);
    len = mMsg_Get_Length_String(dst, 6);
    len += mFont_UnintToString(dst + len, hour % 12, 6 - len, TRUE, FALSE);

    mString_Load_StringFromRom(dst + len, 6 - len, 6);
    return mMsg_Get_Length_String(dst, 6);
#endif
}

s32 mString_Load_MinStringFromRom(char* dst, lbRTC_min_t min) {
#ifdef AFTR
    s32 len;
    u8 hour = COMMON_GET(time.rtcTime.hour);

    if (min >= lbRTC_MINUTES_PER_HOUR) {
        min = 0;
    }

    if (hour >= lbRTC_HOURS_PER_DAY) {
        hour = 0;
    }

    //dst size: 8 (p.m. specifier)
    len = mString_UnintToString2Digit(dst, min);
    dst[len] = ' ';
    len++;
    mString_Load_StringFromRom(dst + len, 8 - len, hour < 12 ? 1 : 2);
    return mMsg_Get_Length_String(dst, 8);
#else
    s32 idx;

    if (min >= lbRTC_MINUTES_PER_HOUR) {
        min = 0;
    }

    idx = mFont_UnintToString(dst, min, 4, TRUE, FALSE);
    mString_Load_StringFromRom(&dst[idx], 4 - idx, 7);
    return mMsg_Get_Length_String(dst, 4);
#endif
}

s32 mString_Load_SecStringFromRom(char* dst, lbRTC_sec_t sec) {
#ifdef AFTR
    if (sec >= lbRTC_MINUTES_PER_HOUR) {
        sec = 0;
    }
    //dst size: 2
    return mString_UnintToString2Digit(dst, sec);
#else
    s32 idx;

    if (sec >= lbRTC_MINUTES_PER_HOUR) {
        sec = 0;
    }

    idx = mFont_UnintToString(dst, sec, 5, TRUE, FALSE);
    mString_Load_StringFromRom(&dst[idx], 5 - idx, 8);
    return mMsg_Get_Length_String(dst, 5);
#endif
}

s32 mString_Load_NumberStringAddUnitFromRom(char* dst, u16 num, s32 idx) {
    s32 len = mFont_UnintToString(dst, num, 10, TRUE, FALSE);

    mString_Load_StringFromRom(&dst[len], 10 - len, idx);
    return mMsg_Get_Length_String(dst, 10);
}

#ifdef AFTR //padding to match original size
void mString_pad0(void) {}
void mString_pad1(void) {}
void mString_pad2(void) {}
void mString_pad3(void) {}
void mString_pad4(void) {}
void mString_pad5(void) {}
void mString_pad6(void) {}
void mString_pad7(void) {}
void mString_pad8(void) {}
void mString_pad9(void) {}
void mString_pad10(void) {}
void mString_pad11(void) {}
#endif