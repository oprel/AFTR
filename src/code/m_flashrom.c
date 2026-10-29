#include "m_flashrom.h"
#include "global.h"
#include "libu64/gfxprint.h"
#include "m_common_data.h"
#include "m_lib.h"
#include "m_malloc.h"
#include "m_time.h"
#include "sFRm_flashrom.h"

extern s32 mFRm_isUsed;
extern s32 mFRm_repairCheck;
extern u8 mFRm_flashDmaFlag;
extern s32 mFRm_error_info[6];
extern s32 (*mFRm_WriteHalfFlash_bg_half_write[2])(u32*, s32, mFRm_rpr_c*);
extern s32 (*mFRm_RepairSaveData_repair_proc[4])(u32*, u32*, mFRm_rpr_c*);
extern s32 (*mFRm_FlashSaveAllData_bg_write_bg[7])(mFRm_wctrl_c*);
extern mFRm_rpr_c mFRm_repair;
extern mFRm_wctrl_c mFRm_writeCtrl;

void mFRm_ClearErrInfo(void) {
    bzero(mFRm_error_info, sizeof(mFRm_error_info));
}

void mFRm_OnErrInfo(s32 no) {
    if ((no >= 0) && (no < 6)) {
        mFRm_error_info[no] = 1;
    }
}

void mFRm_OffErrInfo(s32 no) {
    if ((no >= 0) && (no < 6)) {
        mFRm_error_info[no] = 0;
    }
}

void mFRm_PrintErrInfo(gfxprint* gfxprint) {
    gfxprint_color(gfxprint, 250, 100, 250, 255);
    gfxprint_locate8x8(gfxprint, 22, 3);

    if (mFRm_error_info[0] != 0) {
        gfxprint_printf(gfxprint, "N");
    }
    if (mFRm_error_info[1] != 0) {
        gfxprint_printf(gfxprint, "A");
    }
    if (mFRm_error_info[2] != 0) {
        gfxprint_printf(gfxprint, "W");
    }
    if (mFRm_error_info[3] != 0) {
        gfxprint_printf(gfxprint, "R");
    }
    if (mFRm_error_info[4] != 0) {
        gfxprint_printf(gfxprint, "C");
    }
    if (mFRm_error_info[5] != 0) {
        gfxprint_printf(gfxprint, "O");
    }
}

void mFRm_FlashRomInit(void) {
    sFRm_Init();
    mFRm_ClearFlashromControl();
    mFRm_isUsed = 0;
    mFRm_repairCheck = 0;
}

s32 mFRm_GetFlashArea(void) {
    return mFRm_isUsed;
}

u8 mFRm_FlashDmaCheck(void) {
    return mFRm_flashDmaFlag;
}

u16 mFRm_ReturnCheckSum(u16* data, s32 size) {
    u16 checkSum = 0;

    if ((size & 1) == 0) {
        while (true) {
            if (size == 0) {
                break;
            }
            checkSum += *data++;
            size -= 2;
        }
    }

    return checkSum;
}

u16 mFRm_GetFlatCheckSum(void* data, s32 size, u16 nowCheckSum) {
    u16 checkSum = mFRm_ReturnCheckSum(data, size);

    return ~(checkSum - nowCheckSum) + 1;
}

s32 mFRm_CheckSaveData_ID(mFRm_chk_c* saveCheck) {
    s32 ret = FALSE;

    if (saveCheck->code == 0x4E41464A) {
        ret = TRUE;
    }

    return ret;
}

s32 mFRm_CheckSaveData_common(mFRm_chk_c* saveCheck, u16 landId) {
    s32 ret = FALSE;

    if (mFRm_CheckSaveData_ID(saveCheck)) {
        if (((saveCheck->landId & 0xFF00) == 0x3000) && (saveCheck->landId == landId)) {
            ret = TRUE;
        }
    }

    return ret;
}

s32 mFRm_CheckSaveData(void) {
    mFRm_chk_c* saveCheck = (mFRm_chk_c*)&common_data;
    u16 landId = common_data.save.landInfo.id;

    return mFRm_CheckSaveData_common(saveCheck, landId);
}

