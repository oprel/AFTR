#include "global.h"
#include "m_msg_main.h"
#include "m_choice_main.h"
#include "m_lib.h"
#include "m_npc.h"
#include "m_string.h"

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_base_window_p.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_request_priority.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Change_request_main_index.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_main_index.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_main_wait.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_not_series_main_wait.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_main_hide.s")

void mMsg_Set_client_actor_p(MessageWindow* window, Actor* clientActor, s32 showName) {
    if (clientActor != NULL) {
        f32 width;
        f32 offsetX;
        s32 pad;
        char name[ANIMAL_NAME_LEN];
        s32 len;

        window->clientActor = clientActor;
        window->showActorName = showName;

        mNpc_GetNpcWorldName(name, (Npc*)clientActor);
        len = mMsg_Get_Length_String(name, ANIMAL_NAME_LEN);
        width = mFont_GetStringWidth(name, len, FALSE);
        offsetX = ((12.0f * (f32)ANIMAL_NAME_LEN_VANILLA) - width) * 0.5f; //needs to be ANIMAL_NAME_LEN_VANILLA for proper centering

        window->nameplateX = 61.0f + offsetX;
        window->nameplateY = 64.0f;
        window->clientNameLen = len;
    } else {
        window->clientActor = NULL;
        window->showActorName = FALSE;
    }
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_forceoff.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_appear.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_disappear_wait.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_disappear_wait_sub.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_disappear_wait_type1.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_disappear_wait_type2.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_wait.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_appear_wait.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_appear_wait_type2.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_appear_wait_type1.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_normal.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_request_main_cursol.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_free_str.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_free_str_cl.s")

#ifdef AFTR
static char S_item_str[MESSAGE_ITEM_STR_MAX][ITEM_NAME_LEN];

void mMsg_Set_item_str(MessageWindow* window, s32 itemStrNo, char* str, s32 len) {
    if (itemStrNo >= MESSAGE_ITEM_STR0 && itemStrNo < MESSAGE_ITEM_STR_MAX && str != NULL) {
        s32 i;
        char* itemStr = S_item_str[itemStrNo];

        if (len >= 10) {
            len = ITEM_NAME_LEN;
        }

        for (i = 0; i < ITEM_NAME_LEN; i++) {
            if (i < len) {
                itemStr[i] = str[i];
            } else {
                itemStr[i] = ' ';
            }
        }
    }
}
void mMsg_Set_item_str_pad0(void) {}
void mMsg_Set_item_str_pad1(void) {}
void mMsg_Set_item_str_pad2(void) {}
void mMsg_Set_item_str_pad3(void) {}
void mMsg_Set_item_str_pad4(void) {}
void mMsg_Set_item_str_pad5(void) {}
void mMsg_Set_item_str_pad6(void) {}
void mMsg_Set_item_str_pad7(void) {}
void mMsg_Set_item_str_pad8(void) {}
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_item_str.s")
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_free_str.s")

#ifdef AFTR
void mMsg_Get_item_str(MessageWindow* window, s32 itemStrNo, char* str) {
    if (itemStrNo >= MESSAGE_ITEM_STR0 && itemStrNo < MESSAGE_ITEM_STR_MAX && str != NULL) {
        mem_copy((u8*)str, (u8*)S_item_str[itemStrNo], ITEM_NAME_LEN);
    }
}
void mMsg_Get_item_str_pad0(void) {}
void mMsg_Get_item_str_pad1(void) {}
void mMsg_Get_item_str_pad2(void) {}
void mMsg_Get_item_str_pad3(void) {}
void mMsg_Get_item_str_pad4(void) {}
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_item_str.s")
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_mail_str.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_continue_msg_num.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_msg_num.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_give_item.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_SizeCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Count_SameCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_LastCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_LastCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_ContinueCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_ContinueCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NextIndex_ContinueCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NextIndex_LastCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NextIndex_SetSelectWindowCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_CursolSetTimeCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_CursolSetTimeCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_ColorCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_ColorCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_OrderCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_OrderCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_SoundCutCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_SoundCutCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_bgm_make_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_bgm_make.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_bgm_delete_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_bgm_delete.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_MsgTimeEnd_time_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_MsgTimeEnd_time.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MsgTimeEndCode_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MsgTimeEndCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NextIndex_MsgTimeEndCode.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_sound_trg_sys_forData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_sound_trg_sys.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_LineFontColor.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_init_FontColor.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_init_NowDisplayLIne.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Clear_CursolIndex.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_SetTimer.s")

#ifdef AFTR
void mMsg_Get_MsgDataAddressAndSize(s32 idx, u32* vramp, size_t* size) {
    s32 sized;
    u32 offsetPtrs[5];

    if (idx >= 0 && idx < MESSAGE_IDX_MAX) {
        u32 vrom;
        if (idx & 1) {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_CF9000[idx - 1], 8, "../m_msg_main.c", 855);
            vrom = offsetPtrs[0];
            sized = offsetPtrs[1] - vrom;
        } else if (idx == 0) {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_CF9000, 8, "../m_msg_main.c", 862);
            vrom = 0;
            sized = offsetPtrs[0] - vrom;
        } else {
            DmaMgr_RequestSyncDebug(offsetPtrs, (u32)&D_CF9000[idx - 2], 16, "../m_msg_main.c", 868);
            vrom = offsetPtrs[1];
            sized = offsetPtrs[2] - vrom;
        }
        if (sized <= MESSAGE_TEXT_BUFFER_LEN) {
            *vramp = (u32)vrom + (u32)&D_BD4000;
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
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_MsgDataAddressAndSize.s")
#endif

#ifdef AFTR
s32 mMsg_Count_MsgData(char* data) {
    s32 i = 0;

    while (i < MESSAGE_TEXT_BUFFER_LEN) {
        if (mMsg_Check_LastCode_forData(data, i) || mMsg_Check_ContinueCode_forData(data, i) ||
            mMsg_Check_MsgTimeEndCode_forData(data, i)) {
            i += mFont_CodeSize_idx_get(data, i);
            break;
        }

        i += mFont_CodeSize_idx_get(data, i);
    }

    return i;
}
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Count_MsgData.s")
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_LoadMsgData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_ChangeMsgData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Unset_NowUtter.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_NowUtter.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NowUtter.s")

#ifdef AFTR
static MessageData S_msg_data; //static to allow for larger buffer

extern MessageWindow B_80142410_jp;
#define mMsg_window B_80142410_jp
extern void func_8009FA58_jp(f32 timer);
extern void func_8009FB2C_jp(u8 status);

void mMsg_init(Game_Play* play) {
    S_msg_data.dataLoaded = FALSE;
    mMsg_window.dataLoaded = FALSE;
    mMsg_window.msgNo = 0;
    mMsg_window.unk_008 = 0;
    mMsg_window.messageData = &S_msg_data;
    mMsg_window.centerX = 160.0f;
    mMsg_window.centerY = 185.4f;
    mMsg_window.width = 245.0f;
    mMsg_window.height = 96.0f;
    mMsg_window.clientActor = NULL;
    mMsg_window.clientNameLen = 0;
    mMsg_window.nameplateX = 61.0;
    mMsg_window.nameplateY = 64.0;
    mMsg_window.showContinueButton = FALSE;
    mMsg_window.nameTextColor.r = 50;
    mMsg_window.nameTextColor.g = 90;
    mMsg_window.nameTextColor.b = 0;
    mMsg_window.nameTextColor.a = 255;
    mMsg_window.nameBackgroundColor.r = 160;
    mMsg_window.nameBackgroundColor.g = 215;
    mMsg_window.nameBackgroundColor.b = 30;
    mMsg_window.nameBackgroundColor.a = 255;
    mMsg_window.windowBackgroundColor.r = 235;
    mMsg_window.windowBackgroundColor.g = 255;
    mMsg_window.windowBackgroundColor.b = 235;
    mMsg_window.windowBackgroundColor.a = 255;
    mMsg_init_FontColor(&mMsg_window);
    mMsg_init_NowDisplayLIne(&mMsg_window);
    mMsg_window.continueButtonColor.r = 0;
    mMsg_window.continueButtonColor.g = 0;
    mMsg_window.continueButtonColor.b = 255;
    mMsg_window.continueButtonColor.a = 0;
    mMsg_window.fontScaleX = 1.0;
    mMsg_window.fontScaleY = 1.0;
    mMsg_window.unk_1A0 = 196;
#ifndef AF_CN
    mMsg_window.unk_1A4 = 66;
#else
    mMsg_window.unk_1A4 = 50;
#endif
    mMsg_window.textLines = MESSAGE_LINE_NUMBER;
    mMsg_window.nowDisplayLine = 0;
    mMsg_window.timer = 0.0f;
    mMsg_SetTimer(&mMsg_window, 0.0f);
    mMsg_window.continueButtonTimer = 0.0f;
    mMsg_window.startTextCursolIdx = 0;
    mMsg_window.endTextCursolIdx = 0;
    mMsg_window.windowScale = 0.0f;
    mMsg_window.textScale = 1.0f;
    mMsg_window.requestedMainIndex = -1;
    mMsg_window.requestedPriority = 0;
    mMsg_window.mainIndex = 0;
    mMsg_window.drawFlag = 0;
    mMsg_window.cancelFlag = 0;
    mMsg_window.cancelableFlag = 0;
    mMsg_window.continueMsgNo = 0xFFFF;
    mMsg_Unset_CancelNormalContinue(&mMsg_window);
    mMsg_Unset_ForceNext(&mMsg_window);
    mMsg_Unset_LockContinue(&mMsg_window);
    func_8009FA58_jp(0.0f);
    func_8009FB2C_jp(0);
    mMsg_window.statusFlags &= ~0xC0;
    mMsg_window.freeStrColorIdx[0] = 0;
    mMsg_window.freeStrColorIdx[1] = 0;
    mMsg_window.freeStrColorIdx[2] = 0;
}
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_init.s")
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_Length_String.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MainNormalContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MainNormal.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MainHide.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_MainDisappear.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_CancelNormalContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Unset_CancelNormalContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_ForceNext.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Unset_ForceNext.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_LockContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_LockContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Unset_LockContinue.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_idling_req.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_idling_now.s")

#ifdef AFTR
s32 mMsg_MoveDataCut(char* data, s32 dstIdx, s32 srcIdx, s32 len, s32 spaceFlag) {
    s32 newLen = len;

    if (dstIdx < srcIdx) {
        while (srcIdx < len) {
            data[dstIdx] = data[srcIdx];
            dstIdx++;
            srcIdx++;
        }

        newLen -= srcIdx - dstIdx;

        if (spaceFlag) {
            while (dstIdx < len) {
                data[dstIdx] = ' ';
                dstIdx++;
            }
        }
    } else if (dstIdx > srcIdx) {
        s32 moveSize = len - srcIdx;
        s32 offs = dstIdx - srcIdx;

        newLen += offs;

        if (newLen <= MESSAGE_TEXT_BUFFER_LEN) {
            s32 i;
            char* dstPtr = data + newLen - 1;
            char* srcPtr = data + len - 1;

            for (i = 0; i < moveSize; i++) {
                *dstPtr-- = *srcPtr--;
            }
        }
    }

    return newLen;
}
#else
#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_MoveDataCut.s")
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyString.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_PlayerNameColor.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyPlayerName.s")

s32 mMsg_CopyTalkName(Actor* actor, char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    char name[ANIMAL_NAME_LEN];
    s32 nameLen;
    s32 newLen;

    if (actor != NULL) {
        mNpc_GetNpcWorldName(name, (Npc*)actor);
        nameLen = mMsg_Get_Length_String(name, sizeof(name));
    } else {
        nameLen = 0;
    }

    newLen = mMsg_MoveDataCut(data, startIdx + nameLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], name, nameLen);

    return newLen;
}

//#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyTail.s")

s32 mMsg_CopyTail(Actor* actor, char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    char* tail = (actor != NULL) ? mNpc_GetWordEnding((Npc*)actor) : NULL;
    s32 tailLen = (actor != NULL) ? mMsg_Get_Length_String(tail, ANIMAL_CATCHPHRASE_LEN) : 0;
    s32 newLen;

    newLen = mMsg_MoveDataCut(data, startIdx + tailLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], tail, tailLen);

    return newLen;
}

