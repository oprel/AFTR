#include "m_immig.h"
#include "global.h"

#include "m_common_data.h"
#include "m_cpak.h"
#include "m_flashrom.h"
#include "m_lib.h"
#include "m_malloc.h"
#include "m_nmi_buf.h"
#include "m_npc.h"
#include "m_private.h"
#include "sys_math.h"
#include "m_start_data_init.h"
#include "m_cockroach.h"

typedef s32 (*mImm_INIT_PROC)(s32, s32, s32, u32**, s32*);
typedef s32 (*mImm_SAVE_PROC)(s32, s32, PrivateInfo*, void*, s32*, u32**);

extern s32 mImm_start_mode;
extern s32 mImm_save_mode;
extern s32 mImm_start_combination;
extern s32 mImm_start_dt_ct;
extern s32 mImm_malloc_time;
extern char mImm_GetLastRegistPlayerName_debug_player_name[6];
extern mImm_INIT_PROC mImm_InitGameStart_init_proc[10];
extern u32* mImm_InitGameStart_buffer_p;
extern s32 mImm_InitGameStart_pak_mode;
extern mImm_SAVE_PROC mImm_Save_common_save_proc[9];
extern s32 mImm_Save_common_pak_mode;
extern u32* mImm_Save_common_buffer_p;
extern PrivateInfo mImm_Save_common_keep_private;
extern u8 mImm_Save_common_keep_info[sizeof(mImm_Keep_c)];

void mImm_GetLastRegistPlayerName(char* playerName) {
    mPr_CopyPlayerName(playerName, mImm_GetLastRegistPlayerName_debug_player_name);
}

void mImm_ClearResetCode(void) {
    PrivateInfo* private = common_data.privateInfo;

    bzero(&private->resetCode, sizeof(u32));
}

void mImm_SetResetCode(PrivateInfo* private) {
    private->resetCode = (u32)(fqrand() * 65535.0f) + 1;
}

s32 mImm_CheckResetCode(PrivateInfo* private) {
    s32 ret = TRUE;

    if (private->resetCode != 0) {
        ret = FALSE;
    }

    return ret;
}

void mImm_SetResetInfo(PrivateInfo* private) {
    if ((mImm_CheckResetCode(private) == FALSE) && (mFRm_GetRepair() == FALSE) && ((APPNMI_GETVAL() & 2) == 0)) {
        common_data.unk_10A68 = 1;
        private->resetCount++;
    }
}

void mImm_InitGameStart_init(void) {
    mImm_start_mode = 0;
    mImm_malloc_time = 0;
}

void mImm_InitGameStart_cleanup(u32** buffer) {
    mFRm_ClearRepair();
    mImm_InitGameStart_init();

    if (*buffer != NULL) {
        func_800D986C_jp(*buffer);
    }
}

s32 mImm_InitGameStart_data_init(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    Game* game = gamePT;
    u32* buf;
    s32 ret;
    s32 flag;
    s32 initMode = 0;

    *buffer = NULL;
    if (mImm_malloc_time < 150) {
        ret = 0;
        flag = 0;
        buffer = func_800D97A0_jp(mFRm_SAVE_SIZE);
        *buffer = buffer;
        if (buf != NULL) {
            switch (size) {
                case 0:
                    mImm_start_mode = 10;
                    flag = 1;
                    break;
                case 1:
                    if (mPr_NullCheckPersonalID(&common_data.save.saveFilePrivateInfo[playerNo].playerId) == TRUE) {
                        flag = 1;
                        initMode = 1;
                        mImm_start_mode = 10;
                    } else {
                        initMode = 2;
                    }
                    break;
                case 2:
                    mImm_start_mode = 10;
                    flag = 1;
                    break;
                case 3:
                    initMode = 3;
                    break;
            }
            ret = mSDI_StartDataInit(game, playerNo, initMode);
            if (flag == 0 && ret == 1) {
                ret = 0;
            }
            mImm_malloc_time = 0;
            mImm_start_mode = 1;
        } else {
            mImm_malloc_time++;
        }
    } else {
        ret = 1;
        mImm_malloc_time = 0;
    }

    mImm_start_combination = combination;
    mImm_start_dt_ct = size;
    return ret;
}