void mFRm_ClearSaveCheckData(void* saveCheckV) {
    mFRm_chk_c* saveCheck = saveCheckV;

    saveCheck->code = -1;
    saveCheck->landId = 0xFFFF;
    mem_copy((u8*)&saveCheck->time, (u8*)&mTM_rtcTime_clear_code, sizeof(lbRTC_time_c));
    saveCheck->checkSum = 0;
}

void mFRm_SetSaveCheckData(mFRm_chk_c* saveCheck) {
    u16 landId = common_data.save.landInfo.id;

    saveCheck->code = 0x4E41464A;
    saveCheck->landId = landId;
    lbRTC_TimeCopy(&saveCheck->time, &common_data.time.rtcTime);
}

void mFRm_ClearRepairInfo(mFRm_rpr_c* repairP) {
    repairP->mode = 0;
    repairP->startPage = 0;
    repairP->status = 0;
    repairP->buffer = NULL;
    repairP->top = NULL;
    repairP->retry = 0;
}

s32 mFRm_WriteFlashSector_init_bg(u32* buffer, s32 status, mFRm_rpr_c* repairP) {
    u32 startPageNo;
    s32 ret = 0;

    if (status == 0) {
        startPageNo = 0;
    } else {
        startPageNo = mFRm_SAVE_PAGE_NUM;
    }

    repairP->mode = 1;
    repairP->startPage = startPageNo;
    repairP->status = status;
    repairP->buffer = buffer;
    repairP->top = buffer;
    repairP->retry = 0;
    sFRm_WriteAsync(buffer, startPageNo, mFRm_ARRAY_PAGE_NUM);

    return ret;
}

s32 mFRm_WriteFlashSector_main_bg(u32* buffer, s32 status, mFRm_rpr_c* repairP) {
    u32 basePage;
    s32 ret = 0;

    basePage = repairP->status << 9;
    if (sFRm_IsBusy() == TRUE) {
        if (sFRm_GetResult() == -1) {
            if (repairP->retry < 3) {
                repairP->buffer = repairP->top;
                sFRm_WriteAsync(repairP->buffer, basePage, mFRm_ARRAY_PAGE_NUM);
                repairP->retry++;
                repairP->startPage = basePage;
            } else {
                mFRm_ClearRepairInfo(repairP);
                ret = -1;
                mFRm_OnErrInfo(2);
            }
        } else {
            repairP->startPage += mFRm_ARRAY_PAGE_NUM;
            if (repairP->startPage >= basePage + mFRm_SAVE_PAGE_NUM) {
                sFRm_AwaitResult();
                mFRm_ClearRepairInfo(repairP);
                ret = 1;
            } else {
                sFRm_AwaitResult();
                repairP->buffer += 0x1000;
                sFRm_WriteAsync(repairP->buffer, repairP->startPage, mFRm_ARRAY_PAGE_NUM);
            }
        }
    }

    return ret;
}

s32 mFRm_WriteHalfFlash_bg(u32* buffer, s32 status) {
    s32 mode = mFRm_repair.mode;
    s32 ret = 1;

    if ((mode >= 0) && (mode < 2)) {
        ret = mFRm_WriteHalfFlash_bg_half_write[mode](buffer, status, &mFRm_repair);
    }

    return ret;
}

void mFRm_ClearRepair(void) {
    mFRm_ClearRepairInfo(&mFRm_repair);
    mFRm_repairCheck = 0;
}

s32 mFRm_GetRepair(void) {
    return mFRm_repairCheck;
}

