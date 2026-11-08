#include "mED_editor_ovl.h"
#include "global.h"

#include "m_common_data.h"
#include "m_controller.h"
#include "m_font.h"
#include "m_lib.h"
#include "m_mail.h"
#include "audio.h"
#include "sys_matrix.h"
#include "overlays/submenu/board_ovl/m_board_ovl.h"

// The submenu, menu and board overlay structs are accessed through raw offsets for now.
#define mED_OVL_BASE(submenu) ((u8*)(submenu)->overlay)
#define mED_EDIT_OVL(submenu) (*(mED_Ovl_c**)(mED_OVL_BASE(submenu) + 0x106E0))
#define mED_BOARD_OVL(submenu) (*(u8**)(mED_OVL_BASE(submenu) + 0x106E4))
#define mED_MENU(submenu) ((u8*)(mED_OVL_BASE(submenu) + 0x10358))

// menu (mSM_MenuInfo_c) fields used by this overlay
#define mED_MENU_PROC_STATUS(menuInfo) (*(s32*)((u8*)(menuInfo) + 0x04))
#define mED_MENU_PRE_MENU_TYPE(menuInfo) (*(s32*)((u8*)(menuInfo) + 0x08))
#define mED_MENU_PRE_MOVE_FUNC(menuInfo) (*(void (**)(Submenu*, u8*))((u8*)(menuInfo) + 0x0C))
#define mED_MENU_PRE_DRAW_FUNC(menuInfo) (*(void (**)(Submenu*, Game*))((u8*)(menuInfo) + 0x10))
#define mED_MENU_POS_X(menuInfo) (*(f32*)((u8*)(menuInfo) + 0x18))
#define mED_MENU_POS_Y(menuInfo) (*(f32*)((u8*)(menuInfo) + 0x1C))
#define mED_MENU_SEGMENT(menuInfo) (*(u32*)((u8*)(menuInfo) + 0x28))
#define mED_MENU_DATA0(menuInfo) (*(s32*)((u8*)(menuInfo) + 0x38))

// controller values living in gamePT
#define mED_GAME_STICK_R() (gamePT->controller.adjustedR)
#define mED_GAME_STICK_ANGLE() (gamePT->controller.moveAngle)

// board overlay fields
#define mED_BOARD_HEADER_POS(b) (*(u8*)((b) + 0x02))
#define mED_BOARD_HEADER_BACK_START(b) (*(u8*)((b) + 0x2F))

#define mED_SET_PRIM(gp, c) gDPSetPrimColor(gp, 0, 255, (c).r, (c).g, (c).b, 255)
#define mED_SET_ENV(gp, c) gDPSetEnvColor(gp, (c).r, (c).g, (c).b, 255)

typedef void (*mED_EDIT_FUNC)(Submenu*, u8*);
typedef void (*mED_OVL_PROC)(Submenu*, u8*);
typedef void (*mEDMoveChgBaseProc)(void* menuInfo, s32 moveType);

// SubmenuOverlay up to the board overlay pointer; only what the editor reads is typed.
typedef struct mED_OvlLayout_c {
    /* 0x00000 */ u8 segment[0x10088];
    /* 0x10088 */ mSM_MenuInfo_c menuInfo[SUBMENU_PROGRAM_MAX];
    /* 0x10670 */ mSM_Control_c menuControl;
    /* 0x106A4 */ void* returnFuncProc;
    /* 0x106A8 */ void (*moveMoveProc)(Submenu*, mSM_MenuInfo_c*);
    /* 0x106AC */ void (*moveEndProc)(Submenu*, mSM_MenuInfo_c*);
    /* 0x106B0 */ void (*moveChgBaseProc)(mSM_MenuInfo_c*, s32);
    /* 0x106B4 */ void (*setCharMatrixProc)(void*);
    /* 0x106B8 */ void* cbufCopyProc;
    /* 0x106BC */ void* setDrawModeProc;
    /* 0x106C0 */ void* drawItemProc;
    /* 0x106C4 */ void* drawMailProc;
    /* 0x106C8 */ void* setupViewProc;
    /* 0x106CC */ struct_8085E9B0_unk_106CC dataLoadProc;
    /* 0x106D0 */ void* tagOvl;
    /* 0x106D4 */ void* handOvl;
    /* 0x106D8 */ void* haniwaPortraitOvl;
    /* 0x106DC */ void* inventoryOvl;
    /* 0x106E0 */ mED_Ovl_c* editorOvl;
    /* 0x106E4 */ mBD_Ovl_c* boardOvl;
    /* 0x106E8 */ void* addressOvl;
} mED_OvlLayout_c;

typedef struct {
    /* 0x00 */ Color_RGBA8 wakuPrim;
    /* 0x04 */ Color_RGBA8 wakuEnv;
    /* 0x08 */ Color_RGBA8 selPrim;
    /* 0x0C */ Color_RGBA8 selEnv;
    /* 0x10 */ Color_RGBA8 basePrim;
    /* 0x14 */ Color_RGBA8 baseEnv;
    /* 0x18 */ Color_RGBA8 labelPrim;
} mED_Model_Color_Data; // size = 0x1C


extern f32 mFont_SetLineStrings(Game*, char*, s32 len, f32 x, f32 y, s32 r, s32 g, s32 b, s32 a, s32, s32, f32 sx,
                                f32 sy, s32);
// char code -> voice se number
extern s32 func_8009F730_jp(s32 code);

// Segment 0x0C display lists (sm_editor static segment)
extern Gfx D_C000040[];
extern Gfx D_C000C10[];
extern Gfx D_C000C40[];
extern Gfx D_C000CD8[];
extern Gfx D_C000D70[];
extern Gfx D_C000E08[];
extern Gfx D_C000EC0[];
extern Gfx D_C000F28[];
extern Gfx D_C000F90[];
extern Gfx D_C000FF8[];
extern Gfx D_C001060[];
extern Gfx D_C0010B8[];
extern Gfx D_C001110[];
extern Gfx D_C0015F8[];
extern Gfx D_C001660[];
extern Gfx D_C0016D0[];
extern Gfx D_C001730[];
extern Gfx D_C0017A0[];
extern Gfx D_C0017B8[];
extern Gfx D_C0017D0[];
extern Gfx D_C0017F8[];
extern Gfx D_C016E90[];
extern Gfx D_C016FD0[];

// .data and .bss, still provided by the splat generated asm.
extern mED_Ovl_c mED_edit_ovl_data;
extern u8 mED_hiragana_table[10][5];
extern u8 mED_katakana_table[10][5];
extern u8 mED_kigou_table[30];
extern u8 mED_number_table[10];
extern u8 mED_alphabet_table[30];
extern s32 mED_page_num_max[5];
extern u8* mED_page_data_table[5];
extern s32 mED_offset_max[5];
extern s16 mED_edit_line[5];
extern u16 mED_se_no[10];
extern u8 mED_exchange_se_list[256];
extern u8 mED_ornament_table[256];
extern s16 mED_letter_table_col[3];
extern mED_EDIT_FUNC mED_letter_base_func[3];
extern mED_EDIT_FUNC mED_edit_func[5];
extern mED_OVL_PROC mED_ovl_move_proc[5];
extern char* lat_sousa_button_pallet_tbl[2];
extern unsigned short* lat_sousa_start_button_tbl[2];
extern unsigned short* lat_sousa_b_button_tbl[2];
extern unsigned short* lat_sousa_a_button_tbl[2];
extern char* lat_sousa_c_r_button_tbl[2];
extern char* lat_sousa_c_l_button_tbl[2];
extern char* lat_sousa_c_u_button_tbl[2];
extern char* lat_sousa_c_d_button_tbl[2];
extern char* lat_sousa_z_button_tbl[2];
extern char* lat_sousa_r_button_tbl[2];
extern char* lat_sousa_waku1_tex_tbl[3];
extern char* lat_sousa_st_tex_tbl[9];
extern Gfx* mED_display_list_table[5];
extern mED_Model_Color_Data mED_model_color[3];
extern Color_RGBA8 mED_rstr_color[6];
extern f32 mED_ring_in_return_pos[3][2];
extern u8 mED_return_str[3];
extern f32 mED_ring_in_pos[2][10][2];
extern f32 mED_ring_out_pos[3][5][2];
extern f32 mED_scale_kind[3];
extern struct_func_8085D43C_jp_arg2 mED_data_dlftbl;

