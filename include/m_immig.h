#ifndef M_IMMIG_H
#define M_IMMIG_H

#include "ultra64.h"
#include "unk.h"
#include "m_private.h"

typedef struct mImm_KeepTail_c {
    /* 0x0 */ u16 a;
    /* 0x2 */ u8 b;
} mImm_KeepTail_c; // size = 0x4

typedef struct mImm_Keep_c {
    /* 0x000 */ u16 pockets[mPr_POCKETS_SLOT_COUNT];
    /* 0x01E */ u8 lottoTicketExpiryMonth;
    /* 0x01F */ u8 lottoTicketMailStorage;
    /* 0x020 */ u32 itemConditions;
    /* 0x024 */ u32 wallet;
    /* 0x028 */ mQst_delivery_c deliveries[mPr_DELIVERY_QUEST_NUM];
    /* 0x244 */ QuestErrand errands[mPr_ERRAND_QUEST_NUM];
    /* 0x3D4 */ Mail_c mail[mPr_INVENTORY_MAIL_COUNT];
    /* 0xA3C */ mImm_KeepTail_c tail[5];
} mImm_Keep_c; // size = 0xA50

void mImm_GetLastRegistPlayerName(char* playerName);
void mImm_ClearResetCode(void);
void mImm_SetResetCode(PrivateInfo* private);
s32 mImm_CheckResetCode(PrivateInfo* private);
void mImm_SetResetInfo(PrivateInfo* private);
void mImm_InitGameStart_init(void);
void mImm_InitGameStart_cleanup(u32** buffer);
s32 mImm_InitGameStart_data_init(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_before_save(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_main_init(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_checksum(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_main(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_pak(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_backup(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_load_back(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_re_main(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart_after(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode);
s32 mImm_InitGameStart(s32 size, s32 unused, s32 combination, s32 playerNo);
void mImm_SaveMode_init(void);
void mImm_CopyKeep(u16* dstPockets, u8* dstLottoMonth, u8* dstLottoMail, u32* dstItemCond, u32* dstWallet,
                   mQst_delivery_c* dstDeliveries, QuestErrand* dstErrands, Mail_c* dstMail, mImm_KeepTail_c* dstTail,
                   u16* srcPockets, u8 srcLottoMonth, u8 srcLottoMail, u32 srcItemCond, u32 srcWallet,
                   mQst_delivery_c* srcDeliveries, QuestErrand* srcErrands, Mail_c* srcMail, mImm_KeepTail_c* srcTail);
void mImm_ClearPrivateItem(PrivateInfo* private, mImm_Keep_c* keep);
void mImm_ReplaceKeep(PrivateInfo* private, void* keepInfo);
s32 mImm_Save_init(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_main_init(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_set_checksum(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_save_main(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_pak(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_backup(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_backup_load(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_re_main(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_after(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer);
s32 mImm_Save_common(s32 arg0, s32 arg1, s32 arg2);
void mImm_SaveTest(void);
s32 mImm_CheckStartDataCategory(void);

#endif
