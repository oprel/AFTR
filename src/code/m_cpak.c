#include "m_cpak.h"
#include "global.h"

#include "m_flashrom.h"
#include "m_lib.h"
#include "m_malloc.h"

#include "padmgr.h"

#include "m_immig.h"
#include "s_cpak.h"

PakInfo l_paks_info;
PakCtrl l_pak_ctrl;

u16 sCompanyCode = 0x3031;
u32 sGameCode = 0x4E41464A;
char* l_game_name = D_8010EF70_jp;
char* l_ext_name_table[2] = { "\x1A", "\x1B" };
PakInfo* g_paks_info_p = &l_paks_info;
u8 mCPk_dummyName[8] = { 0xD4, 0x8E, 0xA6, 0x90, 0x85, 0x42, 0x00, 0x00 };

const size_t sFileSizes[2] = { sizeof(Passport), sizeof(D801047B0Struct) };

s32 mCPk_PakOpen(PakInfo* info, s32 channel) {
    return sCPk_PakOpen(&info->pfsInfo, channel);
}

s32 mCPk_NoteMake(PakInfo* info) {
    return sCPk_MakeFile(&info->pfsInfo, &info->pfsState, info->pfsState.file_size);
}

s32 mCPk_NoteOpen(PakInfo* info) {
    return sCPk_FileOpen(&info->pfsInfo, &info->pfsState);
}

s32 mCPk_NoteNum(PakInfo* info) {
    return sCPk_FileNum(&info->pfsInfo, &info->maxFiles, &info->filesUsed);
}

s32 mCPk_AllFileState(PakInfo* info) {
    OSPfsInfo* pfsInfo = &info->pfsInfo;
    OSPfsState* pfsStates = info->unk_94;
    s32* var_s1 = info->unk_294;
    s32 i;
    u32 file_size;

    for (i = 0; i < ARRAY_COUNT(info->unk_94); i++) {
        var_s1[i] = 0;
        pfsInfo->fileNo = i;
        file_size = sCPk_FileState(pfsInfo, &pfsStates[i]);
        if (file_size == 0) {
            if (pfsInfo->err == PFS_ERR_INVALID) {
                var_s1[i] = -1;
                continue;
            }

            return file_size;
        }
    }

    return file_size;
}

s32 mCPk_FreeBlockNum(PakInfo* info) {
    s32 ret = FALSE;

    info->freeBytes = sCPk_PakFreeArea(&info->pfsInfo);
    if (info->pfsInfo.err == 0) {
        ret = TRUE;
    }
    return ret;
}

s32 mCPk_CheckNoteRest(PakInfo* info) {
    s32 ret = FALSE;

    if ((mCPk_FreeBlockNum(info) == TRUE) && (info->freeBytes >= info->pfsState.file_size)) {
        ret = TRUE;
    }
    return ret;
}

void mCPk_ClearPassport(Passport* passport) {
    passport->unk_0000 = 0;
    mPr_ClearPrivateInfo(&passport->priv);
    mNpc_ClearAnimalInfo(&passport->animal);
    passport->unk_1100 = 0xFFFF;
}

s32 mCPk_InitPak(s32 channel) {
    PakInfo* info = mCPk_get_pkinfo();
    OSPfsState* pfsState;
    OSPfsState* iter;
    s32 i;
    s32 ret;

    info->unk_00 = 0;

    pfsState = &info->pfsState;
    bzero(pfsState, sizeof(OSPfsState));

    iter = info->unk_94;
    for (i = 0; i < ARRAY_COUNT(info->unk_94); i++) {
        bzero(iter, sizeof(OSPfsState));
        iter++;
    }

    sCPk_SetCode(pfsState, &sCompanyCode, &sGameCode);

    bcopy(l_game_name, &pfsState->game_name, sizeof(pfsState->game_name));

    ret = mCPk_PakOpen(info, channel);

    mCPk_ClearPassport(&l_pak_ctrl.passport);

    l_pak_ctrl.queue = NULL;
    l_pak_ctrl.unk_1204 = 0;
    l_pak_ctrl.unk_1208 = 0xFFFF;

    return ret;
}

