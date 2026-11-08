#include "m_board_ovl.h"
#include "global.h"

#include "m_common_data.h"
#include "m_controller.h"
#include "m_font.h"
#include "m_lib.h"
#include "m_mail.h"
#include "m_private.h"
#include "m_std_dma.h"
#include "m_submenu.h"
#include "audio.h"
#include "sys_matrix.h"

#define mBD_OVERLAY(submenu) ((mBD_Overlay_c*)(submenu)->overlay)

#define mBD_ML_STRLEN(p, n, c) mMl_strlen((const char*)(p), (n), (char)(c))

typedef void (*mBD_OVL_PROC)(Submenu*, mSM_MenuInfo_c*);

// Segment 0xC display lists
extern Gfx D_C000000[];
// Alias of D_C000000. A separate symbol is needed so that IDO loads the address a second time.
extern Gfx D_C000000_b[];
extern Gfx D_C000040[];
extern Gfx D_C000120[];
extern Gfx D_C000158[];
extern Gfx D_C0001C0[];
extern u8 D_AE3000[];

extern f32 mFont_SetLineStrings(Game* game, void* str, s32 len, f32 x, f32 y, s32 r, s32 g, s32 b, s32 a, s32 arg9,
                                s32 arg10, f32 scaleX, f32 scaleY, s32 mode);

// .data and .bss, still provided by the splat generated asm.
extern Gfx* paper_disp_model[];
extern Gfx* paper_disp_sen_model[];
extern u32 paper_data_offset[];
extern Color_RGBA8 mBD_letter_color[64];
extern mBD_OVL_PROC mBD_ovl_move_proc[5];
extern u8 mBD_header_str[3];
extern u8 mBD_footer_str[2];
extern struct_func_8085D43C_jp_arg2 mBD_data_dlftbl;
extern mBD_Ovl_c mBD_board_ovl_data;

// scroll the board so the cursor line stays visible
void mBD_roll_control(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mED_Ovl_c* editorOvl = mBD_OVERLAY(submenu)->editorOvl;
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;
    int line;
    int dist;
    f32 pos;
    f32 posDist;
    f32 absPosDist;

    if (editorOvl != NULL) {
        if (boardOvl->field == mBD_FIELD_BODY) {
            line = editorOvl->cursorRow + 2;
        } else if (boardOvl->field == mBD_FIELD_HEADER) {
            line = 0;
        } else {
            line = 9;
        }

        dist = line - boardOvl->centerLine;
        if (dist < -2) {
            boardOvl->centerLine = line + 2;
            menuInfo->speed[1] = 1.0f;
        } else if (dist > 2) {
            boardOvl->centerLine = line - 2;
            menuInfo->speed[1] = 1.0f;
        }

        pos = (f32)((boardOvl->centerLine - 2) * 16);
        posDist = pos - menuInfo->position[1];
        absPosDist = fabsf(posDist);

        if (absPosDist > 0.1f) {
            if (absPosDist > 9.0f) {
                menuInfo->speed[1] *= 2.0f;
                if (menuInfo->speed[1] > 4.0f) {
                    menuInfo->speed[1] = 4.0f;
                }
            } else if (absPosDist < 7.0f) {
                menuInfo->speed[1] *= 0.5f;
                if (menuInfo->speed[1] < 1.0f) {
                    menuInfo->speed[1] = 1.0f;
                }
            }
            chase_f(&menuInfo->position[1], pos, menuInfo->speed[1]);
        } else {
            menuInfo->position[1] = pos;
        }
    }
}

