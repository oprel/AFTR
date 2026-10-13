#include "global.h"
#include "m_msg_main.h"
#include "m_choice_main.h"
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

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_item_str.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_free_str.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_item_str.s")

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

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Get_MsgDataAddressAndSize.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Count_MsgData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_LoadMsgData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_ChangeMsgData.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Unset_NowUtter.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Set_NowUtter.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_Check_NowUtter.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_init.s")

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

#pragma GLOBAL_ASM("asm/jp/nonmatchings/code/m_msg_main/mMsg_MoveDataCut.s")

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

    mString_Load_StringFromRom(suffix, M_STRING_MAX_STRING_LENGTH, 0x1E4);
    suffixLen = mMsg_Get_Length_String(suffix, M_STRING_MAX_STRING_LENGTH);

    newLen = mMsg_MoveDataCut(data, newStartIdx + suffixLen, newStartIdx, newLen, FALSE);
    mMsg_CopyString(&data[newStartIdx], suffix, suffixLen);

    return newLen;
}

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
    string = win->itemStr[num];
    stringLen = mMsg_Get_Length_String(string, ITEM_NAME_LEN);
    newLen = mMsg_MoveDataCut(data, startIdx + stringLen, startIdx + commandLen, len, FALSE);
    mMsg_CopyString(&data[startIdx], string, stringLen);

    return newLen;
}

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