s32 mED_get_code(mED_Ovl_c* editorOvl, s32 pageTopNum, s32 num, s32 firstFlg) {
    s32 shiftMode = editorOvl->shiftMode;
    s32 offset;

    if (shiftMode == 0 || shiftMode == 2) {
        if (firstFlg) {
            offset = (pageTopNum + num) * 5;
        } else {
            offset = pageTopNum * 5 + num;
        }
    } else {
        offset = pageTopNum + num;
    }

    offset = offset % mED_offset_max[shiftMode];
    return mED_page_data_table[shiftMode][offset];
}

void mED_set_stick_area(Submenu* submenu) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);
    f32 stickR = mED_GAME_STICK_R();
    u8* stickAreaP = &editorOvl->stickArea;
    s32 angle;

    stickAreaP[1] = stickAreaP[0]; // lastStickArea = stickArea

    if (stickR < 0.2f) {
        stickAreaP[0] = mED_STICK_AREA_CENTER;
    } else {
        angle = mED_GAME_STICK_ANGLE();

        if (angle < -0x7000) {
            stickAreaP[0] = mED_STICK_AREA_LEFT;
        } else if (angle >= -0x7000 && angle < -0x5000) {
            stickAreaP[0] = mED_STICK_AREA_BOTTOM_LEFT;
        } else if (angle < -0x3000) {
            stickAreaP[0] = mED_STICK_AREA_BOTTOM;
        } else if (angle < -0x1000) {
            stickAreaP[0] = mED_STICK_AREA_BOTTOM_RIGHT;
        } else if (angle < 0x1000) {
            stickAreaP[0] = mED_STICK_AREA_RIGHT;
        } else if (angle < 0x3000) {
            stickAreaP[0] = mED_STICK_AREA_TOP_RIGHT;
        } else if (angle < 0x5000) {
            stickAreaP[0] = mED_STICK_AREA_TOP;
        } else if (angle < 0x7000) {
            stickAreaP[0] = mED_STICK_AREA_TOP_LEFT;
        } else {
            stickAreaP[0] = mED_STICK_AREA_LEFT;
        }
    }

    if (editorOvl->stickArea != editorOvl->lastStickArea) {
        editorOvl->stickAreaChanged = TRUE;

        if (stickAreaP[0] != mED_STICK_AREA_CENTER && stickAreaP[0] != mED_STICK_AREA_BOTTOM_LEFT &&
            stickAreaP[0] != mED_STICK_AREA_BOTTOM_RIGHT && stickAreaP[0] != mED_STICK_AREA_BOTTOM) {
            if ((editorOvl->shiftMode != 0 && editorOvl->shiftMode != 2) || editorOvl->consonantNum != -1) {
                sAdo_SysTrgStart(0x40F);
            } else {
                sAdo_SysTrgStart(0x411);
            }
        }
    } else {
        editorOvl->stickAreaChanged = FALSE;
    }
}

s32 mED_check_move_cursol(mED_Ovl_c* editorOvl) {
    u32 buttons = (getButton() | getTrigger()) & (CONT_B | CONT_F | CONT_C | CONT_D | CONT_E);

    if (buttons == 0 || (editorOvl->consonantNum != -1 && (buttons & CONT_B))) {
        editorOvl->lastButtons = buttons;
        editorOvl->buttonRepeatTimer = 15;
        editorOvl->buttonRepeatAccelTimer = 13;
        editorOvl->buttonRepeatStarted = 0;
        return mED_COMMAND_NONE;
    }

    if (buttons == editorOvl->lastButtons) {
        if (editorOvl->buttonRepeatStarted == 1) {
            if (editorOvl->buttonRepeatAccelTimer > 0) {
                editorOvl->buttonRepeatAccelTimer--;
            }
        }

        if (editorOvl->buttonRepeatTimer > 0) {
            editorOvl->buttonRepeatTimer--;
            return mED_COMMAND_9;
        }

        if (editorOvl->buttonRepeatAccelTimer == 0) {
            editorOvl->buttonRepeatTimer = 1;
        } else {
            editorOvl->buttonRepeatTimer = 3;
            editorOvl->buttonRepeatStarted = 1;
        }
    } else {
        editorOvl->lastButtons = buttons;
        editorOvl->buttonRepeatTimer = 15;
        editorOvl->buttonRepeatAccelTimer = 13;
        editorOvl->buttonRepeatStarted = 0;
    }

    if (buttons & CONT_B) {
        return mED_COMMAND_BACKSPACE;
    }
    if (buttons & CONT_C) {
        return mED_COMMAND_CURSOL_LEFT;
    }
    if (buttons & CONT_D) {
        return mED_COMMAND_CURSOL_LOWER;
    }
    if (buttons & CONT_E) {
        return mED_COMMAND_CURSOL_UPPER;
    }
    if (buttons & CONT_F) {
        return mED_COMMAND_CURSOL_RIGHT;
    }

    return mED_COMMAND_NONE;
}

s32 mEditor_Check_Shift_Mode(mED_Ovl_c* editorOvl) {
    if (chkTrigger(Z_TRIG)) {
        editorOvl->shiftMode++;
        editorOvl->pageTopNum = 0;
        editorOvl->consonantNum = -1;
        if (editorOvl->shiftMode >= 5) {
            editorOvl->shiftMode = 0;
        }
        return TRUE;
    }

    return FALSE;
}

s32 mEditor_Check_Page_Mode(mED_Ovl_c* editorOvl) {
    if ((editorOvl->stickArea == mED_STICK_AREA_BOTTOM && (chkTrigger(A_BUTTON) || editorOvl->stickAreaChanged != 0)) ||
        (editorOvl->consonantNum != -1 && chkTrigger(B_BUTTON))) {
        if (editorOvl->consonantNum == -1) {
            editorOvl->pageTopNum += 3;
            editorOvl->rotateTimer = 3;
        } else {
            editorOvl->consonantNum = -1;
        }
        return TRUE;
    }

    return FALSE;
}

s32 mEditor_AreaToConsonant(mED_Ovl_c* editorOvl) {
    s32 stickArea = editorOvl->stickArea;

    if (stickArea < 0 || stickArea >= 5) {
        return -1;
    }

    return editorOvl->pageTopNum + stickArea;
}