// slide-in/out easing used while the "end check" is open
void mBD_roll_control2(mSM_MenuInfo_c* menuInfo) {
    f32 nextPos = menuInfo->position[1] - menuInfo->speed[1];

    if (menuInfo->speed[1] < 1.0f) {
        menuInfo->speed[1] = 1.0f;
    }

    if (nextPos > 0.0f) {
        menuInfo->position[1] = nextPos;
        if (nextPos > -15.0f) {
            menuInfo->speed[1] *= 2.0f;
            if (menuInfo->speed[1] > 16.0f) {
                menuInfo->speed[1] = 16.0f;
            }
        } else if (nextPos < 31.0f) {
            UNUSED f32 dist;

            menuInfo->speed[1] *= 0.5f;
            if (menuInfo->speed[1] < 1.0f) {
                menuInfo->speed[1] = 1.0f;
            }
        }
    } else {
        menuInfo->position[1] = 0.0f;
        menuInfo->speed[1] = 0.0f;
    }
}

void mBD_move_Move(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    (*mBD_OVERLAY(submenu)->moveMoveProc)(submenu, menuInfo);
}

void mBD_move_Wait(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    if (getTrigger() & 0xD000) {
        (*mBD_OVERLAY(submenu)->moveChgBaseProc)(menuInfo, mSM_MOVE_OUT_TOP);
        sAdo_SysTrgStart(0x5F);
    }
}

void mBD_end_board(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mSM_MenuInfo_c* addressMenu = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_ADDRESS];

    (*mBD_OVERLAY(submenu)->moveChgBaseProc)(menuInfo, mSM_MOVE_OUT_TOP);
    (*mBD_OVERLAY(submenu)->returnFuncProc)(submenu, addressMenu);
}

void mBD_move_Play(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mSM_MenuInfo_c* editMenu = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_EDITOR];

    if (editMenu->nextProcStatus == mSM_OVL_PROC_END) {
        menuInfo->procStatus = mSM_OVL_PROC_OBEY;
        mBD_OVERLAY(submenu)->boardOvl->centerLine = 2;
        mSM_open_submenu(submenu, SUBMENU_PROGRAM_EDITENDCHK, 0, 0);
    } else {
        mBD_roll_control(submenu, menuInfo);
    }
}

void mBD_move_Obey(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    mSM_MenuInfo_c* editEndChkMenu = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_EDITENDCHK];
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;

    mBD_roll_control2(menuInfo);

    if (editEndChkMenu->procStatus == mSM_OVL_PROC_MOVE && editEndChkMenu->nextProcStatus == mSM_OVL_PROC_END) {
        mED_Ovl_c* editorOvl = mBD_OVERLAY(submenu)->editorOvl;

        if (editEndChkMenu->data1 == 0) {
            MailHeaderCommon* mailHeader = &common_data.privateInfo->savedMailHeader;
            int tFooterLen;
            int footerLenDiff;
            int i;
            u8* tFooterP;
            u8 tFooter[17];

            mMl_copy_mail(boardOvl->dstP, &boardOvl->mail);

            tFooterLen = mBD_ML_STRLEN(boardOvl->mail.content.footer, MAIL_FOOTER_LEN, CHAR_SPACE);
            footerLenDiff = boardOvl->lengths[mBD_FIELD_FOOTER] - tFooterLen;

            mem_copy(tFooter, boardOvl->dstP->content.footer, 16);

            tFooterP = tFooter;
            for (i = 0; i < tFooterLen; i++) {
                if (*tFooterP != CHAR_SPACE) {
                    break;
                }
                tFooterP++;
            }

            mem_clear(boardOvl->dstP->content.footer, 16, CHAR_SPACE);
            if (footerLenDiff < 16) {
                mem_copy(boardOvl->dstP->content.footer + footerLenDiff, tFooterP, tFooterLen - i);
            }

            mailHeader->unk_00 = boardOvl->mail.content.headerBackStart;
            mem_copy(mailHeader->unk_02, boardOvl->dstP->content.header, 10);
            mem_copy(mailHeader->unk_0C, boardOvl->dstP->content.footer, 16);

            if (menuInfo->data1 != -1) {
                mPr_SetPossessionItem(common_data.privateInfo, menuInfo->data1, 0, 0);
                (*mBD_OVERLAY(submenu)->tagOvl->initTagDataItemWinProc)(submenu);
            }

            mBD_end_board(submenu, menuInfo);
        } else if (editEndChkMenu->data1 == 1) {
            mSM_open_submenu_new(submenu, SUBMENU_PROGRAM_EDITOR, mED_TYPE_BOARD, 0, (s32)boardOvl->mail.content.body);
            boardOvl->field = mBD_FIELD_BODY;
            boardOvl->headerPos = mBD_HEADER_POS_PRE_NAME;
            menuInfo->procStatus = mSM_OVL_PROC_PLAY;
        } else {
            mBD_end_board(submenu, menuInfo);
        }

        if (editorOvl != NULL) {
            editorOvl->cursorIdx = 0;
            editorOvl->cursorCol = 0;
            editorOvl->cursorRow = 0;
        }
    }
}