void mCPk_SetForestPakState(PakInfo* info, u8 state) {
    OSPfsState* pfsState = &info->pfsState;

    if (state < ARRAY_COUNT(l_ext_name_table)) {
        bcopy(l_ext_name_table[state], pfsState->ext_name, sizeof(pfsState->ext_name));
        pfsState->file_size = sFileSizes[state];
    }
}

s32 mCPk_Save(PakInfo* info, void* buffer) {
    s32 ret;
    s32 i;
    u32 file_size;

    for (i = 0; i < 1; i++) {
        OSPfsInfo* pfsInfo = &info->pfsInfo;

        l_pak_ctrl.queue = padmgr_LockSerialMesgQ();
        ret = mCPk_NoteOpen(info);

        if (pfsInfo->err == PFS_ERR_INVALID) {
            ret = mCPk_NoteMake(info);
        }

        if ((ret == 1) || (pfsInfo->err == PFS_ERR_EXIST)) {
            file_size = info->pfsState.file_size;
            ret = sCPk_Save(pfsInfo, 0, file_size, buffer);
        }
        padmgr_UnlockSerialMesgQ(l_pak_ctrl.queue);
    }

    return ret;
}

s32 mCPk_Load(PakInfo* info, void* buffer) {
    s32 ret;
    u32 file_size;

    l_pak_ctrl.queue = padmgr_LockSerialMesgQ();
    ret = mCPk_NoteOpen(info);

    if (ret == 1) {
        file_size = info->pfsState.file_size;
        ret = sCPk_Load(&info->pfsInfo, 0, file_size, buffer);
        if (ret == 0) {
            ret = -1;
        }
    }

    padmgr_UnlockSerialMesgQ(l_pak_ctrl.queue);

    return ret;
}

void mCPk_SetPrivateSavefile(PrivateInfo* priv) {
    if (priv != NULL) {
        mem_copy((void*)&l_pak_ctrl.passport.priv, (void*)priv, sizeof(PrivateInfo));
    }
}

PrivateInfo* mCPk_GetSaveFilePrivateP(void) {
    return &l_pak_ctrl.passport.priv;
}

s32 mCPk_SavePak(PrivateInfo* priv, Animal_c* animal, PakInfo* info) {
    mCPk_SetForestPakState(info, 0);
    mCPk_SetPrivateSavefile(priv);
    mem_copy((void*)&l_pak_ctrl.passport.animal, (void*)animal, sizeof(Animal_c));
    l_pak_ctrl.passport.unk_0000 =
        mFRm_GetFlatCheckSum(&l_pak_ctrl.passport, sizeof(Passport), l_pak_ctrl.passport.unk_0000);

    return mCPk_Save(info, &l_pak_ctrl.passport);
}

s32 mCPk_PakPrivateLoad(PrivateInfo* priv, Animal_c* animal, PakInfo* info) {
    s32 ret;

    mCPk_SetForestPakState(info, 0);
    ret = mCPk_Load(info, &l_pak_ctrl.passport);

    if (ret == 1) {
        l_pak_ctrl.unk_1204 = 1;
    }

    if (ret == 1) {
        if (mPr_CheckPrivate(&l_pak_ctrl.passport.priv) == TRUE) {
            mem_copy((void*)priv, (void*)&l_pak_ctrl.passport.priv, sizeof(PrivateInfo));
            mem_copy((void*)animal, (void*)&l_pak_ctrl.passport.animal, sizeof(Animal_c));
        } else {
            ret = 0;
        }
    } else {
        ret = 0;
    }

    return ret;
}