s32 mEditor_Input_Consonant_Num(mED_Ovl_c* editorOvl) {
    s32 consonantIndex;

    if ((editorOvl->shiftMode == 0 || editorOvl->shiftMode == 2) && editorOvl->consonantNum == -1) {
        consonantIndex = mEditor_AreaToConsonant(editorOvl);
        if (consonantIndex != -1 && chkTrigger(A_BUTTON)) {
            editorOvl->consonantNum = consonantIndex;
            return TRUE;
        }
    }

    return FALSE;
}

s32 mEditor_Get_Now_Code(mED_Ovl_c* editorOvl) {
    s32 stickArea = editorOvl->stickArea;
    s32 shiftMode = editorOvl->shiftMode;
    s32 pageTopNum;

    if (stickArea < 0 || stickArea >= 5 || ((shiftMode == 0 || shiftMode == 2) && editorOvl->consonantNum == -1)) {
        return -1;
    }

    if (editorOvl->consonantNum == -1) {
        pageTopNum = editorOvl->pageTopNum;
    } else {
        pageTopNum = editorOvl->consonantNum;
    }

    return mED_get_code(editorOvl, pageTopNum, stickArea, FALSE);
}

s32 mED_check_now_code(mED_Ovl_c* editorOvl) {
    s32 code;

    if (chkTrigger(A_BUTTON)) {
        code = mEditor_Get_Now_Code(editorOvl);
        if (code != -1) {
            editorOvl->nowCode = code;
            editorOvl->consonantNum = -1;
            return TRUE;
        }
    }

    return FALSE;
}

s32 mED_check_output_code(mED_Ovl_c* editorOvl) {
    if (mEditor_Check_Shift_Mode(editorOvl)) {
        sAdo_SysTrgStart(0x37);
        return mED_COMMAND_NONE;
    }

    if (mEditor_Check_Page_Mode(editorOvl)) {
        sAdo_SysTrgStart(0x38);
        return mED_COMMAND_NONE;
    }

    if (mEditor_Input_Consonant_Num(editorOvl)) {
        sAdo_SysTrgStart(0x32);
        return mED_COMMAND_NONE;
    }

    if (mED_check_now_code(editorOvl)) {
        return mED_COMMAND_OUTPUT_CODE;
    }

    return mED_COMMAND_NONE;
}

void mED_set_command(Submenu* submenu) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);

    if (chkTrigger(START_BUTTON)) {
        editorOvl->command = mED_COMMAND_END_EDIT;
    } else if (chkTrigger(R_TRIG)) {
        editorOvl->command = mED_COMMAND_EXCHANGE_CODE;
    } else {
        editorOvl->command = mED_check_move_cursol(editorOvl);
        if (editorOvl->command == mED_COMMAND_NONE) {
            editorOvl->command = mED_check_output_code(editorOvl);
        }
    }
}

void mED_init(Submenu* submenu, u8* menuInfo) {
    struct editor_ovl_s* editorOvl = *(struct editor_ovl_s**)((u8*)submenu->overlay + 0x106E0);

    editorOvl->stickArea = mED_STICK_AREA_CENTER;
    editorOvl->lastStickArea = mED_STICK_AREA_CENTER;
    editorOvl->stickAreaChanged = 0;

    editorOvl->shiftMode = 0;
    editorOvl->pageTopNum = 0;
    editorOvl->consonantNum = -1;
    editorOvl->command = 0;
    editorOvl->nowCode = 0;
    editorOvl->animFrame = 0;
    editorOvl->maxLineNo = mED_edit_line[*(s32*)(menuInfo + 0x38)];
    editorOvl->inputStr = *(char**)(menuInfo + 0x40);

    if (*(s32*)(menuInfo + 0x3C) > 0) {
        editorOvl->inputLength = *(s32*)(menuInfo + 0x3C);
        editorOvl->nowStrLen = mMl_strlen(editorOvl->inputStr, editorOvl->inputLength * editorOvl->maxLineNo, ' ');
    } else if (*(s32*)(menuInfo + 0x38) == 0) {
        editorOvl->inputLength = 16;
        editorOvl->nowStrLen = *(u8*)(*(u8**)((u8*)submenu->overlay + 0x106E4) + 6);
    }

    editorOvl->cursorIdx = 0;
#if 1
    // Temporary hack to make the character set start at ABC's.
    // It can't be defined where shiftMode is set to 0, because that causes overlay errors and mismatched
    // instruction counts.
    editorOvl->shiftMode = 3;
#else
    editorOvl->cursorCol = 0;
    editorOvl->cursorRow = 0;
#endif
    editorOvl->exchangeCode = -1;
}

void mED_set_se(Submenu* submenu) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);
    s32 command = editorOvl->command;
    s32 se;

    if (command == mED_COMMAND_OUTPUT_CODE) {
        se = func_8009F730_jp(editorOvl->nowCode);
        if (se == 0x52 || se == 0x50 || se == 0x4F || se == 0x51 || se == 0x54 || se == 0x55 || se == -1) {
            se = 0x4E;
        }
        sAdo_VoiceSe(se, 0, 0x20);
    } else if (command == mED_COMMAND_EXCHANGE_CODE) {
        if (editorOvl->exchangeCode != -1 && mED_exchange_se_list[editorOvl->exchangeCode] != 0) {
            se = func_8009F730_jp(editorOvl->exchangeCode);
            sAdo_VoiceSe(se, 0, 0x20);
        } else {
            sAdo_SysTrgStart(0x36);
        }
    } else {
        sAdo_SysTrgStart(mED_se_no[command]);
    }
}

void mED_set_idx(mED_Ovl_c* editorOvl) {
    u8* strP = (u8*)editorOvl->inputStr;
    s32 line = editorOvl->cursorRow;
    s32 col = editorOvl->cursorCol;
    s32 i;

    editorOvl->cursorIdx = 0;

    while (line != 0) {
        for (i = 0; i < editorOvl->inputLength; i++, strP++) {
            if (*strP == CHAR_NEW_LINE) {
                strP++;
                i++;
                break;
            }
        }
        editorOvl->cursorIdx += i;
        line--;
    }

    editorOvl->cursorIdx += col;
}

void mED_get_col_and_line(mED_Ovl_c* editorOvl, s16* col, s16* line, s32 idx) {
    u8* strP = (u8*)editorOvl->inputStr;
    s32 i;
    s32 beforeLineCol = 0;

    *line = 0;
    *col = 0;

    for (i = 0; i < idx; i++, strP++) {
        if (*strP == CHAR_NEW_LINE || *col + 1 == editorOvl->inputLength) {
            (*line)++;
            beforeLineCol = *col;
            *col = -1;
        }
        (*col)++;
    }

    if (*col == 0 && *line == editorOvl->maxLineNo) {
        (*line)--;
        *col = beforeLineCol + 1;
    }
}

s32 mED_get_max_col_in_line(mED_Ovl_c* editorOvl) {
    u8* strP = (u8*)editorOvl->inputStr;
    s32 col;
    s32 maxNum = editorOvl->nowStrLen;
    s32 line = editorOvl->cursorRow;

    do {
        col = 0;
        if (editorOvl->inputLength > 0 && maxNum > 0) {
            do {
                if (*strP == CHAR_NEW_LINE) {
                    strP++;
                    col++;
                    break;
                }
                col++;
                strP++;
            } while (col < editorOvl->inputLength && col < maxNum);
        }
        maxNum -= col;
    } while (--line >= 0);

    return col;
}