void mBD_move_End(Submenu* submenu, mSM_MenuInfo_c* menuInfo) {
    (*mBD_OVERLAY(submenu)->moveEndProc)(submenu, menuInfo);
}

void mBD_board_ovl_move(Submenu* submenu) {
    mSM_MenuInfo_c* menuInfo = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_BOARD];

    (*menuInfo->preMoveFunc)(submenu);
    (*mBD_ovl_move_proc[menuInfo->procStatus])(submenu, menuInfo);
}

// draw the paper (frame) display lists
void mBD_set_frame_dl(GraphicsContext* graph, mSM_MenuInfo_c* menuInfo, f32 posX, f32 posY, mBD_Ovl_c* boardOvl) {
    int paperType = boardOvl->mail.content.paperType;

    SegmentBaseAddress[12] = (u32)menuInfo->segment + 0x80000000;

    Matrix_scale(16.0f, 16.0f, 1.0f, MTXMODE_NEW);
    Matrix_translate(posX, posY, 140.0f, MTXMODE_APPLY);

    OPEN_POLY_OPA_DISP(graph);

    gSPSegment(__polyOpa++, 0xC, menuInfo->segment);
    gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(__polyOpa++, D_C000120);
    gSPSegment(__polyOpa++, 0xC, boardOvl->paperDataP - boardOvl->paperDataOfs);
    gSPDisplayList(__polyOpa++, paper_disp_model[paperType]);

    if (paper_disp_sen_model[paperType] != NULL) {
        gSPSegment(__polyOpa++, 0xC, menuInfo->segment);
        gSPDisplayList(__polyOpa++, D_C000158);
        gSPSegment(__polyOpa++, 0xC, boardOvl->paperDataP - boardOvl->paperDataOfs);
        gSPDisplayList(__polyOpa++, paper_disp_sen_model[paperType]);
    }

    gSPSegment(__polyOpa++, 0xC, menuInfo->segment);

    CLOSE_POLY_OPA_DISP(graph);
}

// the little "point" marker shown when the cursor is on the name
void mBD_set_point(Submenu* submenu, Game_Play* game, f32 posX, f32 posY) {
    GraphicsContext* graph = game->state.gfxCtx;
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;

    boardOvl->ofsX = (f32)(boardOvl->mail.content.headerBackStart + 3) * 12.0f + posX + -96.0f;
    boardOvl->ofsY = posY - -76.0f;

    Matrix_scale(16.0f, 16.0f, 1.0f, MTXMODE_NEW);
    Matrix_translate(boardOvl->ofsX, boardOvl->ofsY, 140.0f, MTXMODE_APPLY);

    OPEN_POLY_OPA_DISP(graph);

    gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(__polyOpa++, D_C000120);
    gSPDisplayList(__polyOpa++, D_C0001C0);
    gSPDisplayList(__polyOpa++, D_C000040);

    CLOSE_POLY_OPA_DISP(graph);
}

