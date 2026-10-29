#ifndef M_FLASHROM_H
#define M_FLASHROM_H

#include "ultra64.h"
#include "unk.h"

#include "lb_rtc.h"
#include "libu64/gfxprint.h"

#define mFRm_ARRAY_PAGE_NUM 0x80
#define mFRm_SAVE_PAGE_NUM (mFRm_ARRAY_PAGE_NUM * 4)
#define mFRm_SAVE_SIZE 0xF980

//check
typedef struct mFRm_chk_c {
    /* 0x00 */ u32 version;
    /* 0x04 */ u32 code;
    /* 0x08 */ u16 landId;
    /* 0x0A */ lbRTC_time_c time;
    /* 0x12 */ u16 checkSum;
} mFRm_chk_c; // size = 0x14

//repair
typedef struct mFRm_rpr_c {
    /* 0x00 */ s32 mode;
    /* 0x04 */ u32 startPage;
    /* 0x08 */ s32 status;
    /* 0x0C */ u32* buffer;
    /* 0x10 */ u32* top;
    /* 0x14 */ s32 retry;
} mFRm_rpr_c; // size = 0x18

//write control
typedef struct mFRm_wctrl_c {
    /* 0x00 */ s32 mode;
    /* 0x04 */ u32 page;
    /* 0x08 */ u32* buffer;
    /* 0x0C */ u32* top;
    /* 0x10 */ s32 checkRetry;
    /* 0x14 */ s32 writeRetry;
    /* 0x18 */ s32 readRetry;
    /* 0x1C */ s32 areaRetry;
} mFRm_wctrl_c; // size = 0x20

void mFRm_ClearErrInfo(void);
void mFRm_OnErrInfo(s32 no);
void mFRm_OffErrInfo(s32 no);
void mFRm_PrintErrInfo(gfxprint* gfxprint);
void mFRm_FlashRomInit(void);
s32 mFRm_GetFlashArea(void);
u8 mFRm_FlashDmaCheck(void);
u16 mFRm_ReturnCheckSum(u16* dataP, s32 size);
u16 mFRm_GetFlatCheckSum(void* dataP, s32 size, u16 nowCheckSum);
s32 mFRm_CheckSaveData_ID(mFRm_chk_c* saveCheckP);
s32 mFRm_CheckSaveData_common(mFRm_chk_c* saveCheckP, u16 landId);
s32 mFRm_CheckSaveData(void);
void mFRm_ClearSaveCheckData(void* saveCheckV);
void mFRm_SetSaveCheckData(mFRm_chk_c* saveCheckP);
void mFRm_ClearRepairInfo(mFRm_rpr_c* repairP);
s32 mFRm_WriteFlashSector_init_bg(u32* pakBuffer, s32 status, mFRm_rpr_c* ctrlP);
s32 mFRm_WriteFlashSector_main_bg(u32* pakBuffer, s32 status, mFRm_rpr_c* ctrlP);
s32 mFRm_WriteHalfFlash_bg(u32* pakBuffer, s32 status);
void mFRm_ClearRepair(void);
s32 mFRm_GetRepair(void);
s32 mFRm_RepairSaveData_checksum(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP);
s32 mFRm_RepairSaveData_load(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP);
s32 mFRm_RepairSaveData_write(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP);
s32 mFRm_RepairSaveData_write_main(u32* writeFileP, u32* readFileP, mFRm_rpr_c* repairP);
s32 mFRm_RepairSaveData(void* writeFileP, void* readFileP);
s32 mFRm_FlashSaveAllData(void);
s32 mFRm_FlashLoadAllData_common(u32* pakBuffer, u32 startSector);
s32 mFRm_FlashLoadAllData(void);
s32 mFRm_FlashLoadAllData_malloc(void);
void mFRm_ClearFlashromWriteControl(mFRm_wctrl_c* wctrlP);
void mFRm_ClearFlashromControl(void);
s32 mFRm_FlashSaveCleanUp_noSync(void);
s32 mFRm_FlashSaveCleanUp(void);
s32 mFRm_FlashSaveGetArea_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashSaveInit_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashSaveReal_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashCheckSave_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashReadReal_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashSaveBackup_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashCheckSaveBackup_bg(mFRm_wctrl_c* wctrlP);
s32 mFRm_FlashSaveAllData_bg(void);
void mFRm_PrintSavedDebug(gfxprint* gfxprint);

#endif