void mED_output_code(mED_Ovl_c* editorOvl) {
    s32 i;
    u8* afterP = (u8*)editorOvl->inputStr + editorOvl->nowStrLen;

    for (i = editorOvl->nowStrLen; editorOvl->cursorIdx < i; i--) {
        *afterP = afterP[-1];
        afterP--;
    }

    editorOvl->cursorIdx++;
    editorOvl->nowStrLen++;
    *afterP = editorOvl->nowCode;
    editorOvl->commandProcessed = TRUE;
}

void mED_input_single_line(mED_Ovl_c* editorOvl) {
    if (editorOvl->nowCode != CHAR_NEW_LINE && editorOvl->nowStrLen != editorOvl->inputLength) {
        mED_output_code(editorOvl);
    }
}

void mED_input_multi_line(mED_Ovl_c* editorOvl) {
    u8 tmpStr[98];
    u8* srcP;
    u8* dstP;
    u8* keepP;
    s32 i;
    s32 strMax;
    short inputLine;
    short inputCol;

    strMax = editorOvl->inputLength * editorOvl->maxLineNo;

    if (strMax != editorOvl->nowStrLen) {
        srcP = (u8*)editorOvl->inputStr;
        dstP = tmpStr;

        for (i = 0; i < editorOvl->cursorIdx; i++) {
            *dstP++ = *srcP++;
        }

        *dstP++ = editorOvl->nowCode;

        for (; i < strMax; i++) {
            *dstP++ = *srcP++;
        }

        keepP = (u8*)editorOvl->inputStr;
        editorOvl->inputStr = (char*)tmpStr;
        mED_get_col_and_line(editorOvl, &inputCol, &inputLine, editorOvl->nowStrLen + 1);
        editorOvl->inputStr = (char*)keepP;

        if (inputLine < editorOvl->maxLineNo) {
            mED_output_code(editorOvl);
        }
    }
}

void mED_move_cursol_right(mED_Ovl_c* editorOvl, s32 multiLineFlg) {
    if (editorOvl->cursorIdx < editorOvl->nowStrLen && editorOvl->nowStrLen > 0) {
        editorOvl->cursorIdx++;
        editorOvl->commandProcessed = TRUE;
    } else if (editorOvl->cursorIdx == editorOvl->nowStrLen) {
        editorOvl->nowCode = CHAR_SPACE;
        if (multiLineFlg) {
            mED_input_multi_line(editorOvl);
        } else {
            mED_input_single_line(editorOvl);
        }

        if (editorOvl->commandProcessed != 0) {
            editorOvl->command = mED_COMMAND_OUTPUT_CODE;
        }
    }
}

void mED_move_cursol_left(mED_Ovl_c* editorOvl) {
    if (editorOvl->cursorIdx > 0) {
        editorOvl->cursorIdx--;
        editorOvl->commandProcessed = TRUE;
    }
}

void mED_move_cursol_upper(mED_Ovl_c* editorOvl) {
    s32 upperCol;

    if (editorOvl->cursorRow > 0) {
        editorOvl->cursorRow--;
        upperCol = mED_get_max_col_in_line(editorOvl);
        if (editorOvl->cursorCol >= upperCol) {
            editorOvl->cursorCol = upperCol - 1;
        }
        mED_set_idx(editorOvl);
        editorOvl->commandProcessed = TRUE;
    }
}

void mED_move_cursol_lower(mED_Ovl_c* editorOvl) {
    short inputCol;
    short inputLine;
    s32 lowerCol;

    mED_get_col_and_line(editorOvl, &inputCol, &inputLine, editorOvl->nowStrLen);

    if (editorOvl->cursorRow < inputLine) {
        editorOvl->cursorRow++;
        lowerCol = mED_get_max_col_in_line(editorOvl);
        if (editorOvl->cursorCol >= lowerCol) {
            if (inputLine == editorOvl->cursorRow) {
                editorOvl->cursorCol = lowerCol;
            } else {
                editorOvl->cursorCol = lowerCol - 1;
            }
        }
        mED_set_idx(editorOvl);
        editorOvl->commandProcessed = TRUE;
    } else if (editorOvl->cursorIdx == editorOvl->nowStrLen) {
        editorOvl->nowCode = CHAR_NEW_LINE;
        mED_input_multi_line(editorOvl);
        if (editorOvl->commandProcessed != 0) {
            editorOvl->command = mED_COMMAND_OUTPUT_CODE;
        }
    }
}

void mED_end_edit_func(Submenu* submenu, u8* menuInfo) {
    (*(mEDMoveChgBaseProc*)(mED_OVL_BASE(submenu) + 0x106B0))(menuInfo, mSM_MOVE_OUT_BOTTOM);
    mED_EDIT_OVL(submenu)->commandProcessed = TRUE;
}

void mED_backspace_func(mED_Ovl_c* editorOvl) {
    u8* beforeStr;
    s32 i;

    if (editorOvl->cursorIdx != 0) {
        editorOvl->cursorIdx--;
        editorOvl->nowStrLen--;

        beforeStr = (u8*)editorOvl->inputStr + editorOvl->cursorIdx;
        for (i = editorOvl->cursorIdx; i < editorOvl->nowStrLen; i++) {
            *beforeStr = beforeStr[1];
            beforeStr++;
        }

        *beforeStr = CHAR_SPACE;
        editorOvl->commandProcessed = TRUE;
    }
}

s32 mED_get_exchange_code(mED_Ovl_c* editorOvl) {
    s32 inputCode;
    s32 newCode;

    if (editorOvl->cursorIdx == 0) {
        return -1;
    }

    inputCode = (u8)editorOvl->inputStr[editorOvl->cursorIdx - 1];
    newCode = mED_ornament_table[inputCode];

    if (inputCode == newCode) {
        return -1;
    }

    return newCode;
}

s32 mED_exchange_code_func(mED_Ovl_c* editorOvl) {
    if (editorOvl->exchangeCode != -1) {
        editorOvl->commandProcessed = TRUE;
        editorOvl->inputStr[editorOvl->cursorIdx - 1] = editorOvl->exchangeCode;
    }

    return editorOvl->commandProcessed;
}

// Returns TRUE (and blanks the buffer) when the text is all spaces, or unchanged from the
// letter-edit menu's stored text.
s32 mED_all_space_check(Submenu* submenu, mED_Ovl_c* editorOvl, u8* menuInfo) {
    u8* leditMenu;
    u8* inputStr;
    s32 i;
    s32 j;
    s32 sameFlg;

    if (mED_MENU_DATA0(menuInfo) != mED_TYPE_LEDIT) {
        return FALSE;
    }

    leditMenu = mED_OVL_BASE(submenu) + 0x101A8;
    inputStr = (u8*)editorOvl->inputStr;
    if (mED_MENU_DATA0(leditMenu) == 2) {
        if (mem_cmp(*(u8**)(leditMenu + 0x40), inputStr, editorOvl->inputLength) == 0) {
            sameFlg = FALSE;
        } else {
            sameFlg = TRUE;
        }
    } else {
        sameFlg = FALSE;
    }

    if (sameFlg == FALSE) {
        for (i = 0; i < editorOvl->nowStrLen; i++) {
            if (inputStr[i] != CHAR_SPACE) {
                return FALSE;
            }
        }
    }

    for (j = 0; j < editorOvl->inputLength; j++) {
        *inputStr++ = CHAR_SPACE;
    }

    editorOvl->nowStrLen = 0;
    editorOvl->cursorCol = 0;
    editorOvl->cursorIdx = 0;
    return TRUE;
}