s32 mCPk_CheckData_common(s32* arg0, s32 arg1, PakInfo* info, void* arg3) {
    OSPfsInfo* pfsInfo = &info->pfsInfo;
    void* buffer;
    s32 ret = FALSE;
    UNUSED s32 pad[2];

    *arg0 = 5;
    l_pak_ctrl.queue = padmgr_LockSerialMesgQ();
    if ((mCPk_NoteOpen(info) == 1) || (pfsInfo->err == PFS_ERR_EXIST)) {
        buffer = (arg1 == 0) ? &l_pak_ctrl.passport : arg3;
        if ((buffer != NULL) && (sCPk_Load(pfsInfo, 0, info->pfsState.file_size, buffer) == 1)) {
            if (arg1 == 0) {
                if (!mPr_NullCheckPersonalID(&l_pak_ctrl.passport.priv.playerId)) {
                    l_pak_ctrl.unk_1204 = 1;
                    *arg0 = 0;
                } else {
                    *arg0 = 2;
                }
            } else {
                *arg0 = 2;
            }
            ret = TRUE;
        }
    } else if (pfsInfo->err == PFS_ERR_INVALID) {
        if ((mCPk_NoteNum(info) == 1) && (info->filesUsed >= 0x10)) {
            *arg0 = 4;
            ret = TRUE;
        } else if (mCPk_CheckNoteRest(info) == TRUE) {
            *arg0 = 1;
            ret = TRUE;
        } else if (pfsInfo->err == 0) {
            *arg0 = 3;
            ret = TRUE;
        }
    }
    padmgr_UnlockSerialMesgQ(l_pak_ctrl.queue);
    return ret;
}

s32 mCPk_CheckData(s32* arg0, s32 arg1, PakInfo* info) {
    UNUSED s32 pad;
    s32 ret;
    void* sp1C;

    if (arg1 == 0) {
        sp1C = &l_pak_ctrl;
    } else {
        sp1C = zelda_malloc(info->pfsState.file_size);
    }

    ret = mCPk_CheckData_common(arg0, arg1, info, sp1C);
    if ((arg1 != 0) && (sp1C != NULL)) {
        zelda_free(sp1C);
    }

    return ret;
}

UNK_RET mCPk_PakDataStatus2Permission(s32 arg0) {
    s32 ret;

    switch (arg0) {
        case 0:
            ret = 4;
            break;

        case 1:
        case 2:
            ret = 3;
            break;

        case 3:
            ret = 2;
            break;

        case 4:
            ret = 6;
            break;

        case 5:
            ret = 5;
            break;

        default:
            ret = 5;
            break;
    }

    return ret;
}

s32 mCPk_GetPakPlayerNo(void) {
    PakInfo* info = mCPk_get_pkinfo();
    PrivateInfo* iter = &common_data.save.saveFilePrivateInfo[0];
    s32 ret = -1;
    s32 temp_v0_2;
    s32 i;

    mCPk_SetForestPakState(info, 0);

    temp_v0_2 = mCPk_Load(info, &l_pak_ctrl.passport);

    if (temp_v0_2 == 1) {
        l_pak_ctrl.unk_1204 = 1;
    }

    if ((temp_v0_2 == 1) && !mPr_NullCheckPersonalID(&l_pak_ctrl.passport.priv.playerId)) {
        ret = 4;
        for (i = 0; i < ARRAY_COUNT(common_data.save.saveFilePrivateInfo); i++) {
            if (mPr_CheckCmpPrivate(&l_pak_ctrl.passport.priv, iter) == TRUE) {
                ret = i;
                break;
            }
            iter++;
        }
    }

    return ret;
}

void* mCPk_GetPakPlayerName(void) {
    PersonalID_c* id = NULL;
    PakInfo* info = mCPk_get_pkinfo();
    void* sp24 = &mCPk_dummyName;

    if (l_pak_ctrl.unk_1204 == 0) {
        mCPk_SetForestPakState(info, 0);
        if (mCPk_Load(info, &l_pak_ctrl.passport) == 1) {
            l_pak_ctrl.unk_1204 = 1;
            id = &l_pak_ctrl.passport.priv.playerId;
        }
    } else {
        id = &l_pak_ctrl.passport.priv.playerId;
    }

    if ((id != NULL) && !mPr_NullCheckPersonalID(id)) {
        sp24 = id;
    }

    return sp24;
}