// place the text cursor
void mBD_set_cursol(Submenu* submenu, Game_Play* game, f32 posX, f32 posY) {
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;
    mED_Ovl_c* editorOvl = mBD_OVERLAY(submenu)->editorOvl;

    if (boardOvl->headerPos == mBD_HEADER_POS_ON_NAME) {
        mBD_set_point(submenu, game, posX, posY);
    } else {
        int idx;
        f32 ofsX;
        f32 ofsY;
        f32 line;

        idx = editorOvl->cursorCol - 1;

        if (boardOvl->field == mBD_FIELD_HEADER) {
            if (boardOvl->headerPos == mBD_HEADER_POS_POST_NAME) {
                idx += 6;
            }
            ofsX = (f32)idx + 0.3f;
            ofsY = 0.0f;
            line = 0.0f;
        } else if (boardOvl->field == mBD_FIELD_BODY) {
            ofsX = (f32)idx + 0.3f;
            ofsY = (f32)(editorOvl->cursorRow + 1);
            line = 12.0f;
        } else {
            ofsX = (f32)(idx - boardOvl->lengths[mBD_FIELD_FOOTER] + 16) + 0.3f;
            ofsY = 7.0f;
            line = 24.0f;
        }

        (*mBD_OVERLAY(submenu)->editorOvl->cursolDraw)(submenu, &game->state, 64.0f + posX + ofsX * 12.0f,
                                                       36.0f - (posY - ofsY * 16.0f - line));
    }
}

// footer (right aligned, "From ...")
void mBD_set_writing_footer(Submenu* submenu, Game_Play* game, f32 posX, f32 posY, Color_RGBA8* color) {
    int footerLen = mBD_OVERLAY(submenu)->boardOvl->lengths[mBD_FIELD_FOOTER];

    posX += 12.0f * (16 - footerLen);
    mFont_SetLineStrings(&game->state, mBD_OVERLAY(submenu)->boardOvl->mail.content.footer, footerLen, posX, posY,
                         color->r, color->g, color->b, 255, FALSE, FALSE, 1.0f, 1.0f, 0);
}

// body, 6 lines of up to 16 chars
void mBD_set_writing_body(Submenu* submenu, mSM_MenuInfo_c* menuInfo, Game_Play* game, f32 posX, f32* posY, f32* endX,
                          f32* endY, Color_RGBA8* color) {
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;
    u8* strP = boardOvl->mail.content.body;
    int bodyMax = boardOvl->lengths[mBD_FIELD_BODY];
    int i;
    int col;
    int dispCol;
    int retFlg;

    for (i = 0; i < 6; i++) {
        retFlg = FALSE;
        for (col = 0; col < bodyMax;) {
            if (strP[col] == 0xCD) {
                col++;
                retFlg = TRUE;
                break;
            }
            col++;
            if (col >= 16) {
                break;
            }
        }

        if (col != 0) {
            if (retFlg && menuInfo->procStatus != mSM_OVL_PROC_PLAY) {
                dispCol = col - 1;
            } else {
                dispCol = col;
            }

            if (dispCol != 0) {
                mFont_SetLineStrings(&game->state, strP, dispCol, posX, *posY, color->r, color->g, color->b, 255,
                                     FALSE, FALSE, 1.0f, 1.0f, 0);
            }

            bodyMax -= col;
            strP += col;

            if (bodyMax == 0) {
                if (i != 5 && (retFlg || col == 16)) {
                    *endX = posX - 160.0f;
                    *endY = 120.0f - (*posY + 16.0f);
                } else {
                    *endX = (f32)col * 12.0f + posX - 160.0f;
                    *endY = 120.0f - *posY;
                }
            }
        }

        *posY += 16.0f;
    }
}