void mED_edit_func_multi_line(Submenu* submenu, u8* menuInfo) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);

    switch (editorOvl->command) {
        case mED_COMMAND_CURSOL_LEFT:
            mED_move_cursol_left(editorOvl);
            break;
        case mED_COMMAND_CURSOL_LOWER:
            mED_move_cursol_lower(editorOvl);
            break;
        case mED_COMMAND_CURSOL_UPPER:
            mED_move_cursol_upper(editorOvl);
            break;
        case mED_COMMAND_CURSOL_RIGHT:
            mED_move_cursol_right(editorOvl, TRUE);
            break;
        case mED_COMMAND_END_EDIT:
            mED_end_edit_func(submenu, menuInfo);
            break;
        case mED_COMMAND_BACKSPACE:
            mED_backspace_func(editorOvl);
            break;
        case mED_COMMAND_EXCHANGE_CODE:
            mED_exchange_code_func(editorOvl);
            break;
        case mED_COMMAND_OUTPUT_CODE:
            mED_input_multi_line(editorOvl);
            break;
    }
}

void mED_edit_func_single_line(Submenu* submenu, u8* menuInfo) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);

    switch (editorOvl->command) {
        case mED_COMMAND_CURSOL_LEFT:
            mED_move_cursol_left(editorOvl);
            break;
        case mED_COMMAND_CURSOL_RIGHT:
            mED_move_cursol_right(editorOvl, FALSE);
            break;
        case mED_COMMAND_END_EDIT:
            if (mED_all_space_check(submenu, editorOvl, menuInfo)) {
                sAdo_SysTrgStart(0x1003);
            } else {
                mED_end_edit_func(submenu, menuInfo);
            }
            break;
        case mED_COMMAND_BACKSPACE:
            mED_backspace_func(editorOvl);
            break;
        case mED_COMMAND_EXCHANGE_CODE:
            mED_exchange_code_func(editorOvl);
            break;
        case mED_COMMAND_OUTPUT_CODE:
            mED_input_single_line(editorOvl);
            break;
    }
}

void mED_edit_func_letter_header(Submenu* submenu, u8* menuInfo) {
    mED_Ovl_c* editorOvl;
    u8* boardOvl;
    s32 keepInputNum;

    boardOvl = mED_BOARD_OVL(submenu);
    editorOvl = mED_EDIT_OVL(submenu);

    if (mED_BOARD_HEADER_POS(boardOvl) != 1) {
        keepInputNum = editorOvl->nowStrLen;

        if (editorOvl->cursorIdx == mED_BOARD_HEADER_BACK_START(boardOvl)) {
            if ((mED_BOARD_HEADER_POS(boardOvl) == 0 && editorOvl->command == mED_COMMAND_CURSOL_RIGHT) ||
                (mED_BOARD_HEADER_POS(boardOvl) == 2 &&
                 (editorOvl->command == mED_COMMAND_CURSOL_LEFT || editorOvl->command == mED_COMMAND_BACKSPACE))) {
                mED_BOARD_HEADER_POS(boardOvl) = 1;
                editorOvl->commandProcessed = TRUE;
                return;
            }
        }

        mED_edit_func_single_line(submenu, menuInfo);

        if (mED_BOARD_HEADER_POS(boardOvl) == 0) {
            if (editorOvl->nowStrLen < keepInputNum) {
                mED_BOARD_HEADER_BACK_START(boardOvl)--;
            } else if (keepInputNum < editorOvl->nowStrLen) {
                mED_BOARD_HEADER_BACK_START(boardOvl)++;
            }
        }
    }
}

void mED_move_letter_table(mED_Ovl_c* editorOvl, u8* boardOvl) {
    s32 newTable;
    s32 upFlg;

    if (boardOvl[0] == 0 && mED_BOARD_HEADER_POS(boardOvl) == 1) {
        return;
    }

    if (editorOvl->command == mED_COMMAND_CURSOL_UPPER) {
        newTable = boardOvl[0] - 1;
        upFlg = TRUE;
    } else if (editorOvl->command == mED_COMMAND_CURSOL_LOWER) {
        newTable = boardOvl[0] + 1;
        upFlg = FALSE;
    } else {
        return;
    }

    if (newTable >= 0 && newTable < 3) {
        editorOvl->commandProcessed = TRUE;
        boardOvl[0] = newTable;
        mED_BOARD_HEADER_POS(boardOvl) = 0;
        editorOvl->inputLength = mED_letter_table_col[newTable];

        if (newTable == 1) {
            editorOvl->maxLineNo = 6;
            editorOvl->inputStr = (char*)(boardOvl + 0x3C);
            if (upFlg == TRUE) {
                editorOvl->cursorIdx = -1;
            } else {
                editorOvl->cursorIdx = 0;
            }
        } else {
            editorOvl->maxLineNo = 1;
            editorOvl->cursorIdx = 0;
            if (newTable == 0) {
                editorOvl->inputStr = (char*)(boardOvl + 0x32);
            } else {
                editorOvl->inputStr = (char*)(boardOvl + 0x9C);
            }
        }

        editorOvl->nowStrLen = (boardOvl + newTable)[5];
        if (editorOvl->cursorIdx == -1) {
            editorOvl->cursorIdx = editorOvl->nowStrLen;
        }
    }
}

void mED_edit_func_letter(Submenu* submenu, u8* menuInfo) {
    s32 tableKind;
    mBD_Ovl_c* boardOvl;
    mED_Ovl_c* editorOvl;

    tableKind = ((mED_OvlLayout_c*)submenu->overlay)->boardOvl->field;
    boardOvl = ((mED_OvlLayout_c*)submenu->overlay)->boardOvl;
    editorOvl = ((mED_OvlLayout_c*)submenu->overlay)->editorOvl;

    (*mED_letter_base_func[tableKind])(submenu, menuInfo);
    boardOvl->lengths[tableKind] = editorOvl->nowStrLen;

    if (editorOvl->commandProcessed == 0) {
        mED_move_letter_table(editorOvl, (u8*)boardOvl);
    }
}

void mED_move_Move(Submenu* submenu, u8* menuInfo) {
    (*(mED_OVL_PROC*)(mED_OVL_BASE(submenu) + 0x106A8))(submenu, menuInfo);
}

