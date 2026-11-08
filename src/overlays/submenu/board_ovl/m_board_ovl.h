#ifndef M_BOARD_OVL_H
#define M_BOARD_OVL_H

#include "ultra64.h"
#include "unk.h"
#include "color.h"
#include "m_mail.h"
#include "m_submenu.h"
#include "overlays/submenu/editor_ovl/mED_editor_ovl.h"
#include "overlays/submenu/submenu_ovl/m_submenu_ovl.h"

struct Submenu;
struct Game_Play;
struct GraphicsContext;

enum {
    mBD_HEADER_POS_PRE_NAME,
    mBD_HEADER_POS_ON_NAME,
    mBD_HEADER_POS_POST_NAME,

    mBD_HEADER_POS_NUM
};

enum {
    mBD_FIELD_HEADER,
    mBD_FIELD_BODY,
    mBD_FIELD_FOOTER,

    mBD_FIELD_NUM
};

typedef struct mBD_Ovl_s {
    /* 0x00 */ u8 field; // one of mBD_FIELD_*
    /* 0x01 */ u8 centerLine;
    /* 0x02 */ u8 headerPos; // one of mBD_HEADER_POS_*
    /* 0x03 */ u8 headerNameLen;
    /* 0x04 */ u8 first;
    /* 0x05 */ u8 lengths[mBD_FIELD_NUM]; // indexed by mBD_FIELD_*
    /* 0x08 */ Mail_c mail;
    Mail_c* dstP;
    f32 ofsX;
    f32 ofsY;
    u8* paperDataP; // _140 in ac-decomp
    u32 paperDataOfs; // _144 in ac-decomp
} mBD_Ovl_c;

typedef struct mBD_Tag_s {
    /* 0x000 */ u8 pad000[0x10C];
    /* 0x10C */ void (*initTagDataItemWinProc)(struct Submenu*);
} mBD_Tag_c;

// View of SubmenuOverlay with the fields that the board overlay needs.
typedef struct mBD_Overlay_s {
    /* 0x00000 */ u8 pad00000[0x10000];
    /* 0x10000 */ u8* allocP;
    /* 0x10004 */ u8 pad10004[0x84];
    /* 0x10088 */ mSM_MenuInfo_c menuInfo[SUBMENU_PROGRAM_MAX];
    /* 0x10670 */ mSM_Control_c menuControl;
    /* 0x106A4 */ void (*returnFuncProc)(struct Submenu*, mSM_MenuInfo_c*);
    /* 0x106A8 */ void (*moveMoveProc)(struct Submenu*, mSM_MenuInfo_c*);
    /* 0x106AC */ void (*moveEndProc)(struct Submenu*, mSM_MenuInfo_c*);
    /* 0x106B0 */ void (*moveChgBaseProc)(mSM_MenuInfo_c*, s32);
    /* 0x106B4 */ void (*setCharMatrixProc)(struct GraphicsContext*);
    /* 0x106B8 */ u8 pad106B8[0x14];
    /* 0x106CC */ struct_8085E9B0_unk_106CC dataLoadProc;
    /* 0x106D0 */ mBD_Tag_c* tagOvl;
    /* 0x106D4 */ u8 pad106D4[0x0C];
    /* 0x106E0 */ mED_Ovl_c* editorOvl;
    /* 0x106E4 */ mBD_Ovl_c* boardOvl;
} mBD_Overlay_c;

void mBD_roll_control(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_roll_control2(mSM_MenuInfo_c* menuInfo);
void mBD_move_Move(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_move_Wait(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_end_board(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_move_Play(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_move_Obey(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_move_End(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo);
void mBD_board_ovl_move(struct Submenu* submenu);
void mBD_set_frame_dl(struct GraphicsContext* graph, mSM_MenuInfo_c* menuInfo, f32 posX, f32 posY,
                      mBD_Ovl_c* boardOvl);
void mBD_set_point(struct Submenu* submenu, struct Game_Play* game, f32 posX, f32 posY);
void mBD_set_cursol(struct Submenu* submenu, struct Game_Play* game, f32 posX, f32 posY);
void mBD_set_writing_footer(struct Submenu* submenu, struct Game_Play* game, f32 posX, f32 posY, Color_RGBA8* color);
void mBD_set_writing_body(struct Submenu* submenu, mSM_MenuInfo_c* menuInfo, struct Game_Play* game, f32 posX,
                          f32* posY, f32* endX, f32* endY, Color_RGBA8* color);
void mBD_set_writing_header(struct Submenu* submenu, struct Game_Play* game, mSM_MenuInfo_c* menuInfo, f32 posX,
                            f32 posY, Color_RGBA8* color);
void mBD_set_character(struct Submenu* submenu, struct Game_Play* game, mSM_MenuInfo_c* menuInfo, f32 posX,
                       f32 posY);
void mBD_set_dl(struct Submenu* submenu, struct Game_Play* game, mSM_MenuInfo_c* menuInfo);
void mBD_board_ovl_draw(struct Submenu* submenu, struct Game_Play* game);
void mBD_board_ovl_init(struct Submenu* submenu);
void mBD_board_letter_dma(struct Submenu* submenu);
void mBD_board_ovl_construct(struct Submenu* submenu);
void mBD_board_ovl_destruct(struct Submenu* submenu);
void mBD_board_ovl_set_proc(struct Submenu* submenu);

#endif