// header ("<front text> NAME <back text>")
void mBD_set_writing_header(Submenu* submenu, Game_Play* game, mSM_MenuInfo_c* menuInfo, f32 posX, f32 posY,
                            Color_RGBA8* color) {
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;
    MailContent* mailContent = &boardOvl->mail.content;
    u8 tmpHeader[16];
    int tmpNum;

    if (menuInfo->procStatus == mSM_OVL_PROC_PLAY) {
        if (boardOvl->field == mBD_FIELD_HEADER) {
            tmpNum = 6;
        } else {
            tmpNum = boardOvl->headerNameLen;
        }

        // front part of the header
        mFont_SetLineStrings(&game->state, mailContent->header, mailContent->headerBackStart, posX, posY, color->r,
                             color->g, color->b, 255, FALSE, FALSE, 1.0f, 1.0f, 0);

        // recipient name (red)
        posX += (f32)(u32)mailContent->headerBackStart * 12.0f;
        mFont_SetLineStrings(&game->state, boardOvl->mail.header.recipient.personalID.playerName, tmpNum, posX, posY,
                             185, 0, 0, 255, FALSE, FALSE, 1.0f, 1.0f, 0);

        // back part of the header
        if (mailContent->headerBackStart < 10) {
            posX += (f32)tmpNum * 12.0f;
            mFont_SetLineStrings(&game->state, mailContent->header + mailContent->headerBackStart,
                                 boardOvl->lengths[mBD_FIELD_HEADER] - mailContent->headerBackStart, posX, posY,
                                 color->r, color->g, color->b, 255, FALSE, FALSE, 1.0f, 1.0f, 0);
        }
    } else {
        if (mailContent->mailType == 2 || mailContent->mailType == 3 || mailContent->mailType == 5) {
            mem_copy(tmpHeader, mailContent->header, 10);
            tmpNum = 10;
        } else {
            mem_copy(tmpHeader, mailContent->header, mailContent->headerBackStart);
            mem_copy(tmpHeader + mailContent->headerBackStart,
                     (u8*)boardOvl->mail.header.recipient.personalID.playerName, boardOvl->headerNameLen);
            if (mailContent->headerBackStart < 10) {
                mem_copy(tmpHeader + (boardOvl->headerNameLen + mailContent->headerBackStart),
                         mailContent->header + mailContent->headerBackStart,
                         boardOvl->lengths[mBD_FIELD_HEADER] - mailContent->headerBackStart);
            }
            tmpNum = boardOvl->lengths[mBD_FIELD_HEADER] + boardOvl->headerNameLen;
        }

        mFont_SetLineStrings(&game->state, tmpHeader, tmpNum, posX, posY, color->r, color->g, color->b, 255, FALSE,
                             FALSE, 1.0f, 1.0f, 0);
    }
}

void mBD_set_character(Submenu* submenu, Game_Play* game, mSM_MenuInfo_c* menuInfo, f32 posX, f32 posY) {
    int paperType = mBD_OVERLAY(submenu)->boardOvl->mail.content.paperType;
    Color_RGBA8* color = &mBD_letter_color[paperType];
    f32 baseX = 64.0f + posX;
    f32 baseY = 36.0f - posY;
    f32 endX;
    f32 endY;

    mBD_set_writing_header(submenu, game, menuInfo, baseX, baseY, color);

    baseY += 28.0f;
    endX = baseX - 160.0f;
    endY = 120.0f - baseY;
    mBD_set_writing_body(submenu, menuInfo, game, baseX, &baseY, &endX, &endY, color);

    baseY += 12.0f;
    mBD_set_writing_footer(submenu, game, baseX, baseY, color);

    if (menuInfo->procStatus == mSM_OVL_PROC_PLAY && mBD_OVERLAY(submenu)->editorOvl != NULL) {
        mBD_set_cursol(submenu, game, posX, posY);
        (*mBD_OVERLAY(submenu)->editorOvl->endCodeDraw)(submenu, &game->state, endX, endY);
    }
}