void mED_move_Play(Submenu* submenu, u8* menuInfo) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);

    editorOvl->commandProcessed = FALSE;

    if (*(u32*)(mED_OVL_BASE(submenu) + 0x106E8) != 0 && *(s32*)(mED_OVL_BASE(submenu) + 0x10460) == 2) {
        editorOvl->stickArea = mED_STICK_AREA_CENTER;
        editorOvl->stickAreaChanged = 0;
        editorOvl->command = mED_COMMAND_NONE;
        editorOvl->lastButtons = B_BUTTON;
        editorOvl->buttonRepeatTimer = 15;
        editorOvl->buttonRepeatAccelTimer = 13;
        editorOvl->buttonRepeatStarted = 0;
    } else {
        editorOvl->cursolOpacityStep++;
        if (editorOvl->cursolOpacityStep == 20) {
            editorOvl->cursolOpacityStep = 0;
        }

        mED_set_stick_area(submenu);

        if (editorOvl->rotateTimer > 0) {
            if (editorOvl->stickAreaChanged != 0 && editorOvl->stickArea != mED_STICK_AREA_CENTER) {
                // page flip interrupted by stick movement: snap to the next page boundary
                editorOvl->rotateTimer = 0;
                while (editorOvl->pageTopNum % 5 != 0) {
                    editorOvl->pageTopNum++;
                }
                if (editorOvl->pageTopNum == mED_page_num_max[editorOvl->shiftMode]) {
                    editorOvl->pageTopNum = 0;
                }
            } else {
                editorOvl->rotateTimer--;
                if (editorOvl->rotateTimer == 1) {
                    editorOvl->pageTopNum += 2;
                    if (editorOvl->pageTopNum == mED_page_num_max[editorOvl->shiftMode]) {
                        editorOvl->pageTopNum = 0;
                    }
                }
                return;
            }
        }

        mED_set_command(submenu);
        mED_edit_func[mED_MENU_DATA0(menuInfo)](submenu, menuInfo);

        if (editorOvl->consonantNum == -1) {
            if (editorOvl->animFrame > 0) {
                editorOvl->animFrame--;
            }
        } else {
            if (editorOvl->animFrame < 2) {
                editorOvl->animFrame++;
            }
        }

        if (editorOvl->commandProcessed != 0) {
            mED_set_se(submenu);
            editorOvl->exchangeCode = mED_get_exchange_code(editorOvl);

            if (editorOvl->command == mED_COMMAND_CURSOL_RIGHT || editorOvl->command == mED_COMMAND_CURSOL_LEFT ||
                editorOvl->command == mED_COMMAND_CURSOL_UPPER || editorOvl->command == mED_COMMAND_CURSOL_LOWER ||
                editorOvl->command == mED_COMMAND_BACKSPACE || editorOvl->command == mED_COMMAND_OUTPUT_CODE) {
                editorOvl->cursolOpacityStep = 0;
            }
        }
    }

    mED_get_col_and_line(editorOvl, &editorOvl->cursorCol, &editorOvl->cursorRow, editorOvl->cursorIdx);
}

void mED_move_End(Submenu* submenu, u8* menuInfo) {
    mED_EDIT_OVL(submenu)->cursorCol = 0;
    (*(mED_OVL_PROC*)(mED_OVL_BASE(submenu) + 0x106AC))(submenu, menuInfo);
}

void mED_editor_ovl_move(Submenu* submenu) {
    u8* menuInfo = mED_MENU(submenu);

    mED_MENU_PRE_MOVE_FUNC(menuInfo)(submenu, menuInfo);
    mED_ovl_move_proc[mED_MENU_PROC_STATUS(menuInfo)](submenu, menuInfo);
}

void mED_editor_draw_init(u8* menuInfo) {
    char** texP;
    unsigned short** texShortP;
    s32 i;
    void* tex;

    SegmentBaseAddress[0xC] = mED_MENU_SEGMENT(menuInfo) + 0x80000000;

    texP = lat_sousa_st_tex_tbl;
    for (i = 0; i < 9; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_waku1_tex_tbl;
    for (i = 0; i < 3; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_button_pallet_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texShortP = lat_sousa_start_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texShortP);
        texShortP++;
        texShortP[-1] = tex;
    }

    texShortP = lat_sousa_a_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texShortP);
        texShortP++;
        texShortP[-1] = tex;
    }

    texShortP = lat_sousa_b_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texShortP);
        texShortP++;
        texShortP[-1] = tex;
    }

    texP = lat_sousa_c_r_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_c_l_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_c_u_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_c_d_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_z_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }

    texP = lat_sousa_r_button_tbl;
    for (i = 0; i < 2; i++) {
        tex = Lib_SegmentedToVirtual(*texP);
        texP++;
        texP[-1] = tex;
    }
}

// TRUE while the outer 5-key "ring out" should be drawn (kana modes only).
s32 mED_StringsDrawCheck_forRingOut(mED_Ovl_c* editorOvl) {
    if (!((editorOvl->shiftMode == 0 || editorOvl->shiftMode == 2) &&
          (editorOvl->consonantNum != -1 || editorOvl->stickArea < 5))) {
        return FALSE;
    }

    return TRUE;
}

#define mED_SET_PRIM(gp, c) gDPSetPrimColor(gp, 0, 255, (c).r, (c).g, (c).b, 255)
#define mED_SET_ENV(gp, c) gDPSetEnvColor(gp, (c).r, (c).g, (c).b, 255)