s32 mCPk_SetPakCopyProtect(PakInfo* info) {
    UNUSED s32 pad;
    u32 temp_v0;
    s32 ret = FALSE;

    if (l_pak_ctrl.unk_1204 == 0) {
        mCPk_SetForestPakState(info, 0);
        if (mCPk_Load(info, &l_pak_ctrl.passport) == 1) {
            l_pak_ctrl.unk_1204 = 1;
        }
    }
    if (l_pak_ctrl.unk_1204 == 1) {
        temp_v0 = fqrand() * 65534.0f;
        l_pak_ctrl.unk_1208 = temp_v0;
        l_pak_ctrl.passport.unk_1100 = temp_v0;
        l_pak_ctrl.passport.unk_0000 = mFRm_GetFlatCheckSum(&l_pak_ctrl, sizeof(Passport), l_pak_ctrl.passport.unk_0000);

        if (mCPk_Save(info, &l_pak_ctrl.passport) == 1) {
            ret = TRUE;
        }
    }
    return ret;
}

s32 mCPk_CheckPakCopyProtect(PakInfo* info) {
    s32 ret;

    if (l_pak_ctrl.unk_1208 != 0xFFFF) {
        ret = FALSE;
        mCPk_SetForestPakState(info, 0);
        if (mCPk_Load(info, &l_pak_ctrl.passport) == 1) {
            l_pak_ctrl.unk_1204 = 1;
            if (l_pak_ctrl.unk_1208 == l_pak_ctrl.passport.unk_1100) {
                ret = TRUE;
            }
        }
    } else {
        ret = TRUE;
    }
    return ret;
}

UNK_RET mCPk_GetPermissionSavePakPlayer(void) {
    PakInfo* info = mCPk_get_pkinfo();
    s32 ret = 0;
    s32 sp1C;

    mCPk_SetForestPakState(info, 0);

    if (mCPk_CheckPakCopyProtect(info) == 1) {
        if (mCPk_CheckData(&sp1C, 0, info) == 1) {
            ret = mCPk_PakDataStatus2Permission(sp1C);
        }
    } else {
        ret = 1;
    }

    return ret;
}

void mCPk_CheckCheckSumMail(UNK_PTR arg0, u32 arg1, PakInfo* info) {
    UNUSED s32 pad[2];
    s32 temp_v0 = mCPk_LoadPakMail2(arg0, info);
    OSMesgQueue* queue;
    PakInfo* info2;

    if (temp_v0 == 1) {
        if (mFRm_ReturnCheckSum(arg0, arg1) != 0) {
            mCPk_SetForestPakState(info, 1);
            queue = padmgr_LockSerialMesgQ();
            sCPk_DeleteFile(&info->pfsInfo, &info->pfsState);
            padmgr_UnlockSerialMesgQ(queue);
        }
    } else if (temp_v0 == -1) {
        info2 = mCPk_get_pkinfo();
        mCPk_SetForestPakState(info2, 1);
        l_pak_ctrl.queue = padmgr_LockSerialMesgQ();
        sCPk_DeleteFile(&info2->pfsInfo, &info2->pfsState);
        padmgr_UnlockSerialMesgQ(l_pak_ctrl.queue);
    }
}

UNK_RET mCPk_GetPermissionSavePakMail(PakInfo* info) {
    UNUSED s32 pad;
    s32 sp28;
    s32 ret = 0;
    UNK_PTR sp20;

    if (!mLd_PlayerManKindCheck()) {
        mCPk_SetForestPakState(info, 1);
        sp20 = zelda_malloc(info->pfsState.file_size);
        if (sp20 != NULL) {
            mCPk_CheckCheckSumMail(sp20, info->pfsState.file_size, info);
        }
        if (mCPk_CheckData_common(&sp28, 1, info, sp20) == TRUE) {
            ret = mCPk_PakDataStatus2Permission(sp28);
        }
        if (sp20 != NULL) {
            zelda_free(sp20);
        }
    } else if (mImm_CheckStartDataCategory() == 1) {
        mCPk_SetForestPakState(info, 0);
        if (mCPk_CheckPakCopyProtect(info) == 1) {
            if (mCPk_CheckData(&sp28, 0, info) == 1) {
                ret = mCPk_PakDataStatus2Permission(sp28);
            }
        } else {
            ret = 1;
        }
    }

    return ret;
}

UNK_RET mCPk_GetPermissionSavePakMailAndOpen(void) {
    PakInfo* info = mCPk_get_pkinfo();
    s32 ret = 0;

    if (mCPk_PakOpen(info, 0) == TRUE) {
        ret = mCPk_GetPermissionSavePakMail(info);
    }

    return ret;
}

