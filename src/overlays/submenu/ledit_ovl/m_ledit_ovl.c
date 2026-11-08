#include "m_ledit_ovl.h"
#include "global.h"

#include "m_common_data.h"
#include "m_font.h"
#include "m_lib.h"
#include "m_std_dma.h"
#include "m_submenu.h"
#include "sys_matrix.h"

#define mLE_MINIDISK_NUM 55
#define mLE_ITEM_NAME_LEN 10
#define mLE_ITEM_MINIDISK_START 0x2A00
#define mLE_ITEM_MINIDISK_END (mLE_ITEM_MINIDISK_START + mLE_MINIDISK_NUM)

#define mLE_OVERLAY(submenu) ((mLE_Overlay_c*)(submenu)->overlay)
#define mLE_MENU(submenu, idx) (&mLE_OVERLAY(submenu)->menuInfo[(idx)])

// data0 of the ledit menu info is one of mLE_TYPE_*, data2 is the buffer that receives the text for the types that
// need one.
typedef void (*mLE_OVL_PROC)(Submenu*, mSM_MenuInfo_c*);

// Alias of mLE_win_data + 0x18, the editMax of the first window.
typedef struct ledit_win_max_s {
    s32 editMax;
    u8 pad[0x24];
} mLE_win_max_c;

// Segment 0xC display lists
extern Gfx D_C015E60[]; // ledit_common_mode
extern Gfx D_C016E90[]; // space underline model

// Segment 0x6 minidisk names inside segment_010F4000
extern u8 D_6000000[];
extern u8 D_6001628[];
extern u8 D_10F4000[];

extern mLE_win_data_c mLE_win_data[]; // one entry per mLE_TYPE_*
extern mLE_win_max_c mLE_win_data_edit_max[];
extern mLE_OVL_PROC mLE_ovl_move_proc[];             // indexed by menu procStatus: Move, none, Wait, none, End
extern u8 mLE_country_str[];                         // 3 characters drawn after the town name
extern struct_func_8085D43C_jp_arg2 mLE_data_dlftbl; // ROM range of the ledit menu segment

extern u8 mLE_md_name[];           // minidisk name cache
extern u8 mLE_npc_end_word_edit[]; // NPC catchphrase edit buffer

extern f32 mFont_SetLineStrings(Game*, u8*, s32, f32, f32, s32, s32, s32, s32, s32, s32, f32, f32, s32);

void mLE_move_Move(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mLE_OVERLAY(submenu)->moveMoveProc(submenu, (void*)menuInfo);
}

void mLE_move_Wait(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    s32 procEnd = mSM_OVL_PROC_END;
    mSM_MenuInfo_c* editorMenu = mLE_MENU(submenu, SUBMENU_PROGRAM_EDITOR);

    if (procEnd == editorMenu->nextProcStatus) {
        mLE_OVERLAY(submenu)->moveChgBaseProc((void*)menuInfo, procEnd);

        if (menuInfo->data0 == mLE_TYPE_REQUEST) {
            mED_Ovl_c* editorOvl = mLE_OVERLAY(submenu)->editorOvl;
            u8* mdNameP = mLE_md_name;
            s32 i;

            if (editorOvl != NULL) {
                for (i = 0; i < mLE_MINIDISK_NUM; i++) {
                    if (mem_cmp(mdNameP, (u8*)editorOvl->inputStr, mLE_ITEM_NAME_LEN)) {
                        submenu->unk_E0 = mLE_ITEM_MINIDISK_START + i;
                        break;
                    }
                    mdNameP += mLE_ITEM_NAME_LEN;
                }

                if (i == mLE_MINIDISK_NUM) {
                    submenu->unk_E0 = mLE_ITEM_MINIDISK_END;
                }
            } else {
                submenu->unk_E0 = mLE_ITEM_MINIDISK_END;
            }
        } else if (menuInfo->data0 == mLE_TYPE_EPHRASE) {
            mem_copy((u8*)menuInfo->data2, mLE_npc_end_word_edit, ANIMAL_CATCHPHRASE_LEN);
        }
    }
}