s32 mFRm_RepairSaveData_checksum(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP) {
    Save* save = (Save*)writeFileP;
    u16 checkSum;
    s32 ret = 0;
    s32 retries;
    s32 firstCheck = 0;
    s32 realCheck = 0;
    s32 backCheck = 0;

    mFRm_OnErrInfo(5);
    mFRm_ClearRepairInfo(repairP);
    mFRm_repairCheck = 1;
    bzero(writeFileP, 0x10000);
    *((u8*)&common_data + 0x10A7C) = 0;

    if (sFRm_IsInit() == FALSE) {
        mFRm_ClearRepairInfo(repairP);
        ret = 1;
    }

    if (ret == 0) {
        retries = 0;
        while (true) {
            mFRm_FlashLoadAllData_common(writeFileP, 0);
            if (mFRm_CheckSaveData_ID((mFRm_chk_c*)save)) {
                firstCheck = 1;
            }
            if ((mFRm_ReturnCheckSum(writeFileP, mFRm_SAVE_SIZE) == 0) &&
                mFRm_CheckSaveData_common((mFRm_chk_c*)save, save->landInfo.id)) {
                mFRm_isUsed = 0;
                mFRm_ClearRepairInfo(repairP);
                realCheck = 1;
                repairP->mode = 1;
                repairP->startPage = 0;
                repairP->status = 0;
                mFRm_repairCheck = 0;
                mFRm_OffErrInfo(5);
                break;
            }
            if (retries >= 3) {
                break;
            }
            retries++;
        }
    }

    if (ret == 0) {
        retries = 0;
        while (true) {
            bzero(writeFileP, 0x10000);
            mFRm_FlashLoadAllData_common(writeFileP, mFRm_SAVE_PAGE_NUM);
            if (mFRm_CheckSaveData_ID((mFRm_chk_c*)save)) {
                firstCheck = 1;
            }
            if ((mFRm_ReturnCheckSum(writeFileP, mFRm_SAVE_SIZE) == 0) &&
                mFRm_CheckSaveData_common((mFRm_chk_c*)save, save->landInfo.id)) {
                mFRm_ClearRepairInfo(repairP);
                if (realCheck == 0) {
                    mFRm_isUsed = 1;
                    repairP->mode = 1;
                    repairP->startPage = mFRm_SAVE_PAGE_NUM;
                    repairP->status = 1;
                }
                backCheck = 1;
                break;
            }
            if (retries >= 3) {
                break;
            }
            retries++;
        }
    }

    if ((ret == 0) && (realCheck == 0) && (backCheck == 0)) {
        ret = 1;
        if (firstCheck != 0) {
            *((u8*)&common_data + 0x10A7C) = 1;
        }
        mFRm_ClearRepairInfo(repairP);
        mFRm_OffErrInfo(5);
    } else if ((realCheck == 1) && (backCheck == 1)) {
        mFRm_ClearRepairInfo(repairP);
        bzero(writeFileP, 0x10000);
        mFRm_FlashLoadAllData_common(writeFileP, 0);
        bzero(readFileP, 0x10000);
        mFRm_FlashLoadAllData_common(readFileP, mFRm_SAVE_PAGE_NUM);
        if (bcmp(writeFileP, readFileP, mFRm_SAVE_SIZE) != 0) {
            mFRm_isUsed = 1;
            repairP->mode = 1;
            repairP->startPage = mFRm_SAVE_PAGE_NUM;
            repairP->status = 1;
        } else {
            mFRm_isUsed = 0;
            ret = 1;
            mFRm_OffErrInfo(5);
            mFRm_repairCheck = 0;
        }
    }

    return ret;
}

s32 mFRm_RepairSaveData_load(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP) {
    s32 readTime = 0;
    s32 result;
    s32 ret = 0;
    u32 startPage;

    startPage = repairP->startPage;
    do {
        result = mFRm_FlashLoadAllData_common(writeFileP, startPage);
        readTime++;
    } while ((result == 0) && (readTime != 3));

    if (result == 1) {
        repairP->mode = 2;
        repairP->buffer = writeFileP;
        repairP->top = writeFileP;
        repairP->retry = 0;
        if (repairP->status == 0) {
            repairP->startPage = mFRm_SAVE_PAGE_NUM;
            repairP->status = 1;
        } else {
            repairP->startPage = 0;
            repairP->status = 0;
        }
    } else {
        mFRm_ClearRepairInfo(repairP);
        ret = 1;
    }

    return ret;
}

s32 mFRm_RepairSaveData_write(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP) {
    s32 ret = 0;

    repairP->buffer = repairP->top;
    sFRm_WriteAsync(repairP->buffer, repairP->startPage, mFRm_ARRAY_PAGE_NUM);
    repairP->mode = 3;

    return ret;
}