s32 mCPk_SavePakMail_init(D801047B0Struct* arg0, PakInfo* info, u8* index) {
    mCPk_SetForestPakState(info, 1);
    arg0->unk_0000 = mFRm_GetFlatCheckSum(arg0, sizeof(D801047B0Struct), arg0->unk_0000);
    if (mCPk_Save(info, arg0) == 1) {
        if (mLd_PlayerManKindCheck()) {
            if (mCPk_SavePak(common_data.privateInfo, mNpc_GetInAnimalP(), info) == 1) {
                *index = 1;
                return 0;
            }
            return -1;
        }

        *index = 1;
        return 0;
    }

    *index = 0;
    return -1;
}

s32 mCPk_SavePakMail_main(UNUSED D801047B0Struct* arg0, UNUSED PakInfo* info, u8* arg2) {
    s32 ret = mFRm_FlashSaveAllData_bg();

    if (ret != 0) {
        *arg2 = 0;
    }

    return ret;
}

typedef s32 (*D801047B0Func)(D801047B0Struct*, PakInfo*, u8*);
D801047B0Func D_801047B0_jp[2] = { mCPk_SavePakMail_init, mCPk_SavePakMail_main };

s32 mCPk_SavePakMail2(D801047B0Struct* arg0, PakInfo* info) {
    static u8 D_801047B8_jp = 0;

    if (D_801047B8_jp >= ARRAY_COUNT(D_801047B0_jp)) {
        D_801047B8_jp = 0;
    }

    return D_801047B0_jp[D_801047B8_jp](arg0, info, &D_801047B8_jp);
}

s32 mCPk_LoadPakMail2(UNK_PTR arg0, PakInfo* info) {
    mCPk_SetForestPakState(info, 1);
    return mCPk_Load(info, arg0);
}

s32 mCPk_EraseForestData(void) {
    PakInfo* info = mCPk_get_pkinfo();

    mCPk_ClearPassport(&l_pak_ctrl.passport);
    mCPk_SetForestPakState(info, 0);
    l_pak_ctrl.passport.unk_0000 = mFRm_GetFlatCheckSum(&l_pak_ctrl, sizeof(Passport), l_pak_ctrl.passport.unk_0000);
    return mCPk_Save(info, &l_pak_ctrl.passport);
}

s32 mCPk_CheckCheckSumPlayer(void) {
    PakInfo* info = mCPk_get_pkinfo();
    u16 sp2A = 0;
    s32 sp24;
    s32 ret = 0;

    mCPk_SetForestPakState(info, 0);

    sp24 = mCPk_Load(info, &l_pak_ctrl.passport);

    if (sp24 == 1) {
        l_pak_ctrl.unk_1204 = 1;
        sp2A = mFRm_ReturnCheckSum(&l_pak_ctrl, sizeof(Passport));
    } else if (sp24 == -1) {
        ret = -2;
    } else {
        ret = -1;
    }

    if ((sp24 == 1) && (sp2A == 0)) {
        ret = 1;
    } else {
        l_pak_ctrl.unk_1204 = 0;
        mCPk_ClearPassport(&l_pak_ctrl.passport);
    }

    return ret;
}

void mCPk_CheckCheckSumPlayerErase(void) {
    PakInfo* info = mCPk_get_pkinfo();
    s32 temp_v0 = mCPk_CheckCheckSumPlayer();
    OSMesgQueue* queue;

    if ((temp_v0 == 0) || (temp_v0 == -2)) {
        mCPk_SetForestPakState(info, 0);
        queue = padmgr_LockSerialMesgQ();
        sCPk_DeleteFile(&info->pfsInfo, &info->pfsState);
        padmgr_UnlockSerialMesgQ(queue);
    }
}

PakInfo* mCPk_get_pkinfo(void) {
    return g_paks_info_p;
}

s32 mCPk_CheckPakID(void) {
    PakInfo* info = mCPk_get_pkinfo();
    s32 ret = TRUE;

    if ((info != NULL) && (info->pfsInfo.err == PFS_ERR_ID_FATAL)) {
        ret = FALSE;
    }

    return ret;
}
