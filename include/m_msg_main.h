#ifndef M_MSG_MAIN_H
#define M_MSG_MAIN_H

#include "ultra64.h"
#include "unk.h"
#include "m_actor.h"
#include "m_item_name.h"
#include "m_choice_main.h"
#include "overlays/gamestates/ovl_play/m_play.h"

struct Game_Play;
struct Actor;
struct Color_RGBA8;

//source: https://decomp.me/scratch/LbucU

//temporarily here since no m_font.h exists yet
typedef struct FontSentence {
    u8 unk_00[0x88];
} FontSentence; // size = 0x88

#ifdef AFTR
#define MESSAGE_TEXT_BUFFER_LEN 2048
#else
#define MESSAGE_TEXT_BUFFER_LEN 1024
#endif
#ifndef AF_CN
#define MESSAGE_IDX_MAX 0x2DE8
#else
#define MESSAGE_IDX_MAX 0x2E10
#endif

typedef struct MessageData {
    s32 dataLoaded;
    s32 msgNo;
    s32 msgLen;
    s32 cut;
    char dataBuffer[MESSAGE_TEXT_BUFFER_LEN];
    u8 unk_410[0x10];
} MessageData;

typedef struct MessageMainWaitAppearData {
    s32 lastMainIndex;
} MessageMainWaitAppearData;

typedef struct MessageMainWaitDisappearData {
    s32 lastMainIndex;
} MessageMainWaitDisappearData;

typedef struct MessageMainWaitData {
    s32 lastMainIndex;
} MessageMainWaitData;

typedef union MessageMainData {
    MessageMainWaitAppearData appearWaitData;
    MessageMainWaitDisappearData disappearWaitData;
    MessageMainWaitData waitData;
    u64 align;
} MessageMainData;

typedef struct MessageRequestAppearData {
    Actor* speakerActor;
    s32 msgNo;
    s32 nameShown;
    Color_RGBA8 windowColor;
} MessageRequestAppearData;

typedef struct MessageRequestNormalData {
    s32 waitFlag;
} MessageRequestNormalData;

typedef struct MessageRequestCursolData {
    s32 waitFlag;
} MessageRequestCursolData;

typedef struct MessageRequestDisappearWaitData {
    s32 lastMainIndex;
} MessageRequestDisappearWaitData;

typedef struct MessageRequestAppearWaitData {
    s32 lastMainIndex;
    s32 setupFlag;
} MessageRequestAppearWaitData;

typedef struct MessageRequestWaitData {
    s32 lastMainIndex;
} MessageRequestWaitData;

typedef union MessageRequestData {
    MessageRequestAppearData appearData;
    MessageRequestNormalData normalData;
    MessageRequestCursolData cursolData;
    MessageRequestAppearWaitData appearWaitData;
    MessageRequestWaitData waitData;
    MessageRequestDisappearWaitData disappearWaitData;
    u64 align;
} MessageRequestData;

#define MESSAGE_FREE_STR_LEN 10

#define MESSAGE_MAIL_STR_LEN 68
#define MESSAGE_LINE_NUMBER 4

#ifndef AF_CN
#define MESSAGE_CHAR_NEW_LINE '\xCD'
#define MESSAGE_CHAR_TAG_CODE '\x80'

#else
#define MESSAGE_CHAR_NEW_LINE '\x7D'
#define MESSAGE_CHAR_TAG_CODE '\x80'

#endif

#define MESSAGE_CHAR_CONTROL_CODE '\x7F'

#define MESSAGE_BUTTON_TURN_TIME 30

typedef enum MessageFreeStr {
    MESSAGE_FREE_STR0,
    MESSAGE_FREE_STR1,
    MESSAGE_FREE_STR2,
    MESSAGE_FREE_STR3,
    MESSAGE_FREE_STR4,
    MESSAGE_FREE_STR5,
    MESSAGE_FREE_STR6,
    MESSAGE_FREE_STR7,
    MESSAGE_FREE_STR8,
    MESSAGE_FREE_STR9,
    MESSAGE_FREE_STR10,
    MESSAGE_FREE_STR11,
    MESSAGE_FREE_STR12,
    MESSAGE_FREE_STR13,
    MESSAGE_FREE_STR14,
    MESSAGE_FREE_STR15,
    MESSAGE_FREE_STR16,
    MESSAGE_FREE_STR17,
    MESSAGE_FREE_STR18,
    MESSAGE_FREE_STR19,

    MESSAGE_FREE_STR_MAX
} MessageFreeStr;