void mLE_move_End(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mLE_OVERLAY(submenu)->moveEndProc(submenu, (void*)menuInfo);
}

void mLE_ledit_ovl_move(Submenu* submenu) {
    mSM_MenuInfo_c* menuInfo = mLE_MENU(submenu, SUBMENU_PROGRAM_LEDIT);

    menuInfo->preMoveFunc(submenu);
    mLE_ovl_move_proc[menuInfo->procStatus](submenu, menuInfo);
}

void mLE_set_dl(Submenu* submenu, Game* game, mSM_MenuInfo_c* menuInfo) {
    mED_Ovl_c* editorOvl = mLE_OVERLAY(submenu)->editorOvl;
    mLE_win_data_c* dataP = &mLE_win_data[menuInfo->data0];
    GraphicsContext* graph = game->gfxCtx;
    f32 posX = menuInfo->position[0];
    f32 posY = menuInfo->position[1];
    f32 editPosX = dataP->editOfs[0] + posX;
    f32 editPosY = dataP->editOfs[1] - posY;
    s32 texX;
    s32 texY;
    s32 i;

    Matrix_scale(16.0f, 16.0f, 1.0f, MTXMODE_NEW);
    Matrix_translate(posX, posY, 140.0f, MTXMODE_APPLY);

    OPEN_POLY_OPA_DISP(graph);

    gSPSegment(__polyOpa++, 0xC, menuInfo->segment);
    gSPDisplayList(__polyOpa++, D_C015E60);
    gSPDisplayList(__polyOpa++, dataP->gfxMode);
    gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gDPTileSync(__polyOpa++);

    texX = (s32)(-mLE_OVERLAY(submenu)->menuControl.texturePos[0] * 2.0f) & 0x7F;
    texY = (s32)(-mLE_OVERLAY(submenu)->menuControl.texturePos[1] * 2.0f) & 0x7F;

    gDPSetTileSize(__polyOpa++, G_TX_RENDERTILE, texX, texY, 124 + texX, 124 + texY);
    gSPDisplayList(__polyOpa++, dataP->gfxModel);

    if (editorOvl != NULL) {
        mSM_MenuInfo_c* editorMenu = mLE_MENU(submenu, SUBMENU_PROGRAM_EDITOR);

        gSPSegment(__polyOpa++, 0xC, editorMenu->segment);
        gDPSetPrimColor(__polyOpa++, 0, 255, dataP->editColor.r, dataP->editColor.g, dataP->editColor.b, 255);

        for (i = 0; i < editorOvl->nowStrLen; i++) {
            if (editorOvl->inputStr[i] == CHAR_SPACE) {
                Matrix_push();
                Matrix_translate((editPosX - 160.0f) + 6.0f + (f32)i * 12.0f, -((editPosY - 120.0f) + 8.0f), 0.0f,
                                 MTXMODE_APPLY);
                Matrix_scale(0.75f, 1.0f, 1.0f, MTXMODE_APPLY);

                gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(__polyOpa++, D_C016E90);

                Matrix_pull();
            }
        }

        gSPSegment(__polyOpa++, 0xC, menuInfo->segment);
        gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }

    CLOSE_POLY_OPA_DISP(graph);

    if (editorOvl != NULL) {
        mLE_OVERLAY(submenu)->setCharMatrixProc(graph);

        mFont_SetLineStrings(game, dataP->titleStr, dataP->titleLen, dataP->titleOfs[0] + posX,
                             dataP->titleOfs[1] - posY, 255, 255, 255, 255, 0, 0, 0.875f, 0.875f, 0);

        mFont_SetLineStrings(game, (u8*)editorOvl->inputStr, editorOvl->inputLength, editPosX, editPosY,
                             dataP->editColor.r, dataP->editColor.g, dataP->editColor.b, 255, 0, 0, 1.0f, 1.0f,
                             0);

        if (menuInfo->data0 == mLE_TYPE_COUNTRY_NAME) {
            mFont_SetLineStrings(game, mLE_country_str, 3, editPosX + 75.0f, editPosY, 145, 55, 0, 255, 0, 0, 0.875f,
                                 0.875f, 0);
        }

        editPosX += ((f32)(editorOvl->cursorIdx - 1) + 0.3f) * 12.0f;

        mLE_OVERLAY(submenu)->editorOvl->cursolDraw(submenu, game, editPosX, editPosY);
    }
}