void mBD_set_dl(Submenu* submenu, Game_Play* game, mSM_MenuInfo_c* menuInfo) {
    GraphicsContext* graph = game->state.gfxCtx;
    f32 posX = menuInfo->position[0];
    f32 posY = menuInfo->position[1];

    mBD_set_frame_dl(graph, menuInfo, posX, posY, mBD_OVERLAY(submenu)->boardOvl);

    if (menuInfo->data0 == mSM_BD_OPEN_READ || mBD_OVERLAY(submenu)->boardOvl->first == 0) {
        (*mBD_OVERLAY(submenu)->setCharMatrixProc)(graph);
        mBD_set_character(submenu, game, menuInfo, posX, posY);
    }
}

void mBD_board_ovl_draw(Submenu* submenu, Game_Play* game) {
    mSM_MenuInfo_c* menuInfo = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_BOARD];

    (*menuInfo->preDrawFunc)(submenu, game);

    if (mBD_OVERLAY(submenu)->boardOvl->paperDataP != NULL) {
        mBD_set_dl(submenu, game, menuInfo);
    }
}

void mBD_board_ovl_set_proc(Submenu* submenu) {
    mSM_Control_c* menuControl = &mBD_OVERLAY(submenu)->menuControl;

    menuControl->menuMoveFunc = mBD_board_ovl_move;
    menuControl->menuDrawFunc = mBD_board_ovl_draw;
}

void mBD_board_ovl_init(Submenu* submenu) {
    mSM_MenuInfo_c* menuInfo = &mBD_OVERLAY(submenu)->menuInfo[SUBMENU_PROGRAM_BOARD];
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;

    mBD_OVERLAY(submenu)->menuControl.animationFlag = 0;
    menuInfo->procStatus = mSM_OVL_PROC_MOVE;
    menuInfo->moveDrt = mSM_MOVE_IN_TOP;

    boardOvl->field = mBD_FIELD_BODY;
    boardOvl->centerLine = 2;
    boardOvl->headerPos = mBD_HEADER_POS_PRE_NAME;
    boardOvl->dstP = (Mail_c*)menuInfo->data2;

    if (menuInfo->data0 == mSM_BD_OPEN_WRITE) {
        // writing a brand new letter / notice
        MailHeaderCommon* mailHeader = &common_data.privateInfo->savedMailHeader;
        u32 nameLen;

        boardOvl->first = 1;
        menuInfo->nextProcStatus = mSM_OVL_PROC_PLAY;
        mSM_open_submenu_new(submenu, SUBMENU_PROGRAM_ADDRESS, 0, 0, (s32)boardOvl->mail.content.body);
        mMl_init_mail(&boardOvl->mail, &common_data.privateInfo->playerId);
        boardOvl->mail.content.paperType = common_data.privateInfo->inventory.pockets[menuInfo->data1] - 0x2000;

        if (mailHeader->unk_00 == -1) {
            // no saved header/footer yet: default "To <name>" / "From <player>"
            nameLen = mBD_ML_STRLEN(common_data.privateInfo->playerId.playerName, PLAYER_NAME_LEN, CHAR_SPACE);
            mem_copy(boardOvl->mail.content.header, mBD_header_str, 3);
            mem_copy(boardOvl->mail.content.footer, (u8*)common_data.privateInfo->playerId.playerName, nameLen);
            mem_copy(boardOvl->mail.content.footer + nameLen, mBD_footer_str, 2);
            boardOvl->mail.content.headerBackStart = 0;
        } else {
            // reuse the previously saved header/footer
            mem_copy(boardOvl->mail.content.header, mailHeader->unk_02, 10);
            mem_copy(boardOvl->mail.content.footer, mailHeader->unk_0C, 16);
            boardOvl->mail.content.headerBackStart = mailHeader->unk_00;
        }
    } else {
        // existing letter
        mMl_copy_mail(&boardOvl->mail, (Mail_c*)menuInfo->data2);
        if (menuInfo->data0 == mSM_BD_OPEN_READ) {
            menuInfo->nextProcStatus = mSM_OVL_PROC_WAIT;
        } else {
            menuInfo->nextProcStatus = mSM_OVL_PROC_PLAY;
            mSM_open_submenu_new(submenu, SUBMENU_PROGRAM_ADDRESS, 1, 0, (s32)boardOvl->mail.content.body);
        }
        boardOvl->first = 0;
        boardOvl->headerNameLen =
            mBD_ML_STRLEN(boardOvl->mail.header.recipient.personalID.playerName, PLAYER_NAME_LEN, CHAR_SPACE);
    }

    boardOvl->lengths[mBD_FIELD_HEADER] = mBD_ML_STRLEN(boardOvl->mail.content.header, MAIL_HEADER_LEN, CHAR_SPACE);
    boardOvl->lengths[mBD_FIELD_FOOTER] = mBD_ML_STRLEN(boardOvl->mail.content.footer, MAIL_FOOTER_LEN, CHAR_SPACE);
    boardOvl->lengths[mBD_FIELD_BODY] = mBD_ML_STRLEN(boardOvl->mail.content.body, MAIL_BODY_LEN, CHAR_SPACE);

    if (boardOvl->mail.content.headerBackStart > 10) {
        boardOvl->mail.content.headerBackStart = 10;
    }
    if (boardOvl->lengths[mBD_FIELD_HEADER] < boardOvl->mail.content.headerBackStart) {
        boardOvl->lengths[mBD_FIELD_HEADER] = boardOvl->mail.content.headerBackStart;
    }

    boardOvl->ofsX = 0.0f;
    boardOvl->ofsY = 76.0f;

    if (boardOvl->lengths[mBD_FIELD_FOOTER] != 0) {
        // left-justify the footer
        int i;
        u8* tmpFooterP;
        u8 tmpFooter[17];

        tmpFooterP = tmpFooter;
        mem_copy(tmpFooterP, boardOvl->mail.content.footer, 16);

        for (i = 0; i < boardOvl->lengths[mBD_FIELD_FOOTER]; i++) {
            if (*tmpFooterP != CHAR_SPACE) {
                break;
            }
            tmpFooterP++;
        }

        mem_clear(boardOvl->mail.content.footer, 16, CHAR_SPACE);
        mem_copy(boardOvl->mail.content.footer, tmpFooterP, boardOvl->lengths[mBD_FIELD_FOOTER] - i);
    }
}