s32 mFRm_RepairSaveData_write_main(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP) {
    u32 basePage;
    s32 ret = 0;

    basePage = repairP->status << 9;
    if (sFRm_IsBusy() == TRUE) {
        if (sFRm_GetResult() == -1) {
            if (repairP->retry < 3) {
                repairP->buffer = repairP->top;
                sFRm_WriteAsync(writeFileP, basePage, mFRm_ARRAY_PAGE_NUM);
                repairP->retry++;
                repairP->startPage = basePage;
            } else {
                mFRm_ClearRepairInfo(repairP);
                ret = 1;
                mFRm_OnErrInfo(2);
            }
        } else {
            repairP->startPage += mFRm_ARRAY_PAGE_NUM;
            if (repairP->startPage >= basePage + mFRm_SAVE_PAGE_NUM) {
                sFRm_AwaitResult();
                mFRm_ClearRepairInfo(repairP);
                ret = 1;
            } else {
                sFRm_AwaitResult();
                repairP->buffer += 0x1000;
                sFRm_WriteAsync(repairP->buffer, repairP->startPage, mFRm_ARRAY_PAGE_NUM);
            }
        }
    }

    return ret;
}

s32 mFRm_RepairSaveData(void* writeFileP, void* readFileP) {
    s32 mode = mFRm_repair.mode;
    s32 ret;

    if ((mode >= 0) && (mode < 4)) {
        ret = mFRm_RepairSaveData_repair_proc[mode](writeFileP, readFileP, &mFRm_repair);
    } else {
        mFRm_ClearRepairInfo(&mFRm_repair);
        ret = 1;
    }

    return ret;
}

s32 mFRm_FlashSaveAllData(void) {
    u32* rwFile = (u32*)&common_data;
    s32 err;
    u32 sectorNumMax;
    u32 i;
    u32 j;
    s32 result;
    mFRm_chk_c* saveCheck = (mFRm_chk_c*)rwFile;

    sectorNumMax = 0x1F3;
    err = 0;
    sFRm_EraseAll();
    mFRm_SetSaveCheckData(saveCheck);
    *(volatile u16*)&saveCheck->checkSum =
        mFRm_GetFlatCheckSum(saveCheck, mFRm_SAVE_SIZE, *(volatile u16*)&saveCheck->checkSum);

    for (i = 0; i != sectorNumMax; i++) {
        result = sFRm_WritePage(rwFile, i);
        if (result == -1) {
            j = 0;
            do {
                result = sFRm_WritePage(rwFile, i);
                j++;
            } while ((result != 0) && (j != 3));

            if (result == -1) {
                err = -1;
            }
        }
        rwFile += 0x20;
    }

    return err;
}

s32 mFRm_FlashLoadAllData_common(u32* buffer, u32 startSector) {
    u32 sectorNumMax;
    u32 i;
    s32 result;
    s32 ret = 1;

    if (sFRm_IsInit() == TRUE) {
        sectorNumMax = startSector + 0x1F3;
        if (startSector < sectorNumMax) {
            i = startSector;
            do {
                result = sFRm_ReadPage(buffer, i);
                i++;
                if (result == -1) {
                    ret = 0;
                    break;
                }
                buffer += 0x20;
            } while (i != sectorNumMax);
        }
    } else {
        ret = 0;
    }

    return ret;
}

s32 mFRm_FlashLoadAllData(void) {
    u32 startPage = mFRm_isUsed << 9;
    u32* rwFile = (u32*)&common_data;

    return mFRm_FlashLoadAllData_common(rwFile, startPage);
}

s32 mFRm_FlashLoadAllData_malloc(void) {
    u32* rwFile;
    Save* save;
    s32 readTime = 0;
    s32 result = 0;
    s32 ret = 0;
    u32 startPage;

    startPage = mFRm_isUsed << 9;
    rwFile = (u32*)zelda_malloc(mFRm_SAVE_SIZE);
    if (rwFile != NULL) {
        do {
            result = mFRm_FlashLoadAllData_common(rwFile, startPage);
            readTime++;
        } while ((readTime < 3) && (result == 0));

        if ((result == 1) && (mFRm_CheckSaveData_ID((mFRm_chk_c*)rwFile) == TRUE)) {
            bcopy(rwFile, &common_data, mFRm_SAVE_SIZE);
            ret = 1;
        }

        if (rwFile != NULL) {
            zelda_free(rwFile);
        }
    }

    return ret;
}

void mFRm_ClearFlashromWriteControl(mFRm_wctrl_c* wctrlP) {
    wctrlP->mode = 0;
    wctrlP->page = 0;
    wctrlP->buffer = NULL;
    wctrlP->top = NULL;
    wctrlP->checkRetry = 0;
    wctrlP->writeRetry = 0;
    wctrlP->readRetry = 0;
    wctrlP->areaRetry = 0;
}