void mLE_ledit_ovl_draw(Submenu* submenu, Game* game) {
    mSM_MenuInfo_c* menuInfo = mLE_MENU(submenu, SUBMENU_PROGRAM_LEDIT);

    menuInfo->preDrawFunc(submenu, (struct Game_Play*)game);
    mLE_set_dl(submenu, game, menuInfo);
}

void mLE_ledit_ovl_set_proc(Submenu* submenu) {
    mSM_Control_c* menuControl = &mLE_OVERLAY(submenu)->menuControl;

    menuControl->menuMoveFunc = (void*)mLE_ledit_ovl_move;
    menuControl->menuDrawFunc = (void*)mLE_ledit_ovl_draw;
}

void mLE_ledit_ovl_init(Submenu* submenu) {
    mLE_Overlay_c* overlay = mLE_OVERLAY(submenu);
    mSM_MenuInfo_c* menuInfo = &overlay->menuInfo[SUBMENU_PROGRAM_LEDIT];
    s32 max;
    u8* str;

    overlay->menuControl.animationFlag = 0;

    if (menuInfo->data0 == mLE_TYPE_PLAYER_NAME) {
        str = (u8*)common_data.privateInfo->playerId.playerName;
    } else if (menuInfo->data0 == mLE_TYPE_COUNTRY_NAME) {
        str = (u8*)common_data.save.landInfo.name;
    } else if (menuInfo->data0 == mLE_TYPE_EPHRASE) {
        str = mLE_npc_end_word_edit;
    } else {
        str = (u8*)menuInfo->data2;

        if (menuInfo->data0 == mLE_TYPE_REQUEST) {
            DmaMgr_RequestSyncDebug(mLE_md_name, (u32)D_6001628 - (u32)D_6000000 + (u32)D_10F4000,
                                    sizeof(u8) * mLE_MINIDISK_NUM * mLE_ITEM_NAME_LEN, "../m_ledit_ovl.c", 525);
        }
    }

    max = mLE_win_data_edit_max[menuInfo->data0].editMax;
    mem_clear(str, max, CHAR_SPACE);
    menuInfo->procStatus = mSM_OVL_PROC_MOVE;
    menuInfo->nextProcStatus = mSM_OVL_PROC_WAIT;
    menuInfo->moveDrt = mSM_MOVE_IN_TOP;

    mSM_open_submenu_new(submenu, SUBMENU_PROGRAM_EDITOR, mED_TYPE_LEDIT, max, (s32)str);
}

void mLE_ledit_ovl_construct(Submenu* submenu) {
    mLE_Overlay_c* overlay = mLE_OVERLAY(submenu);

    if (overlay->segmentLoaded == 0) {
        overlay->segmentLoaded = 1;
        mLE_OVERLAY(submenu)->dataLoadProc(submenu, &overlay->menuInfo[SUBMENU_PROGRAM_LEDIT].segment, &mLE_data_dlftbl);
    }

    mLE_ledit_ovl_init(submenu);
    mLE_ledit_ovl_set_proc(submenu);
}

void mLE_ledit_ovl_destruct(Submenu* submenu) {
    mLE_OVERLAY(submenu)->segmentLoaded = 0;
}
