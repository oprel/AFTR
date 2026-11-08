#ifndef M_EDITOR_OVL_H
#define M_EDITOR_OVL_H

#include "ultra64.h"
#include "unk.h"
#include "m_submenu.h"
#include "game.h"
#include "overlays/submenu/submenu_ovl/m_submenu_ovl.h"

#define mED_COLUMNS 10
#define mED_ROWS 4

#define mED_NG_WORD_START 0x74A
#define mED_NG_WORD_NUM 14

enum {
    mED_TYPE_BOARD,
    mED_TYPE_HBOARD,
    mED_TYPE_NOTICE,
    mED_TYPE_LEDIT,
    mED_TYPE_CP_TITLE,
    mED_TYPE_DIARY,
    mED_TYPE_PASSWORDMAKE,
    mED_TYPE_PASSWORDCHK,
    mED_TYPE_LEDIT_LATIN,

    mED_TYPE_NUM
};

enum {
    mED_STICK_AREA_LEFT,
    mED_STICK_AREA_TOP_LEFT,
    mED_STICK_AREA_TOP,
    mED_STICK_AREA_TOP_RIGHT,
    mED_STICK_AREA_RIGHT,
    mED_STICK_AREA_BOTTOM_RIGHT,
    mED_STICK_AREA_BOTTOM,
    mED_STICK_AREA_BOTTOM_LEFT,
    mED_STICK_AREA_CENTER,

    mED_STICK_AREA_NUM
};

enum {
    mED_COMMAND_NONE,
    mED_COMMAND_CURSOL_RIGHT,
    mED_COMMAND_CURSOL_LEFT,
    mED_COMMAND_CURSOL_UPPER,
    mED_COMMAND_CURSOL_LOWER,
    mED_COMMAND_END_EDIT,
    mED_COMMAND_BACKSPACE,
    mED_COMMAND_EXCHANGE_CODE,
    mED_COMMAND_OUTPUT_CODE,
    mED_COMMAND_9,

    mED_COMMAND_NUM
};

enum {
    mED_SHIFT_LOWER,
    mED_SHIFT_UPPER,

    mED_SHIFT_NUM
};

enum {
    mED_ARRANGE_QWERTY,
    mED_ARRANGE_ALPHA,

    mED_ARRANGE_NUM
};

enum {
    mED_INPUT_MODE_LETTER,
    mED_INPUT_MODE_SIGN,
    mED_INPUT_MODE_MARK,

    mED_INPUT_MODE_NUM
};

enum {
    mED_LINETYPE_SINGLE,
    mED_LINETYPE_MULTI,
    mED_LINETYPE_PW,

    mED_LINETYPE_NUM
};

enum {
    mED_LINE_OK,
    mED_LINE_NEWLINE,
    mED_LINE_WIDTH_OVER,
    mED_LINE_NUM_OVER,

    mED_LINE_NUM
};

enum {
    mED_SHIFT_MODE_HIRAGANA,
    mED_SHIFT_MODE_KIGOU,
    mED_SHIFT_MODE_KATAKANA,
    mED_SHIFT_MODE_ALPHABET,
    mED_SHIFT_MODE_NUMBER,
    mED_SHIFT_MODE_EMOJI,

    mED_SHIFT_MODE_NUM
};

typedef void (*mED_ENDCODE_DRAW_PROC)(Submenu*, Game*, f32, f32);
typedef void (*mED_CURSOL_DRAW_PROC)(Submenu*, Game*, f32, f32);

struct editor_ovl_s {
    /* 0x00 */ u8 stickArea;
    /* 0x01 */ u8 lastStickArea;
    /* 0x02 */ u8 stickAreaChanged;
    /* 0x03 */ u8 rotateTimer;
    /* 0x04 */ u8 shiftMode; // character set
    /* 0x05 */ u8 pageTopNum;
    /* 0x06 */ s16 consonantNum;
    /* 0x08 */ u32 lastButtons;
    /* 0x0C */ s16 cursolOpacityStep;
    /* 0x0E */ u8 buttonRepeatStarted;
    /* 0x0F */ u8 buttonRepeatAccelTimer;
    /* 0x10 */ u8 buttonRepeatTimer;
    /* 0x11 */ u8 command;
    /* 0x12 */ u8 inputOrnamentCode;
    /* 0x13 */ u8 nowCode;
    /* 0x14 */ u8 animFrame;
    /* 0x15 */ u8 commandProcessed;
    /* 0x16 */ s16 cursorIdx;
    /* 0x18 */ s16 inputLength;
    /* 0x1A */ s16 maxLineNo;
    /* 0x1C */ s16 nowStrLen;
    /* 0x1E */ s16 exchangeCode;
    /* 0x20 */ s16 cursorCol;
    /* 0x22 */ s16 cursorRow;
    /* 0x24 */ char* inputStr;
    /* 0x28 */ mED_ENDCODE_DRAW_PROC endCodeDraw;
    /* 0x2C */ mED_CURSOL_DRAW_PROC cursolDraw;
}; // size = 0x30