s32 mImm_InitGameStart_before_save(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    PakInfo* pkinfo = mCPk_get_pkinfo();
    PrivateInfo* private;
    s32 result;
    s32 ret = 0;

    *mode = 1;
    if (*buffer == NULL) {
        return 1;
    }

    switch (size) {
        case 1:
            private = &common_data.save.saveFilePrivateInfo[playerNo];
            if (mPr_NullCheckPersonalID(&private->playerId) == FALSE) {
                mImm_SetResetInfo(private);
                mImm_SetResetCode(private);
                if (common_data.playerDecoyFlag == 1) {
                    private->exists = 0;
                }
            } else {
                ret = 1;
            }
            break;
        case 3:
            result = mCPk_PakOpen(pkinfo, 0);
            if (result == 1) {
                if (mLd_PlayerManKindCheckNo(playerNo) == 0) {
                    private = common_data.privateInfo;
                    if (private != NULL) {
                        mImm_SetResetInfo(private);
                        mImm_SetResetCode(private);
                    }
                    *mode = 0;
                } else {
                    private = mCPk_GetSaveFilePrivateP();
                    if (private != NULL) {
                        mImm_SetResetInfo(private);
                        mImm_SetResetCode(private);
                    }
                    private = common_data.privateInfo;
                    if (private != NULL) {
                        mImm_SetResetInfo(private);
                        mImm_SetResetCode(private);
                    }
                    result = mCPk_SetPakCopyProtect(pkinfo);
                }
                if (result == 0) {
                    ret = -2;
                }
            } else {
                ret = -1;
            }
            break;
        default:
            ret = 1;
            break;
    }

    mImm_start_mode = 2;
    return ret;
}

s32 mImm_InitGameStart_main_init(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    mFRm_ClearRepair();
    if (*buffer != NULL) {
        bcopy(&common_data, *buffer, mFRm_SAVE_SIZE);
    }

    mImm_start_mode = 3;
    return 0;
}

s32 mImm_InitGameStart_checksum(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    mFRm_chk_c* saveP = (mFRm_chk_c*)*buffer;
    u32* buf = *buffer;
    s32 ret = 0;

    if (buf != NULL) {
        saveP->version = 2;
        mFRm_SetSaveCheckData(saveP);
        saveP->checkSum = mFRm_GetFlatCheckSum(saveP, mFRm_SAVE_SIZE, saveP->checkSum);
    } else {
        ret = 1;
    }

    mImm_start_mode = 4;
    return ret;
}

s32 mImm_InitGameStart_main(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 0);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_start_mode = 5;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_InitGameStart_pak(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    s32 result;
    s32 ret = 0;

    result = 1;
    if (*mode == 0) {
        result = mCPk_EraseForestData();
    }

    if (result != 1) {
        mImm_start_mode = 7;
    } else {
        mImm_start_mode = 6;
    }

    return ret;
}