//#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyYear.s")

s32 mMsg_CopyYear(char* data, s32 startIdx, s32 len){
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u16 year = COMMON_GET(time.rtcTime.year);
    #ifdef AFTR
    char string[4];
    #else
    char string[6];
    #endif
    s32 nameLen;
    s32 newLen;

    nameLen = mString_Load_YearStringFromRom(string, year);

    newLen = mMsg_MoveDataCut(data, startIdx + nameLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, nameLen);

    return newLen;
}

//#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyMonth.s")
s32 mMsg_CopyMonth(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 month = COMMON_GET(time.rtcTime.month);
    #ifdef AFTR
    char string[9];
    #else
    char string[4];
    #endif
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_MonthStringFromRom(string, month);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyWeek.s")
s32 mMsg_CopyWeek(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 weekday = COMMON_GET(time.rtcTime.weekday);
    #ifdef AFTR
    char string[9];
    #else
    char string[5];
    #endif
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_WeekStringFromRom(string, weekday);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyDay.s")
s32 mMsg_CopyDay(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 day = COMMON_GET(time.rtcTime.day);
    char string[4];
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_DayStringFromRom(string, day);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyHour.s")
s32 mMsg_CopyHour(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 hour = COMMON_GET(time.rtcTime.hour);
    #ifdef AFTR
    char string[2];
    #else
    char string[6];
    #endif
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_HourStringFromRom(string, hour);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyMin.s")
s32 mMsg_CopyMin(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 min = COMMON_GET(time.rtcTime.min);
    #ifdef AFTR
    char string[8];
    #else
    char string[4];
    #endif
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_MinStringFromRom(string, min);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopySec.s")
s32 mMsg_CopySec(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    u8 sec = COMMON_GET(time.rtcTime.sec);
    #ifdef AFTR
    char string[2];
    #else
    char string[5];
    #endif
    s32 stringLen;
    s32 newLen;

    stringLen = mString_Load_SecStringFromRom(string, sec);

    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyFree.s")
s32 mMsg_CopyFree(MessageWindow* win, s32 num, char* data, s32 startIdx, s32 len) {
    s32 commandLen;
    char* string;
    s32 stringLen;
    s32 newLen;

    if (num < 0 || num >= MESSAGE_FREE_STR_MAX) {
        num = 0;
    }

    commandLen = mFont_CodeSize_idx_get(data, startIdx);
    string = win->freeStr[num];
    stringLen = mMsg_Get_Length_String(string, MESSAGE_FREE_STR_LEN);
    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_PfColor.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyDetermination.s")

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyCountryName.s")
s32 mMsg_CopyCountryName(char* data, s32 startIdx, s32 len) {
    s32 commandLen = mFont_CodeSize_idx_get(data, startIdx);
    char* countryName = mLd_GetLandName();
    s32 countryNameLen = mMsg_Get_Length_String(countryName, LAND_NAME_SIZE);
    s32 newStartIdx = startIdx + countryNameLen;
    s32 newLen;
    char suffix[M_STRING_MAX_STRING_LENGTH];
    s32 suffixLen;

    newLen = mMsg_MoveDataCut(data, newStartIdx, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], countryName, countryNameLen);

    #ifndef AFTR
    mString_Load_StringFromRom(suffix, M_STRING_MAX_STRING_LENGTH, 0x1E4);
    suffixLen = mMsg_Get_Length_String(suffix, M_STRING_MAX_STRING_LENGTH);

    newLen = mMsg_MoveDataCut(data, newStartIdx + suffixLen, newStartIdx, newLen, FALSE);
    mMsg_CopyString(&data[newStartIdx], suffix, suffixLen);
    #endif

    return newLen;
}

#ifdef AFTR //padding to even out country name size change
void mMsg_pad0(void) {}
void mMsg_pad1(void) {}
void mMsg_pad2(void) {}
void mMsg_pad3(void) {}
void mMsg_pad4(void) {}
void mMsg_pad5(void) {}
void mMsg_pad6(void) {}
void mMsg_pad7(void) {}
void mMsg_pad8(void) {}
void mMsg_pad9(void) {}
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyRamdomNumber2.s")

// #pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_CopyItem.s")
s32 mMsg_CopyItem(MessageWindow* win, s32 num, char* data, s32 startIdx, s32 len) {
    s32 commandLen;
    char* string;
    s32 stringLen;
    s32 newLen;

    if (num < 0 || num >= MESSAGE_FREE_STR_MAX) {
        num = 0;
    }

    commandLen = mFont_CodeSize_idx_get(data, startIdx);
#ifdef AFTR
    string = S_item_str[num];
#else
    string = win->itemStr[num];
#endif
    stringLen = mMsg_Get_Length_String(string, ITEM_NAME_LEN);
    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

#ifdef AFTR
// pad to the size of the vanilla function, or everything after it in the code segment moves
void mMsg_CopyItem_pad0(void) {}
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F670_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F730_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F75C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F790_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F7CC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F800_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F830_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F864_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F8AC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F9D8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009F9F8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FA18_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FA38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FA58_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FAD4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FB2C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FB54_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FBD8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FC2C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FC5C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FCB8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FCE0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_sound_spec_change_voice.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FD80_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FDA0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FDF8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FE4C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FE6C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FE90_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FF24_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FF68_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_8009FFB0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0110_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A014C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0184_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A01C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A03B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0478_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A04E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A054C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A05A8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A05D8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0614_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0650_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A06AC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0770_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A07E8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0864_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A08B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A08F8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0964_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0984_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A09A4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A09C4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A09E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0A04_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0A60_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0B14_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0B34_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0B54_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0B74_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0B94_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0BB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0D94_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0DB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0DD4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0DF4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0FCC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A0FEC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A100C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A102C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1078_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A10D8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1124_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1170_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A11B4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A11F8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A123C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1280_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A12C4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1308_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A134C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1394_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A141C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1444_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1468_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A148C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A14B4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A14DC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1500_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1528_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1550_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1578_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A15A0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A15C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A15F0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1618_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1640_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1668_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1690_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A16B8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A16E0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1708_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1730_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1774_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A17B8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A17FC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1844_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A186C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1894_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A18BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A18E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A190C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1954_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A197C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A19C4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A19E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1A04_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1A24_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1A44_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1A64_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1A84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1AA4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1AC4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1AE4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1B04_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1B4C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1B6C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1B8C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1BAC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1BCC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1BEC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1C24_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1CA4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1CDC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1D14_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1D4C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1DD0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1E38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1EA0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1F1C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1F7C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A1FB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2000_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A204C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2098_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A20E0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2150_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A21C0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A223C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A24A8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A24D0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2538_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2588_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2620_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2648_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A268C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A272C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2784_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A27FC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A287C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A289C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A28DC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2904_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2948_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A29B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2AB0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2AD0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2BB0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2C4C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2CE4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A2EAC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3190_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A31D8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3220_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_ct.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3304_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A332C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Main.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Draw.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsm_ClearRecord.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3420_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A345C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A34E8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A35C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3658_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsm_SendInformationMail.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3784_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A37D0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3810_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3A5C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3B84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3C18_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3CF0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3E34_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3F70_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A3FB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4184_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A41BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4254_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A43C0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4448_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A44D4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4508_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A456C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4614_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A46F0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A49E8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4A84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4B00_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4D10_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4DC8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4E74_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4F38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A4FD0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A50AC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A518C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A524C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/func_800A5460_jp.s")
