#ifndef M_LEDIT_OVL_H
#define M_LEDIT_OVL_H

#include "ultra64.h"
#include "unk.h"
#include "color.h"
#include "m_submenu.h"
#include "overlays/submenu/editor_ovl/mED_editor_ovl.h"
#include "overlays/submenu/submenu_ovl/m_submenu_ovl.h"

struct Submenu;
struct Game;

// Types of single line input. Passed to the overlay as the first submenu argument.
enum {
    mLE_TYPE_PLAYER_NAME,
    mLE_TYPE_COUNTRY_NAME,
    mLE_TYPE_EPHRASE,
    mLE_TYPE_RESET,
    mLE_TYPE_REQUEST,

    mLE_TYPE_NUM
};

// Per type window layout. Unlike Animal Crossing the scales are fixed in code.
typedef struct ledit_win_data_s {
    /* 0x00 */ f32 editOfs[2];  // top left of the input line, relative to the window position
    /* 0x08 */ f32 titleOfs[2]; // top left of the title line, relative to the window position
    /* 0x10 */ u8* titleStr;
    /* 0x14 */ s32 titleLen;
    /* 0x18 */ s32 editMax; // maximum number of characters that can be entered
    /* 0x1C */ Color_RGBA8 editColor;
    /* 0x20 */ Gfx* gfxMode;  // segment 0xC display list, sets up the render mode of the window
    /* 0x24 */ Gfx* gfxModel; // segment 0xC display list, the window itself
} mLE_win_data_c;             // size = 0x28

// View of SubmenuOverlay with the fields that the ledit overlay needs.
typedef struct mLE_Overlay_s {
    /* 0x00000 */ UNK_TYPE1 unk_00000[0x10088];
    /* 0x10088 */ mSM_MenuInfo_c menuInfo[SUBMENU_PROGRAM_MAX];
    /* 0x10670 */ mSM_Control_c menuControl;
    /* 0x106A4 */ struct_8085E9B0_unk_106A4 returnFuncProc;
    /* 0x106A8 */ struct_8085E9B0_unk_106A8 moveMoveProc;
    /* 0x106AC */ struct_8085E9B0_unk_106AC moveEndProc;
    /* 0x106B0 */ struct_8085E9B0_unk_106B0 moveChgBaseProc;
    /* 0x106B4 */ struct_8085E9B0_unk_106B4 setCharMatrixProc;
    /* 0x106B8 */ struct_8085E9B0_unk_106B8 cbufCopyProc;
    /* 0x106BC */ struct_8085E9B0_unk_106BC setDrawModeProc;
    /* 0x106C0 */ struct_8085E9B0_unk_106C0 drawItemProc;
    /* 0x106C4 */ struct_8085E9B0_unk_106C4 drawMailProc;
    /* 0x106C8 */ struct_8085E9B0_unk_106C8 setupViewProc;
    /* 0x106CC */ struct_8085E9B0_unk_106CC dataLoadProc;
    /* 0x106D0 */ UNK_TYPE1 unk_106D0[0x10];
    /* 0x106E0 */ mED_Ovl_c* editorOvl;
    /* 0x106E4 */ UNK_TYPE1 unk_106E4[0x41];
    /* 0x10725 */ u8 segmentLoaded;
} mLE_Overlay_c;

void mLE_move_Move(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mLE_move_Wait(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mLE_move_End(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mLE_ledit_ovl_move(struct Submenu* submenu);
void mLE_set_dl(struct Submenu* submenu, struct Game* game, mSM_MenuInfo_c* menuInfo);
void mLE_ledit_ovl_draw(struct Submenu* submenu, struct Game* game);
void mLE_ledit_ovl_init(struct Submenu* submenu);
void mLE_ledit_ovl_construct(struct Submenu* submenu);
void mLE_ledit_ovl_destruct(struct Submenu* submenu);
void mLE_ledit_ovl_set_proc(struct Submenu* submenu);

#endif