s32 mImm_InitGameStart_backup(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 1);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_start_mode = 9;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_InitGameStart_load_back(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    s32 ret = 0;

    if (mFRm_FlashLoadAllData_common(*buffer, mFRm_SAVE_PAGE_NUM) == 1) {
        if (mFRm_ReturnCheckSum((u16*)*buffer, mFRm_SAVE_SIZE) == 0) {
            if (mFRm_CheckSaveData_ID((mFRm_chk_c*)*buffer) == 1) {
                mFRm_ClearRepair();
                mImm_start_mode = 8;
            } else {
                ret = 1;
            }
        } else {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_InitGameStart_re_main(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 0);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_start_mode = 0;
            ret = -2;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_InitGameStart_after(s32 size, s32 combination, s32 playerNo, u32** buffer, s32* mode) {
    PrivateInfo* private = common_data.privateInfo;

    if ((common_data.playerDecoyFlag == 1) && (private != NULL)) {
        if (mPr_NullCheckPersonalID(&private->playerId) == FALSE) {
            private->exists = 1;
            private->unk_3EC = 0;
        }
    }

    mImm_start_mode = 0;
    return 1;
}

s32 mImm_InitGameStart(s32 size, s32 unused, s32 combination, s32 playerNo) {
    s32 ret;

    if ((mImm_start_mode < 0) || (mImm_start_mode >= 10)) {
        return 1;
    }

    ret = mImm_InitGameStart_init_proc[mImm_start_mode](size, 
        combination, playerNo, &mImm_InitGameStart_buffer_p, &mImm_InitGameStart_pak_mode);

    if (mImm_start_mode >= 10) {
        mImm_start_mode = 0;
        ret = 1;
    }

    if (ret != 0) {
        mImm_InitGameStart_cleanup(&mImm_InitGameStart_buffer_p);
        mImm_InitGameStart_buffer_p = NULL;
        mImm_InitGameStart_pak_mode = 1;
    }

    return ret;
}

void mImm_SaveMode_init(void) {
    mImm_save_mode = 0;
}

void mImm_CopyKeep(u16* dstPockets, u8* dstLottoMonth, u8* dstLottoMail, u32* dstItemCond, u32* dstWallet,
                   mQst_delivery_c* dstDeliveries, QuestErrand* dstErrands, Mail_c* dstMail, mImm_KeepTail_c* dstTail,
                   u16* srcPockets, u8 srcLottoMonth, u8 srcLottoMail, u32 srcItemCond, u32 srcWallet,
                   mQst_delivery_c* srcDeliveries, QuestErrand* srcErrands, Mail_c* srcMail, mImm_KeepTail_c* srcTail) {
    s32 i;

    bcopy(srcPockets, dstPockets, sizeof(u16) * mPr_POCKETS_SLOT_COUNT);
    *dstItemCond = srcItemCond;
    *dstLottoMonth = srcLottoMonth;
    *dstLottoMail = srcLottoMail;
    *dstWallet = srcWallet;

    for (i = 0; i < mPr_DELIVERY_QUEST_NUM; i++) {
        func_800BA8A0_jp(dstDeliveries, srcDeliveries);
        dstDeliveries++;
        srcDeliveries++;
    }
    for (i = 0; i < mPr_ERRAND_QUEST_NUM; i++) {
        func_800BA8F4_jp(dstErrands, srcErrands);
        dstErrands++;
        srcErrands++;
    }
    for (i = 0; i < mPr_INVENTORY_MAIL_COUNT; i++) {
        mMl_copy_mail(dstMail, srcMail);
        dstMail++;
        srcMail++;
    }
    for (i = 0; i < 5; i++) {
        dstTail->a = srcTail->a;
        dstTail->b = srcTail->b;
        dstTail++;
        srcTail++;
    }
}

void mImm_ClearPrivateItem(PrivateInfo* private, mImm_Keep_c* keep) {
    if (keep != NULL) {
        bzero(keep, sizeof(mImm_Keep_c));
        mImm_CopyKeep(keep->pockets, &keep->lottoTicketExpiryMonth, &keep->lottoTicketMailStorage,
                      &keep->itemConditions, &keep->wallet, keep->deliveries, keep->errands, keep->mail, keep->tail,
                      private->inventory.pockets, private->inventory.lotto_ticket_expiry_month,
                      private->inventory.lotto_ticket_mail_storage, private->inventory.item_conditions,
                      private->inventory.wallet, private->deliveries, private->errands, private->mail,
                      (mImm_KeepTail_c*)private->unk_A94);
    }

    bzero(private->inventory.pockets, sizeof(u16) * mPr_POCKETS_SLOT_COUNT);
    private->inventory.item_conditions = 0;
    private->inventory.lotto_ticket_expiry_month = 0;
    private->inventory.lotto_ticket_mail_storage = 0;
    private->inventory.wallet = 0;
    mQst_ClearDelivery(private->deliveries, mPr_DELIVERY_QUEST_NUM);
    mQst_ClearErrand(private->errands, mPr_ERRAND_QUEST_NUM);
    mMl_clear_mail_box(private->mail, mPr_INVENTORY_MAIL_COUNT);
    bzero(private->unk_A94, 20);
}

void mImm_ReplaceKeep(PrivateInfo* private, void* keepInfo) {
    mImm_Keep_c* keep = (mImm_Keep_c*)keepInfo;

    if (keep != NULL) {
        mImm_CopyKeep(private->inventory.pockets, &private->inventory.lotto_ticket_expiry_month,
                      &private->inventory.lotto_ticket_mail_storage, &private->inventory.item_conditions,
                      &private->inventory.wallet, private->deliveries, private->errands, private->mail,
                      (mImm_KeepTail_c*)private->unk_A94, keep->pockets, keep->lottoTicketExpiryMonth,
                      keep->lottoTicketMailStorage, keep->itemConditions, keep->wallet, keep->deliveries, keep->errands,
                      keep->mail, keep->tail);
    }
}

s32 mImm_Save_init(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    u32* buf;
    PrivateInfo* private;
    s32 result;
    PakInfo* pkinfo;
    s32 ret = 0;

    *mode = 1;
    *buffer = NULL;
    if (mImm_malloc_time < 150) {
        buf = func_800D97A0_jp(mFRm_SAVE_SIZE);
        *buffer = buf;
        if (buf != NULL) {
            mCkRh_SavePlayTime(common_data.playerNumber);
            switch (arg0) {
                case 1:
                    if (arg1 != 1) {
                        mImm_ClearResetCode();
                    } else {
                        private = common_data.privateInfo;
                        if (mImm_CheckResetCode(private) == 1) {
                            mImm_SetResetCode(private);
                        }
                    }
                    if (mImm_CheckStartDataCategory() == 1) {
                        mNpc_GetRemoveAnimal(mNpc_GetInAnimalP(), 0);
                    }
                    break;
                case 3:
                    if (arg1 != 1) {
                        mImm_ClearResetCode();
                    }
                    pkinfo = mCPk_get_pkinfo();
                    result = mCPk_PakOpen(pkinfo, 0);
                    if (result == 1) {
                        private = common_data.privateInfo;
                        mNpc_GetRemoveAnimal(mNpc_GetInAnimalP(), 1);
                        *mode = 0;
                        mPr_CopyPrivateInfo(keepPrivate, private);
                        if (mLd_PlayerManKindCheckNo(common_data.playerNumber) == 0) {
                            private->exists = 0;
                            mImm_ClearPrivateItem(private, keepInfo);
                        }
                    } else {
                        ret = -1;
                    }
                    break;
                default:
                    ret = -1;
                    break;
            }
            mImm_malloc_time = 0;
            mImm_save_mode = 1;
        } else {
            mImm_malloc_time++;
        }
    } else {
        ret = 1;
        mImm_malloc_time = 0;
    }

    return ret;
}

s32 mImm_Save_main_init(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    s32 ret = 0;

    mFRm_ClearRepair();
    if (*buffer != NULL) {
        bcopy(&common_data, *buffer, mFRm_SAVE_SIZE);
    } else {
        ret = 1;
    }

    mImm_save_mode = 2;
    return ret;
}

s32 mImm_Save_set_checksum(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    mFRm_chk_c* saveP = (mFRm_chk_c*)*buffer;
    u32* buff = *buffer;
    s32 ret = 0;

    if (buff != NULL) {
        saveP->version = 2;
        mFRm_SetSaveCheckData(saveP);
        saveP->checkSum = mFRm_GetFlatCheckSum(saveP, mFRm_SAVE_SIZE, saveP->checkSum);
    } else {
        ret = 1;
    }

    mImm_save_mode = 3;
    return ret;
}

s32 mImm_Save_save_main(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 0);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_save_mode = 4;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_Save_pak(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    s32 result;
    s32 ret = 0;

    result = 1;
    if (*mode == 0) {
        result = mCPk_SavePak(keepPrivate, mNpc_GetInAnimalP(), mCPk_get_pkinfo());
    }

    if (result != 1) {
        mImm_save_mode = 6;
    } else {
        mImm_save_mode = 5;
    }

    return ret;
}

s32 mImm_Save_backup(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 1);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_save_mode = 8;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_Save_backup_load(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    PrivateInfo* private;
    s32 ret = 0;

    if (mFRm_FlashLoadAllData_common(*buffer, mFRm_SAVE_PAGE_NUM) == 1) {
        if (mFRm_ReturnCheckSum((u16*)*buffer, mFRm_SAVE_SIZE) == 0) {
            if (mFRm_CheckSaveData_ID((mFRm_chk_c*)*buffer) == 1) {
                mFRm_ClearRepair();
                mImm_save_mode = 7;
            } else {
                ret = 1;
            }
        } else {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    private = common_data.privateInfo;
    if (private != NULL) {
        if (mLd_PlayerManKindCheckNo(common_data.playerNumber) == 0) {
            private->exists = 1;
            mImm_ReplaceKeep(private, keepInfo);
        }
    }

    return ret;
}

s32 mImm_Save_re_main(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    s32 result;
    s32 ret = 1;

    if (*buffer != NULL) {
        ret = 0;
        result = mFRm_WriteHalfFlash_bg(*buffer, 0);
        if (result == 1) {
            mFRm_ClearRepair();
            mImm_save_mode = 0;
            ret = -2;
        } else if (result == -1) {
            ret = 1;
        }
    } else {
        ret = 1;
    }

    return ret;
}

s32 mImm_Save_after(s32 arg0, s32 arg1, PrivateInfo* keepPrivate, void* keepInfo, s32* mode, u32** buffer) {
    PrivateInfo* private = common_data.privateInfo;

    if (arg1 == 1) {
        if ((common_data.playerDecoyFlag == 1) && (private != NULL)) {
            private->exists = 1;
        }
    } else {
        common_data.playerDecoyFlag = 0;
    }

    return 1;
}

s32 mImm_Save_common(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    if ((mImm_save_mode < 0) || (mImm_save_mode >= 9)) {
        mImm_save_mode = 8;
    }

    ret = mImm_Save_common_save_proc[mImm_save_mode](arg0, arg1 ? arg2 : arg2, &mImm_Save_common_keep_private,
                                                       mImm_Save_common_keep_info, &mImm_Save_common_pak_mode,
                                                       &mImm_Save_common_buffer_p);
    if (ret != 0) {
        mFRm_ClearRepair();
        mPr_ClearPrivateInfo(&mImm_Save_common_keep_private);
        bzero(mImm_Save_common_keep_info, sizeof(mImm_Save_common_keep_info));
        mImm_Save_common_pak_mode = 1;
        mImm_save_mode = 0;
        if (mImm_Save_common_buffer_p != NULL) {
            func_800D986C_jp(mImm_Save_common_buffer_p);
        }
        mImm_Save_common_buffer_p = NULL;
    }

    return ret;
}

void mImm_SaveTest(void) {
}

s32 mImm_CheckStartDataCategory(void) {
    s32 ret = FALSE;

    if (mImm_start_dt_ct == 3) {
        ret = TRUE;
    }

    return ret;
}