typedef enum MessageItemStr {
    MESSAGE_ITEM_STR0,
    MESSAGE_ITEM_STR1,
    MESSAGE_ITEM_STR2,
    MESSAGE_ITEM_STR3,
    MESSAGE_ITEM_STR4,

    MESSAGE_ITEM_STR_MAX
} MessageItemStr;

typedef enum MessageMailStr {
    MESSAGE_MAIL_STR0,
    MESSAGE_MAIL_STR_MAX,
} MessageMailStr;

typedef struct MessageWindow {
    /* 0x000 */ s32 dataLoaded;
    /* 0x004 */ s32 msgNo;
    /* 0x008 */ s32 unk_008;
    /* 0x00C */ MessageData* messageData;
    /* 0x010 */ f32 centerX;
    /* 0x014 */ f32 centerY;
    /* 0x018 */ f32 width;
    /* 0x01C */ f32 height;
    /* 0x020 */ Actor* clientActor;
    /* 0x024 */ s32 showActorName;
    /* 0x028 */ s32 clientNameLen;
    /* 0x02C */ f32 nameplateX;
    /* 0x030 */ f32 nameplateY;
    /* 0x034 */ s32 showContinueButton;
    /* 0x038 */ char freeStr[MESSAGE_FREE_STR_MAX][MESSAGE_FREE_STR_LEN];
    /* 0x100 */ char itemStr[MESSAGE_ITEM_STR_MAX][Choice_CHOICE_STRING_LEN];
    /* 0x132 */ char mailStr[MESSAGE_MAIL_STR_MAX][MESSAGE_MAIL_STR_LEN];
    /* 0x178 */ Color_RGBA8 nameTextColor;
    /* 0x178 */ Color_RGBA8 nameBackgroundColor;
    /* 0x17C */ Color_RGBA8 windowBackgroundColor;
    /* 0x182 */ Color_RGBA8 fontColor[MESSAGE_LINE_NUMBER];
    /* 0x194 */ Color_RGBA8 continueButtonColor;
    /* 0x198 */ f32 fontScaleX;
    /* 0x19C */ f32 fontScaleY;
    /* 0x1A0 */ s32 unk_1A0;
    /* 0x1A4 */ s32 unk_1A4;
    /* 0x1A8 */ s32 textLines;
    /* 0x1AC */ s32 nowDisplayLine;
    /* 0x1B0 */ Choice choiceWindow;
    /* 0x26C */ s32 unk26C;
    /* 0x270 */ u16 endTimer;
    /* 0x272 */ s16 animalVoiceIdx;
    /* 0x274 */ s32 voiceSfxIdx;
    /* 0x278 */ u8 voiceIdx;
    /* 0x279 */ s8 hideChoiceWindowTimer;
    /* 0x27C */ s32 spec;
    /* 0x280 */ u8 freeStrColorIdx[4];
    /* 0x284 */ UNK_TYPE1 unk_284[0x8];
    /* 0x28C */ u32 statusFlags;
    /* 0x290 */ f32 timer;
    /* 0x294 */ f32 cursolTimer;
    /* 0x298 */ f32 continueButtonTimer;
    /* 0x29C */ s32 startTextCursolIdx;
    /* 0x2A0 */ s32 endTextCursolIdx;
    /* 0x2A4 */ f32 windowScale;
    /* 0x2A8 */ f32 textScale;
    /* 0x2AC */ s32 requestedMainIndex;
    /* 0x2B0 */ s32 requestedPriority;
    /* 0x2B4 */ s32 mainIndex;
    /* 0x2B8 */ s32 drawFlag;
    /* 0x2BC */ s32 cancelFlag;
    /* 0x2C0 */ s32 cancelableFlag;
    /* 0x2C4 */ s32 continueMsgNo;
    /* 0x2C8 */ s32 continueCancelFlag;
    /* 0x2CC */ s32 forceNext;
    /* 0x2D0 */ s32 lockContinue;
    /* 0x2D4 */ u8 nowUtter;
    /* 0x2D8 */ MessageMainData mainData;
    /* 0x2E0 */ MessageRequestData requestData;
} MessageWindow; // size >= 0x26C

typedef enum MessageMainIndex {
    MESSAGE_MAIN_INDEX_HIDE,
    MESSAGE_MAIN_INDEX_APPEAR,
    MESSAGE_MAIN_INDEX_NORMAL,
    MESSAGE_MAIN_INDEX_CURSOL,
    MESSAGE_MAIN_INDEX_DISAPPEAR,
    MESSAGE_MAIN_INDEX_APPEAR_WAIT,
    MESSAGE_MAIN_INDEX_WAIT,
    MESSAGE_MAIN_INDEX_DISAPPEAR_WAIT,
    MESSAGE_MAIN_INDEX_MAX,
} MessageMainIndex;

