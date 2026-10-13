#ifndef M_FONT_H
#define M_FONT_H

#include "ultra64.h"
#include "unk.h"

struct GraphicsContext;
struct Game;

s32 mFont_Get_end_load_texture(void);
void mFont_Set_end_load_texture(s32 arg0);
// void mFont_Get_FontOffset();
// void mFont_Get_FontTex_p();
// void mFont_Load_FontTex();
// void mFont_GetCodeWidth();
s32 mFont_GetStringWidth(char*, s32, s32);
// void func_8009034C_jp();
// void func_800903A8_jp();
// void func_800903CC_jp();
// void func_800903E4_jp();
// void func_8009040C_jp();
// void func_80090470_jp();
// void func_800904B8_jp();
// void func_80090504_jp();
// void func_80090564_jp();
// void func_800905D4_jp();
// void func_80090648_jp();
// void func_8009069C_jp();
// void func_80090848_jp();
// void func_8009087C_jp();
// void func_80090CC0_jp();
f32 mFont_SetLineStrings_AndSpace(struct Game*, char*, s32, f32 x, f32 y, s32, s32, s32, s32, s32, s32, s32, f32, f32,s32);
// void func_80090E98_jp();
void mFont_SetMatrix(struct GraphicsContext* gfxCtx, s32 type);
void mFont_UnSetMatrix(struct GraphicsContext* gfxCtx, s32 type);
void mFont_Main_start(struct GraphicsContext* gfxCtx);
// void mFont_Get_NextCharRectX();
// void mFont_Get_NextCharRectY();
// void mFont_gppDrawCharRect();
// void mFont_gppDrawCharPoly();
// void mFontChar_gppDraw_sentence_restore();
// void mFontChar_total_scale_reset();
// void mFontChar_ContProc_set_color_char();
// void mFontChar_ContProc_set_char_scale();
// void mFontChar_cont_proc_get();
// void mFontChar_set();
// void mFontChar_gppDrawRect();
// void mFontChar_gppDrawPoly();
// void mFontChar_gppDraw();
// void mFontSentence_line_offset_calc();
// void mFontSentence_ContProc_set_line_offset();
// void mFontSentence_ContProc_set_line_type();
// void mFontSentence_ContProc_set_line_scale();
// void mFontSentence_cont_proc_get();
// void mFontSentence_animal_voice_se();
// void mFontSentence_set();
// void mFontSentence_gppDraw_before();
// void mFontSentence_gppDraw_main();
// void mFontSentence_gppDraw_after();
// void mFontSentence_gppDraw();
// void mFont_Get_MarkTex_p();
// void mFont_Get_MarkTexRom_p();
// void mFont_Get_MarkTexRom_size();
// void mFont_Get_MarkTex_sizeW();
// void mFont_Get_MarkTex_sizeH();
// void mFont_Load_MarkTex();
f32 mFont_SetMarkChar(struct Game*, u8, f32, f32, s32, s32, s32, s32, s32, f32, f32, s32);
// void mFont_gppSetRectMode();
// void mFont_SetRectMode();
void mFont_ct(UNK_PTR arg0, OSMesgQueue arg1[], UNK_PTR arg2);
// void mMsg_CutLeftSpace();
s32 mFont_UnintToString(char*, u32, s32, s32, s32);

#endif