typedef struct editor_ovl_s mED_Ovl_c;

s32 mED_get_code(mED_Ovl_c* editorOvl, s32 pageTopNum, s32 num, s32 firstFlg);
void mED_set_stick_area(Submenu* submenu);
s32 mED_check_move_cursol(mED_Ovl_c* editorOvl);
s32 mEditor_Check_Shift_Mode(mED_Ovl_c* editorOvl);
s32 mEditor_Check_Page_Mode(mED_Ovl_c* editorOvl);
s32 mEditor_AreaToConsonant(mED_Ovl_c* editorOvl);
s32 mEditor_Input_Consonant_Num(mED_Ovl_c* editorOvl);
s32 mEditor_Get_Now_Code(mED_Ovl_c* editorOvl);
s32 mED_check_now_code(mED_Ovl_c* editorOvl);
s32 mED_check_output_code(mED_Ovl_c* editorOvl);
void mED_set_command(Submenu* submenu);
void mED_init(Submenu* submenu, u8* menuInfo);
void mED_set_se(Submenu* submenu);
void mED_set_idx(mED_Ovl_c* editorOvl);
void mED_get_col_and_line(mED_Ovl_c* editorOvl, s16* col, s16* line, s32 idx);
s32 mED_get_max_col_in_line(mED_Ovl_c* editorOvl);
void mED_output_code(mED_Ovl_c* editorOvl);
void mED_input_single_line(mED_Ovl_c* editorOvl);
void mED_input_multi_line(mED_Ovl_c* editorOvl);
void mED_move_cursol_right(mED_Ovl_c* editorOvl, s32 multiLineFlg);
void mED_move_cursol_left(mED_Ovl_c* editorOvl);
void mED_move_cursol_upper(mED_Ovl_c* editorOvl);
void mED_move_cursol_lower(mED_Ovl_c* editorOvl);
void mED_end_edit_func(Submenu* submenu, u8* menuInfo);
void mED_backspace_func(mED_Ovl_c* editorOvl);
s32 mED_get_exchange_code(mED_Ovl_c* editorOvl);
s32 mED_exchange_code_func(mED_Ovl_c* editorOvl);
s32 mED_all_space_check(Submenu* submenu, mED_Ovl_c* editorOvl, u8* menuInfo);
void mED_edit_func_multi_line(Submenu* submenu, u8* menuInfo);
void mED_edit_func_single_line(Submenu* submenu, u8* menuInfo);
void mED_edit_func_letter_header(Submenu* submenu, u8* menuInfo);
void mED_move_letter_table(mED_Ovl_c* editorOvl, u8* boardOvl);
void mED_edit_func_letter(Submenu* submenu, u8* menuInfo);
void mED_move_Move(Submenu* submenu, u8* menuInfo);
void mED_move_Play(Submenu* submenu, u8* menuInfo);
void mED_move_End(Submenu* submenu, u8* menuInfo);
void mED_editor_ovl_move(Submenu* submenu);
void mED_editor_draw_init(u8* menuInfo);
s32 mED_StringsDrawCheck_forRingOut(mED_Ovl_c* editorOvl);
void mED_KeyDraw(mED_Ovl_c* editorOvl, struct GraphicsContext* graph, float posX, float posY);
void mED_StringsDraw_RingSingle(Game* game, u8* strRing, float posX, float posY, float scale, s32 colorType,
                                s32 ringFlg);
void mED_StringsDraw_forRingInReturn(Game* game, float posX, float posY);
void mED_StringsDraw_forRingIn(mED_Ovl_c* editorOvl, Game* game, float posX, float posY);
void mED_StringsDraw_forRingOut(mED_Ovl_c* editorOvl, Game* game, float posX, float posY);
void mED_StringsDraw_select(Submenu* submenu, Game* game, float posX, float posY);
void mED_StringsDraw_ornament(mED_Ovl_c* editorOvl, Game* game, f32 posX, f32 posY);
void mED_StringsDraw(Submenu* submenu, mED_Ovl_c* editorOvl, Game* game, float posX, float posY);
void mED_set_dl(Submenu* submenu, u8* menuInfo, Game* game);
void mED_endCode_draw(Submenu* submenu, Game* game, float posX, float posY);
void mED_cursol_draw(Submenu* submenu, Game* game, f32 posX, f32 posY);
void mED_editor_ovl_draw(Submenu* submenu, Game* game);
void mED_editor_ovl_init(Submenu* submenu);

extern void mED_editor_ovl_set_proc(Submenu* submenu);
extern void mED_editor_ovl_construct(Submenu* submenu);
extern void mED_editor_ovl_destruct(Submenu* submenu);

#endif