typedef enum MessageStatus {
    MESSAGE_STATUS_NORMAL,
    MESSAGE_STATUS_ANGRY,
    MESSAGE_STATUS_SAD,
    MESSAGE_STATUS_FUN,
    MESSAGE_STATUS_SLEEPY,
    MESSAGE_STATUS_MAX
} MessageStatus;

extern MessageData mMsg_data;
extern MessageWindow mMsg_window;

extern Gfx con_kaiwa2_modelT[];
extern Gfx mMsg_init_disp[];
extern Gfx con_kaiwaname_modelT[];

extern s32 D_CF9000[];
extern u8 D_BD4000[];

MessageWindow* mMsg_Get_base_window_p(void);
s32 mMsg_Check_request_priority(MessageWindow* window, s32 priority);
s32 mMsg_Change_request_main_index(MessageWindow* window, s32 mainIndex, s32 priority);
s32 mMsg_Check_main_index(MessageWindow* window, s32 mainIndex);
s32 mMsg_Check_main_wait(MessageWindow* window);
int mMsg_Check_not_series_main_wait(MessageWindow* window);
s32 mMsg_Check_main_hide(MessageWindow* window);
void mMsg_Set_client_actor_p(MessageWindow* window, Actor* clientActor, s32 showName);
s32 mMsg_request_main_forceoff();
s32 mMsg_request_main_appear(MessageWindow* window, Actor* clientActor, s32 showName, Color_RGBA8* windowColor,
                             s32 msgNo, s32 prio);