void mED_KeyDraw(mED_Ovl_c* editorOvl, GraphicsContext* graph, float posX, float posY) {
    s32 shiftMode = editorOvl->shiftMode;
    s32 consonantNum = editorOvl->consonantNum;
    s32 stickArea = editorOvl->stickArea;
    s32 anmFrame = editorOvl->animFrame;
    s32 i;
    s32 buttonFlg;
    mED_Model_Color_Data* colorP;

    colorP = &mED_model_color[anmFrame];

    Matrix_scale(16.0f, 16.0f, 1.0f, 0);
    Matrix_translate(posX, posY, 0.0f, 1);

    OPEN_POLY_OPA_DISP(graph);

    gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(__polyOpa++, D_C000C10);
    gSPDisplayList(__polyOpa++, D_C0017A0);
    gSPDisplayList(__polyOpa++, D_C000E08);
    gSPDisplayList(__polyOpa++, D_C0017D0);

    // START
    buttonFlg = (getButton() & START_BUTTON) ? TRUE : FALSE;
    gSPSegment(__polyOpa++, 8, lat_sousa_button_pallet_tbl[buttonFlg]);
    gSPSegment(__polyOpa++, 9, lat_sousa_start_button_tbl[buttonFlg]);
    gSPDisplayList(__polyOpa++, D_C000CD8);

    // B
    buttonFlg = (getButton() & B_BUTTON) ? TRUE : FALSE;
    gSPSegment(__polyOpa++, 8, lat_sousa_button_pallet_tbl[buttonFlg]);
    gSPSegment(__polyOpa++, 9, lat_sousa_b_button_tbl[buttonFlg]);
    gSPDisplayList(__polyOpa++, D_C000D70);

    // A
    buttonFlg = (getButton() & A_BUTTON) ? TRUE : FALSE;
    gSPSegment(__polyOpa++, 8, lat_sousa_button_pallet_tbl[buttonFlg]);
    gSPSegment(__polyOpa++, 9, lat_sousa_a_button_tbl[buttonFlg]);
    gSPDisplayList(__polyOpa++, D_C000C40);

    gSPDisplayList(__polyOpa++, D_C0017B8);
    gDPSetCycleType(__polyOpa++, G_CYC_1CYCLE);
    gDPSetRenderMode(__polyOpa++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

    mED_SET_PRIM(__polyOpa++, colorP->basePrim);
    mED_SET_ENV(__polyOpa++, colorP->baseEnv);
    gSPDisplayList(__polyOpa++, D_C001110);

    if (consonantNum != -1) {
        gDPPipeSync(__polyOpa++);
        mED_SET_PRIM(__polyOpa++, colorP->selPrim);
        mED_SET_ENV(__polyOpa++, colorP->selEnv);
        gSPDisplayList(__polyOpa++, D_C0010B8);
    }

    if (shiftMode == 0 || shiftMode == 2) {
        gDPPipeSync(__polyOpa++);
        mED_SET_PRIM(__polyOpa++, colorP->wakuPrim);
        mED_SET_ENV(__polyOpa++, colorP->wakuEnv);
        gSPSegment(__polyOpa++, 9, lat_sousa_waku1_tex_tbl[anmFrame]);
        gSPDisplayList(__polyOpa++, D_C001060);
    }

    if (consonantNum == -1) {
        gDPPipeSync(__polyOpa++);
        mED_SET_PRIM(__polyOpa++, colorP->selPrim);
        mED_SET_ENV(__polyOpa++, colorP->selEnv);
        gSPDisplayList(__polyOpa++, D_C0010B8);
    }

    // C buttons
    gSPSegment(__polyOpa++, 8, lat_sousa_c_r_button_tbl[(getButton() & CONT_F) ? TRUE : FALSE]);
    gSPDisplayList(__polyOpa++, D_C000EC0);

    gSPSegment(__polyOpa++, 8, lat_sousa_c_l_button_tbl[(getButton() & CONT_C) ? TRUE : FALSE]);
    gSPDisplayList(__polyOpa++, D_C000F28);

    gSPSegment(__polyOpa++, 8, lat_sousa_c_u_button_tbl[(getButton() & CONT_E) ? TRUE : FALSE]);
    gSPDisplayList(__polyOpa++, D_C000F90);

    gSPSegment(__polyOpa++, 8, lat_sousa_c_d_button_tbl[(getButton() & CONT_D) ? TRUE : FALSE]);
    gSPDisplayList(__polyOpa++, D_C000FF8);

    // Z / R
    gSPSegment(__polyOpa++, 8, lat_sousa_z_button_tbl[(getButton() & Z_TRIG) ? TRUE : FALSE]);
    gSPSegment(__polyOpa++, 9, lat_sousa_r_button_tbl[(getButton() & R_TRIG) ? TRUE : FALSE]);
    gSPDisplayList(__polyOpa++, D_C0017F8);

    if (mEditor_Get_Now_Code(editorOvl) == -1) {
        gSPDisplayList(__polyOpa++, D_C001660);
    }

    if (editorOvl->exchangeCode == -1) {
        gSPDisplayList(__polyOpa++, D_C001730);
    } else {
        gSPDisplayList(__polyOpa++, D_C0017A0);
        gSPDisplayList(__polyOpa++, D_C016FD0);
    }
    gSPDisplayList(__polyOpa++, D_C0017A0);

    mED_SET_PRIM(__polyOpa++, colorP->labelPrim);
    gSPDisplayList(__polyOpa++, D_C0016D0);

    // highlighted character-set model, then the other four
    gDPSetPrimColor(__polyOpa++, 0, 255, 225, 45, 0, 255);
    gSPDisplayList(__polyOpa++, mED_display_list_table[shiftMode]);

    gDPSetPrimColor(__polyOpa++, 0, 255, 205, 135, 80, 255);
    for (i = 0; i < 5; i++) {
        if (i != shiftMode) {
            gSPDisplayList(__polyOpa++, mED_display_list_table[i]);
        }
    }

    if (stickArea >= 3 && stickArea < 6) {
        Matrix_RotateY(-0x8000, 1);
        gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }

    gSPSegment(__polyOpa++, 8, lat_sousa_st_tex_tbl[stickArea]);
    gSPDisplayList(__polyOpa++, D_C0017B8);
    gSPDisplayList(__polyOpa++, D_C0015F8);

    CLOSE_POLY_OPA_DISP(graph);
}

void mED_StringsDraw_RingSingle(Game* game, u8* strRing, float posX, float posY, float scale, s32 colorType,
                                s32 ringFlg) {
    Color_RGBA8* colorP;
    GraphicsContext* graph;

    colorP = &mED_rstr_color[colorType];
    if (*strRing != CHAR_SPACE) {
        mFont_SetLineStrings(game, (char*)strRing, 1, posX, posY, colorP->r, colorP->g, colorP->b, 255, FALSE, FALSE,
                             scale, scale, FALSE);
    } else {
        // blank key: draw a placeholder square instead of a glyph
        graph = game->gfxCtx;

        OPEN_POLY_OPA_DISP(graph);

        Matrix_scale(16.0f, 16.0f, 1.0f, 0);
        Matrix_translate(posX - 160.0f, -(posY - 120.0f), 0.0f, 1);
        if (ringFlg != 0) {
            Matrix_scale(scale, scale, 1.0f, 1);
        } else {
            Matrix_scale(scale * 0.75f, scale, 1.0f, 1);
        }
        Matrix_translate(7.0f, -9.0f, 0.0f, 1);

        gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(__polyOpa++, 0, 255, colorP->r, colorP->g, colorP->b, 255);
        gSPDisplayList(__polyOpa++, D_C016E90);
        gSPMatrix(__polyOpa++, &Mtx_clear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        CLOSE_POLY_OPA_DISP(graph);
    }
}

void mED_StringsDraw_forRingInReturn(Game* game, float posX, float posY) {
    s32 i;
    float* posP = mED_ring_in_return_pos[0];
    u8* strP = mED_return_str;

    for (i = 0; i < 3; i++) {
        mFont_SetLineStrings(game, (char*)strP, 1, posP[0] + posX, posP[1] - posY, 225, 135, 225, 255, FALSE, FALSE,
                             0.875f, 0.875f, FALSE);
        posP += 2;
        strP++;
    }
}

void mED_StringsDraw_forRingIn(mED_Ovl_c* editorOvl, Game* game, float posX, float posY) {
    u8 strRing[10];
    s32 i;
    s32 colorType;
    UNUSED s32 areaNum;
    float* posP;
    float scale;

    if (editorOvl->rotateTimer == 0 || (editorOvl->rotateTimer >= 2 && editorOvl->rotateTimer < 3)) {
        posP = mED_ring_in_pos[0][0];
    } else {
        posP = mED_ring_in_pos[1][0];
    }

    for (i = 0; i != 10; i++) {
        strRing[i] = mED_get_code(editorOvl, editorOvl->pageTopNum, i, TRUE);
    }

    for (i = 0; i < 10; i++) {
        if (mED_ring_in_pos[0][0][1] < posP[1]) {
            colorType = 1;
            scale = 0.75f;
        } else {
            scale = 1.125f;
            if (i == editorOvl->stickArea) {
                colorType = 0;
            } else {
                colorType = 2;
            }
        }

        mED_StringsDraw_RingSingle(game, &strRing[i], posP[0] + posX, posP[1] - posY, scale, colorType, TRUE);
        posP += 2;
    }
}

void mED_StringsDraw_forRingOut(mED_Ovl_c* editorOvl, Game* game, float posX, float posY) {
    u8 strRing[5];
    s32 stickArea = editorOvl->stickArea;
    s32 consonantNum = editorOvl->consonantNum;
    s32 anmFrame = editorOvl->animFrame;
    s32 i;
    s32 j;
    UNUSED s32 cNum;
    s32 areaNum;
    s32 colorType;
    float* posP = mED_ring_out_pos[anmFrame][0];
    float scale = mED_scale_kind[anmFrame];

    if (consonantNum == -1) {
        areaNum = editorOvl->pageTopNum + stickArea;
    } else {
        areaNum = consonantNum;
    }

    for (i = 0; i != 5; i++) {
        strRing[i] = mED_get_code(editorOvl, areaNum, i, FALSE);
    }

    for (j = 0; j < 5; j++) {
        if (j == stickArea && consonantNum != -1) {
            colorType = 0;
        } else {
            colorType = anmFrame + 3;
        }

        mED_StringsDraw_RingSingle(game, &strRing[j], posP[0] + posX, posP[1] - posY, scale, colorType, TRUE);
        posP += 2;
    }
}

void mED_StringsDraw_select(Submenu* submenu, Game* game, float posX, float posY) {
    s32 intCode;
    u8 code;

    intCode = mEditor_Get_Now_Code(mED_EDIT_OVL(submenu));
    if (intCode != -1) {
        code = intCode;
        mED_StringsDraw_RingSingle(game, &code, 239.0f + posX, 182.0f - posY, 1.0f, 0, 0);
    }
}

void mED_StringsDraw_ornament(mED_Ovl_c* editorOvl, Game* game, f32 posX, f32 posY) {
    if (editorOvl->exchangeCode != -1) {
        u8 code;

        code = editorOvl->exchangeCode;

        posY = 120.0f - (posY + 8.0f - 6.0f);
        posX = posX + 51.0f + 160.0f;

        mFont_SetLineStrings(game, editorOvl->inputStr + editorOvl->cursorIdx - 1, 1, posX + 8.0f, posY, 200, 200, 200,
                             255, FALSE, FALSE, 1.0f, 1.0f, FALSE);
        mFont_SetLineStrings(game, (char*)&code, 1, posX + 18.0f + 16.0f, posY, 255, 255, 255, 255, FALSE, FALSE, 1.0f,
                             1.0f, FALSE);
    }
}

void mED_StringsDraw(Submenu* submenu, mED_Ovl_c* editorOvl, Game* game, float posX, float posY) {
    (*(void (**)(GraphicsContext*))(mED_OVL_BASE(submenu) + 0x106B4))(game->gfxCtx);

    if (editorOvl->consonantNum == -1) {
        mED_StringsDraw_forRingIn(editorOvl, game, posX, posY);
    } else {
        mED_StringsDraw_forRingInReturn(game, posX, posY);
    }

    if (mED_StringsDrawCheck_forRingOut(editorOvl)) {
        mED_StringsDraw_forRingOut(editorOvl, game, posX, posY);
    }

    mED_StringsDraw_select(submenu, game, posX, posY);
    mED_StringsDraw_ornament(editorOvl, game, posX, posY);
}

void mED_set_dl(Submenu* submenu, u8* menuInfo, Game* game) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);
    GraphicsContext* graph = game->gfxCtx;

    SegmentBaseAddress[0xC] = mED_MENU_SEGMENT(menuInfo) + 0x80000000;

    OPEN_POLY_OPA_DISP(graph);
    gSPSegment(__polyOpa++, 0x0C, mED_MENU_SEGMENT(menuInfo));
    CLOSE_POLY_OPA_DISP(graph);

    mED_KeyDraw(editorOvl, graph, mED_MENU_POS_X(menuInfo), mED_MENU_POS_Y(menuInfo));
    mED_StringsDraw(submenu, editorOvl, game, mED_MENU_POS_X(menuInfo), mED_MENU_POS_Y(menuInfo));
}