void mFRm_ClearFlashromControl(void) {
    mFRm_ClearFlashromWriteControl(&mFRm_writeCtrl);
}

s32 mFRm_FlashSaveCleanUp_noSync(void) {
    mFRm_wctrl_c* wctrlP = &mFRm_writeCtrl;

    if (wctrlP->top != NULL) {
        func_800D986C_jp(wctrlP->top);
    }
    mFRm_ClearFlashromWriteControl(wctrlP);

    return 1;
}

s32 mFRm_FlashSaveCleanUp(void) {
    sFRm_AwaitResult();
    mFRm_FlashSaveCleanUp_noSync();

    return 1;
}

s32 mFRm_FlashSaveGetArea_bg(mFRm_wctrl_c* wctrlP) {
    s32 ret = 0;

    mFRm_ClearErrInfo();
    wctrlP->buffer = func_800D97A0_jp(mFRm_SAVE_SIZE);
    if (wctrlP->buffer != NULL) {
        wctrlP->top = wctrlP->buffer;
        wctrlP->mode = 1;
        wctrlP->areaRetry = 0;
    } else if (wctrlP->areaRetry < 10) {
        wctrlP->areaRetry++;
    } else {
        mFRm_FlashSaveCleanUp_noSync();
        ret = -1;
        mFRm_OnErrInfo(1);
    }

    return ret;
}

s32 mFRm_FlashSaveInit_bg(mFRm_wctrl_c* wctrlP) {
    mFRm_chk_c* saveCheck = (mFRm_chk_c*)wctrlP->buffer;

    bcopy(&common_data, saveCheck, mFRm_SAVE_SIZE);
    wctrlP->page = 0;
    wctrlP->writeRetry = 0;
    saveCheck->version = 2;
    mFRm_SetSaveCheckData(saveCheck);
    saveCheck->checkSum = mFRm_GetFlatCheckSum(saveCheck, mFRm_SAVE_SIZE, saveCheck->checkSum);
    sFRm_WriteAsync(wctrlP->buffer, 0, mFRm_ARRAY_PAGE_NUM);
    wctrlP->mode = 2;

    return 0;
}

s32 mFRm_FlashSaveReal_bg(mFRm_wctrl_c* wctrlP) {
    s32 ret = 0;

    if (sFRm_IsBusy() == TRUE) {
        if (sFRm_GetResult() == -1) {
            if (wctrlP->writeRetry < 3) {
                wctrlP->page = 0;
                wctrlP->buffer = wctrlP->top;
                sFRm_WriteAsync(wctrlP->buffer, 0, mFRm_ARRAY_PAGE_NUM);
                wctrlP->writeRetry++;
            } else {
                mFRm_FlashSaveCleanUp();
                ret = -1;
                mFRm_OnErrInfo(2);
            }
        } else {
            wctrlP->page += mFRm_ARRAY_PAGE_NUM;
            if (wctrlP->page >= mFRm_SAVE_PAGE_NUM) {
                sFRm_AwaitResult();
                wctrlP->mode = 3;
                wctrlP->writeRetry = 0;
                wctrlP->buffer = wctrlP->top;
            } else {
                sFRm_AwaitResult();
                wctrlP->buffer += 0x1000;
                sFRm_WriteAsync(wctrlP->buffer, wctrlP->page, mFRm_ARRAY_PAGE_NUM);
            }
        }
    }

    return ret;
}

s32 mFRm_FlashCheckSave_bg(mFRm_wctrl_c* wctrlP) {
    u16 checkSum;
    s32 ret = 0;
    s32 result;

    result = mFRm_FlashLoadAllData_common(wctrlP->buffer, 0);
    if (result == 1) {
        wctrlP->readRetry = 0;
        checkSum = mFRm_ReturnCheckSum(wctrlP->buffer, mFRm_SAVE_SIZE);
        if (checkSum == 0) {
            wctrlP->mode = 5;
            wctrlP->page = mFRm_SAVE_PAGE_NUM;
            sFRm_WriteAsync(wctrlP->buffer, mFRm_SAVE_PAGE_NUM, mFRm_ARRAY_PAGE_NUM);
            wctrlP->checkRetry = 0;
        } else if (wctrlP->checkRetry < 3) {
            wctrlP->checkRetry++;
            wctrlP->mode = 1;
        } else {
            mFRm_FlashSaveCleanUp_noSync();
            ret = -1;
            mFRm_OnErrInfo(4);
        }
    } else if (wctrlP->readRetry < 3) {
        wctrlP->readRetry++;
    } else {
        mFRm_FlashSaveCleanUp_noSync();
        ret = -1;
        mFRm_OnErrInfo(3);
    }

    return ret;
}