s32 mMsg_request_main_disappear_wait(MessageWindow* window, s32 lastIndex, s32 prio);
s32 mMsg_request_main_disappear_wait_sub(MessageWindow* window, s32 lastIndex);
s32 mMsg_request_main_disappear_wait_type1(MessageWindow* window);
s32 mMsg_request_main_disappear_wait_type2(MessageWindow* window);
s32 mMsg_request_main_wait(MessageWindow* window, s32 lastIndex, s32 prio);
s32 mMsg_request_main_appear_wait(MessageWindow* window, s32 lastIndex, s32 setupFlag, s32 prio);
s32 mMsg_request_main_appear_wait_type2(MessageWindow* window, s32 flag);
s32 mMsg_request_main_appear_wait_type1(MessageWindow* window);
s32 mMsg_request_main_normal(MessageWindow* window, s32 waitFlag, s32 prio);
s32 mMsg_request_main_cursol(MessageWindow* window, s32 waitFlag, s32 prio);
void mMsg_Set_free_str(MessageWindow* window, s32 freeStrNo, char* str, s32 len);
void mMsg_Set_free_str_cl(MessageWindow* window, s32 freeStrNo, char* str, s32 len, s32 colorIdx);
void mMsg_Set_item_str(MessageWindow* window, s32 itemStrNo, char* str, s32 len);
void mMsg_Get_free_str(MessageWindow* window, s32 freeStrNo, char* str);
void mMsg_Get_item_str(MessageWindow* window, s32 itemStrNo, char* str);
void mMsg_Set_mail_str(MessageWindow* window, s32 mailStrNo, char* str, s32 len);
void mMsg_Set_continue_msg_num(MessageWindow* window, s32 msgNo);
s32 mMsg_Get_msg_num(MessageWindow* window);
int mMsg_Check_give_item();
s32 mMsg_Set_SizeCode(MessageWindow* window, s32 idx);
s32 mMsg_Count_SameCode(char* data, s32 start, s32 length, char code);
s32 mMsg_Check_LastCode_forData(char* data, s32 idx);
s32 mMsg_Check_LastCode(MessageWindow* window, s32 idx);
s32 mMsg_Check_ContinueCode_forData(char* data, s32 idx);
s32 mMsg_Check_ContinueCode(MessageWindow* window, s32 idx);
s32 mMsg_Check_NextIndex_ContinueCode(MessageWindow* window);
s32 mMsg_Check_NextIndex_LastCode(MessageWindow* window);
s32 mMsg_Check_NextIndex_SetSelectWindowCode(MessageWindow* window);
f32 mMsg_Get_CursolSetTimeCode_forData(char* data, s32 idx);
f32 mMsg_Get_CursolSetTimeCode(MessageWindow* window, s32 idx);
s32 mMsg_Get_ColorCode_forData(char* data, s32 idx, char* r, char* g, char* b);
s32 mMsg_Get_ColorCode(MessageWindow* window, s32 idx, char* r, char* g, char* b);
s32 mMsg_Get_OrderCode_forData(char* data, s32 idx, s32* orderIdx, u16* orderValue);
s32 mMsg_Get_OrderCode(MessageWindow* window, s32 idx, s32* orderIdx, u16* orderVal);
s32 mMsg_Get_SoundCutCode_forData(char* data, s32 idx);
s32 mMsg_Get_SoundCutCode(MessageWindow* window, s32 idx);
void mMsg_Get_bgm_make_forData(char* data, s32 idx, s32* bgmType, s32* stopType);
void mMsg_Get_bgm_make(MessageWindow* window, s32 idx, s32* bgmType, s32* stopTypee);
void mMsg_Get_bgm_delete_forData(char* data, s32 idx, s32* bgmType, s32* stopType);
void mMsg_Get_bgm_delete(MessageWindow* window, s32 idx, s32* bgmType, s32* stopTypee);
s32 mMsg_Get_MsgTimeEnd_time_forData(char* data, s32 idx);
s32 mMsg_Get_MsgTimeEnd_time(MessageWindow* window, s32 idx);
int mMsg_Check_MsgTimeEndCode_forData(char* data, s32 idx);
s32 mMsg_Check_MsgTimeEndCode(MessageWindow* window, s32 idx);
s32 mMsg_Check_NextIndex_MsgTimeEndCode(MessageWindow* window);
void mMsg_Get_sound_trg_sys_forData(char* data, s32 idx, s32* seNo);
void mMsg_Get_sound_trg_sys(MessageWindow* window, s32 idx, s32* seNo);
void mMsg_Set_LineFontColor(MessageWindow* window, s32 lineNo, char r, char g, char b, char a);
void mMsg_init_FontColor(MessageWindow* window);
void mMsg_init_NowDisplayLIne(MessageWindow* window);
void mMsg_Clear_CursolIndex(MessageWindow* window);
void mMsg_SetTimer(MessageWindow* window, f32 timer);
void mMsg_Get_BodyParam(s32 idx, u32* vramp, size_t* size);
s32 mMsg_Count_MsgData(char* data);
s32 mMsg_LoadMsgData(MessageData* data, s32 idx, s32 cut);
s32 mMsg_ChangeMsgData(MessageWindow* window, s32 idx);
void mMsg_Unset_NowUtter(MessageWindow* window);
void mMsg_Set_NowUtter(MessageWindow* window);
void mMsg_init(Game_Play* play);
s32 mMsg_Get_Length_String(char* str, s32 len);
s32 mMsg_Check_MainNormalContinue(MessageWindow* window);
s32 mMsg_Check_MainNormal(MessageWindow* window);
s32 mMsg_Check_MainHide(MessageWindow* window);
int mMsg_Check_MainDisappear(MessageWindow* window);
void mMsg_Set_CancelNormalContinue(MessageWindow* window);
void mMsg_Unset_CancelNormalContinue(MessageWindow* window);
void mMsg_Set_ForceNext(MessageWindow* window);
void mMsg_Unset_ForceNext(MessageWindow* window);
s32 mMsg_Get_LockContinue(MessageWindow* window);
void mMsg_Set_LockContinue(MessageWindow* window);
void mMsg_Unset_LockContinue(MessageWindow* window);
void mMsg_Set_idling_req(MessageWindow* window);
s32 mMsg_Check_idling_now(MessageWindow* window);
s32 mMsg_MoveDataCut(char* data, s32 dstIdx, s32 srcIdx, s32 len, s32 spaceFlag);
void mMsg_CopyString(char* dst, char* src, s32 len);
s32 mMsg_Set_PlayerNameColor(char* data, s32* startIdx, s32 len);
s32 mMsg_CopyPlayerName(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyTalkName(Actor* actor, char* data, s32 startIdx, s32 len);
s32 mMsg_CopyTail(Actor* actor, char* data, s32 startIdx, s32 len);
s32 mMsg_CopyYear(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyMonth(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyWeek(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyDay(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyHour(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyMin(char* data, s32 startIdx, s32 len);
s32 mMsg_CopySec(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyFree(MessageWindow* window, s32 strNo, char* data, s32 startIdx, s32 len);
s32 mMsg_Set_PfColor(char* data, s32* startIdx, s32 len, char* freeStr, s32 fcColorId);
s32 mMsg_CopyDetermination(MessageWindow* window, char* data, s32 startIdx, s32 len);
s32 mMsg_CopyCountryName(char* data, s32 startIdx, s32 len);
s32 mMsg_CopyRamdomNumber2(u8* data, s32 startIdx, s32 len);
s32 mMsg_CopyItem(MessageWindow* window, s32 strNo, char* data, s32 startIdx, s32 len);
s32 mMsg_CopyMail(MessageWindow* window, s32 strNo, char* data, s32 startIdx, s32 len);
s32 mMsg_sound_voice_get(s32 code);
s32 mMsg_sound_voice_get2(s32 code);
int mMsg_check_sound_special(MessageWindow* window);
int mMsg_check_sound_animal(MessageWindow* window);
s32 mMsg_sound_npc_id_get(MessageWindow* window);
void mMsg_sound_voice_entry(MessageWindow* window, s32 voiceSfxIdx, u8 voiceIdx, s16 animalVoiceIdx);
void mMsg_sound_voice_endcode_set(MessageWindow* window);
int mMsg_sound_CodeVoice(MessageWindow* window, s32 idx);
void mMsg_sound_PAGE_OKURI();
void mMsg_sound_ZOOMUP();
void mMsg_sound_ZOOMDOWN_SHORT();
void mMsg_sound_ZOOMDOWN_LONG();
void mMsg_sound_MessageSpeedForce(f32 timer);
void mMsg_sound_MessageSpeedClear();
void mMsg_sound_MessageStatus(u8 status);
void mMsg_sound_bgm_make(s32 bgmNum, s32 stopType);
void mMsg_sound_bgm_delete(s32 bgmNum, s32 stopType);
void mMsg_sound_sound_trg_sys(s32 seNum);
u8 mMsg_sound_voice_mode_get(MessageWindow* window);
void mMsg_sound_voice_mode(MessageWindow* window);
void mMsg_sound_spec_change_voice_force(MessageWindow* window);
s32 mMsg_sound_spec_change_voice(MessageWindow* window);
void mMsg_sound_spec_change_scene(MessageWindow* window);
void mMsg_sound_spec_change_true(MessageWindow* window);
void mMsg_sound_spec_change_false(MessageWindow* window);
void mMsg_Main_Hide(MessageWindow* window, Game_Play* play);
void mMsg_MainSetup_Hide(MessageWindow* window, Game_Play* play);
s32 mMsg_Main_Appear_SetScale(MessageWindow* window, Game_Play* play);
void mMsg_request_main_index_fromAppear(MessageWindow* window, Game_Play* play, s32 scaleDownFlag);
void mMsg_Main_Appear(MessageWindow* window, Game_Play* play);
void mMsg_MainSetup_Appear(MessageWindow* window, Game_Play* play);
int mMsg_Check_ScrollOrder(MessageWindow* window);
s32 mMsg_MsgTimeEnd_dec(MessageWindow* window);
void mMsg_end_to_disappear(MessageWindow* window);
void mMsg_request_main_index_fromNormal(MessageWindow* window, Game_Play* play);
void mMsg_Set_display_button_turn_color(MessageWindow* window, Game_Play* play);
void mMsg_Main_Normal(MessageWindow* window, Game_Play* play);
void mMsg_MainSetup_Normal(MessageWindow* window, Game_Play* play);
s32 mMsg_Check_CancelOrder(MessageWindow* window);
s32 mMsg_Main_Cursol_Check_ControlCursol(MessageWindow* window, s32 idx);
s32 mMsg_Main_Cursol_Last_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_Continue_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_Clear_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_CursolSetTime_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_Button_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_Color_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_AbleCancel_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_UnableCancel_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetDemoOrder_ControlCursol(MessageWindow* window, s32* idxPtr, DemoOrderType demoType);
s32 mMsg_Main_Cursol_SetDemoOrderPlayer_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetDemoOrderNpc0_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetDemoOrderNpc1_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetDemoOrderNpc2_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetDemoOrderQuest_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetSelectWindow_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessage_ControlCursol(MessageWindow* window, s32* idxPtr, s32 selectNo);
s32 mMsg_Main_Cursol_SetNextMessageF_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessage0_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessage1_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessage2_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessage3_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessageRamdomCommon_ControlCursol(MessageWindow* window, s32* idxPtr, s32 max);
s32 mMsg_Main_Cursol_SetNextMessageRamdom2_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessageRamdom3_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetNextMessageRamdom4_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetSelectString_ControlCursol(MessageWindow* window, s32* idxPtr, s32 selectCount);
s32 mMsg_Main_Cursol_SetSelectString2_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetSelectString3_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetSelectString4_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_SetForceNext_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_PlayerName_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_TalkName_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Tail_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Year_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Month_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Week_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Day_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Hour_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Min_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Sec_ControlCursol(MessageWindow* window, s32* idxPtr);
s32 mMsg_Main_Cursol_PutString_Free(MessageWindow* window, s32 idx, s32 freeStrNo);

void mMsg_MainSetup_Window(MessageWindow* window, Game_Play* play);

#endif