void mED_endCode_draw(Submenu* submenu, Game* game, float posX, float posY) {
    GraphicsContext* graph = game->gfxCtx;
    u8* editMenu = mED_MENU(submenu);

    Matrix_scale(16.0f, 16.0f, 1.0f, 0);
    Matrix_translate(posX, posY, 140.0f, 1);

    OPEN_POLY_OPA_DISP(graph);

    gSPSegment(__polyOpa++, 0x0C, mED_MENU_SEGMENT(editMenu));
    gSPMatrix(__polyOpa++, _Matrix_to_Mtx_new(graph), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(__polyOpa++, D_C000040);

    CLOSE_POLY_OPA_DISP(graph);
}

void mED_cursol_draw(Submenu* submenu, Game* game, f32 posX, f32 posY) {
    mED_Ovl_c* editorOvl = mED_EDIT_OVL(submenu);
    s32 cursolAlpha;

    if (editorOvl->cursolOpacityStep >= 11) {
        cursolAlpha = 20 - editorOvl->cursolOpacityStep;
    } else {
        cursolAlpha = editorOvl->cursolOpacityStep;
    }

    cursolAlpha = (10 - cursolAlpha) * 255 / 10;

    mFont_SetMarkChar(game, 2, posX, posY, 0xC3, 0x50, 0x50, cursolAlpha, FALSE, 1.0f, 1.0f, 0);
}

void mED_editor_ovl_draw(Submenu* submenu, Game* game) {
    u8* menuInfo = mED_MENU(submenu);

    mED_MENU_PRE_DRAW_FUNC(menuInfo)(submenu, game);
    mED_set_dl(submenu, menuInfo, game);

    if (mED_MENU_PRE_MENU_TYPE(menuInfo) == SUBMENU_PROGRAM_ADDRESS) {
        if (*(u32*)(*(u8**)(mED_OVL_BASE(submenu) + 0x106E8) + 0x194) != 0) {
            OPEN_DISPS(game->gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, *(u32*)(*(u8**)(mED_OVL_BASE(submenu) + 0x106E8) + 0x194));
            CLOSE_DISPS(game->gfxCtx);
        }
    }
}

void mED_editor_ovl_set_proc(Submenu* submenu) {
    void** ctrlP = (void**)(mED_OVL_BASE(submenu) + 0x10670);

    ctrlP[0] = mED_editor_ovl_move;
    ctrlP[1] = mED_editor_ovl_draw;
}

void mED_editor_ovl_init(Submenu* submenu) {
    u8* menuInfo = (u8*)submenu->overlay + 0x10358;

    (*(mEDMoveChgBaseProc*)((u8*)submenu->overlay + 0x106B0))(menuInfo, mSM_MOVE_IN_BOTTOM);

    if (*(s32*)(menuInfo + 0x38) == mED_TYPE_CP_TITLE || *(s32*)(menuInfo + 0x38) == mED_TYPE_LEDIT ||
        *(s32*)(menuInfo + 0x38) == mED_TYPE_HBOARD) {
        sAdo_SysTrgStart(0x59);
    }
}

void mED_editor_ovl_construct(Submenu* submenu) {
    mED_OvlLayout_c* overlay = (mED_OvlLayout_c*)submenu->overlay;
    u8* menuInfo = (u8*)&overlay->menuInfo[SUBMENU_PROGRAM_EDITOR];

    if (overlay->editorOvl == NULL) {
        (*((mED_OvlLayout_c*)submenu->overlay)->dataLoadProc)(submenu, (void**)(menuInfo + 0x28), &mED_data_dlftbl);
        overlay->editorOvl = &mED_edit_ovl_data;
        mED_editor_draw_init(menuInfo);
    }

    mED_init(submenu, menuInfo);
    mED_editor_ovl_init(submenu);
    mED_editor_ovl_set_proc(submenu);

    ((mED_OvlLayout_c*)submenu->overlay)->editorOvl->endCodeDraw = mED_endCode_draw;
    ((mED_OvlLayout_c*)submenu->overlay)->editorOvl->cursolDraw = mED_cursol_draw;
}

void mED_editor_ovl_destruct(Submenu* submenu) {
    *(void**)(mED_OVL_BASE(submenu) + 0x106E0) = NULL;
}