s32 mFRm_FlashReadReal_bg(mFRm_wctrl_c* wctrlP) {
    s32 ret = 0;

    wctrlP->buffer = wctrlP->top;
    if (mFRm_FlashLoadAllData_common(wctrlP->buffer, 0) == 1) {
        wctrlP->readRetry = 0;
        wctrlP->mode = 5;
        wctrlP->page = mFRm_SAVE_PAGE_NUM;
        sFRm_WriteAsync(wctrlP->buffer, mFRm_SAVE_PAGE_NUM, mFRm_ARRAY_PAGE_NUM);
    } else if (wctrlP->readRetry < 3) {
        wctrlP->readRetry++;
    } else {
        mFRm_FlashSaveCleanUp_noSync();
        ret = -1;
        mFRm_OnErrInfo(3);
    }

    return ret;
}

s32 mFRm_FlashSaveBackup_bg(mFRm_wctrl_c* wctrlP) {
    s32 ret = 0;

    if (sFRm_IsBusy() == TRUE) {
        if (sFRm_GetResult() == -1) {
            if (wctrlP->writeRetry < 3) {
                wctrlP->buffer = wctrlP->top;
                wctrlP->mode = 4;
                wctrlP->writeRetry++;
            } else {
                mFRm_FlashSaveCleanUp();
                ret = -1;
                mFRm_OnErrInfo(2);
            }
        } else {
            wctrlP->page += mFRm_ARRAY_PAGE_NUM;
            if (wctrlP->page >= mFRm_SAVE_PAGE_NUM * 2) {
                sFRm_AwaitResult();
                wctrlP->mode = 6;
                wctrlP->writeRetry = 0;
                wctrlP->buffer = wctrlP->top;
            } else {
                sFRm_AwaitResult();
                wctrlP->buffer += 0x1000;
                sFRm_WriteAsync(wctrlP->buffer, wctrlP->page, mFRm_ARRAY_PAGE_NUM);
            }
        }
    }

    return ret;
}

s32 mFRm_FlashCheckSaveBackup_bg(mFRm_wctrl_c* wctrlP) {
    u16 checkSum;
    s32 ret = 0;
    s32 result;

    result = mFRm_FlashLoadAllData_common(wctrlP->buffer, mFRm_SAVE_PAGE_NUM);
    if (result == 1) {
        wctrlP->readRetry = 0;
        checkSum = mFRm_ReturnCheckSum(wctrlP->buffer, mFRm_SAVE_SIZE);
        if (checkSum == 0) {
            mFRm_FlashSaveCleanUp_noSync();
            ret = 1;
            mFRm_ClearErrInfo();
        } else if (wctrlP->checkRetry < 3) {
            wctrlP->checkRetry++;
            wctrlP->mode = 4;
        } else {
            mFRm_FlashSaveCleanUp_noSync();
            ret = -1;
            mFRm_OnErrInfo(4);
        }
    } else if (wctrlP->readRetry < 3) {
        wctrlP->readRetry++;
    } else {
        mFRm_FlashSaveCleanUp_noSync();
        ret = -1;
        mFRm_OnErrInfo(3);
    }

    return ret;
}

s32 mFRm_FlashSaveAllData_bg(void) {
    s32 mode = mFRm_writeCtrl.mode;
    s32 ret = -1;

    if (sFRm_IsInit() != FALSE) {
        ret = mFRm_FlashSaveAllData_bg_write_bg[mode](&mFRm_writeCtrl);
    } else {
        mFRm_OnErrInfo(0);
    }

    return ret;
}

void mFRm_PrintSavedDebug(gfxprint* gfxprint) {
    if (*((u8*)&common_data + 0x10A7C) == 1) {
        gfxprint_color(gfxprint, 50, 250, 100, 255);
        gfxprint_locate8x8(gfxprint, 29, 3);
        gfxprint_printf(gfxprint, "X");
    }
}