// DMA the selected stationery's display lists
void mBD_board_letter_dma(Submenu* submenu) {
    mBD_Ovl_c* boardOvl = mBD_OVERLAY(submenu)->boardOvl;

    boardOvl->paperDataOfs = paper_data_offset[boardOvl->mail.content.paperType];

    if (boardOvl->paperDataP == NULL) {
        boardOvl->paperDataP = mBD_OVERLAY(submenu)->allocP;
        mBD_OVERLAY(submenu)->allocP += 0x1400;
    }

    DmaMgr_RequestSyncDebug(boardOvl->paperDataP, boardOvl->paperDataOfs + (u32)D_AE3000 - (u32)D_C000000,
                            paper_data_offset[boardOvl->mail.content.paperType + 1] - boardOvl->paperDataOfs,
                            "../m_board_ovl.c", 1228);

    boardOvl->paperDataOfs -= (u32)D_C000000_b;
}

void mBD_board_ovl_construct(Submenu* submenu) {
    mBD_Overlay_c* ovl = mBD_OVERLAY(submenu);

    if (ovl->boardOvl == NULL) {
        mSM_MenuInfo_c* menuInfo = &ovl->menuInfo[SUBMENU_PROGRAM_BOARD];

        (*mBD_OVERLAY(submenu)->dataLoadProc)(submenu, &menuInfo->segment, &mBD_data_dlftbl);
        ovl->boardOvl = &mBD_board_ovl_data;
    }

    mBD_board_ovl_init(submenu);
    mBD_board_ovl_set_proc(submenu);
    mBD_board_letter_dma(submenu);
}

void mBD_board_ovl_destruct(Submenu* submenu) {
    mBD_OVERLAY(submenu)->boardOvl = NULL;
}
