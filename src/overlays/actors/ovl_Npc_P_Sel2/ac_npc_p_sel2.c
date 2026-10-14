#include "ac_npc_p_sel2.h"
#include "m_lib.h"
#include "m_choice_main.h"
#include "m_actor_dlftbls.h"
#include "m_object.h"
#include "overlays/gamestates/ovl_play/m_play.h"

void aNPS2_actor_ct(Actor* thisx, Game_Play* game_play);
void aNPS2_actor_dt(Actor* thisx, Game_Play* game_play);
void aNPS2_actor_init(Actor* thisx, Game_Play* game_play);
void aNPS2_actor_save(Actor* thisx, Game_Play* game_play);

#ifdef AFTR
//#include "aftr_tools.h"
//extern char aftr_new_player_str[4];
// static char aftr_new_player_str[4] = {0x4E, 0x65, 0x77, 0x21}; // New!
#endif

#if 0
ActorProfile Npc_P_Sel2_Profile = {
    /* */ ACTOR_NPC_P_SEL2,
    /* */ ACTOR_PART_NPC,
    /* */ 0,
    /* */ 0xD01F,
    /* */ GAMEPLAY_KEEP,
    /* */ sizeof(Npc_P_Sel2),
    /* */ aNPS2_actor_ct,
    /* */ aNPS2_actor_dt,
    /* */ aNPS2_actor_init,
    /* */ (void*)none_proc1,
    /* */ aNPS2_actor_save,
};
#endif

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_ct.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_save.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_dt.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_init.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_move.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/aNPS2_actor_draw.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEA0C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEA28_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEA58_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEA98_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEB04_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEB34_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEB98_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEC44_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BECB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BECF8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BED64_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEDE4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEE64_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEF0C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BEFC4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF02C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF070_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF12C_jp.s")


void aNPS2_copy_hokanohito(Npc_P_Sel2* p_sel2, u8* str) {
    Choice* choice_win = mChoice_Get_base_window_p();
 
    mChoice_Load_ChoseStringFromRom((s32)choice_win, (char*)(str + 2 * Choice_CHOICE_STRING_LEN), 0x1B9, (Actor*)p_sel2);
    mChoice_Load_ChoseStringFromRom((s32)choice_win, (char*)(str + 3 * Choice_CHOICE_STRING_LEN), 0x29, (Actor*)p_sel2);
}

//new_player_str (はじめて)
extern u8 D_809C0C38_jp[4];
extern void func_809BF070_jp(void* strBuf, void* strPArray, s32 type);

void aNPS2_set_choice_str(Npc_P_Sel2* p_sel2, volatile s32 useFixedBytes) {
    Choice* choice_win = mChoice_Get_base_window_p();
    u8 str[Choice_CHOICE_STRING_LEN * Choice_CHOICE_MAX];
    u8* str_p[Choice_CHOICE_MAX];
    s32 next_choice_idx = p_sel2->card_player_next_choice_idx;
    s32 i;
    u8* p;
    

    mem_clear(str, sizeof(str), 0x20); //CHAR_SPACE
    for (i = 0; i < Choice_CHOICE_MAX; i++) {
        str_p[i] = NULL;
    }

    switch (next_choice_idx) {
    case 1:
    case 2:
    case 3:
        func_809BF070_jp(str, str_p, next_choice_idx);

        if (useFixedBytes == 0) {
            p = str + next_choice_idx * Choice_CHOICE_STRING_LEN;
            mChoice_Load_ChoseStringFromRom(choice_win, p, Choice_CHOICE_STRING_LEN * Choice_CHOICE_MAX, (Actor*)p_sel2);
        } else {
            p = str + next_choice_idx * Choice_CHOICE_STRING_LEN;
            #ifdef AFTR
            aftr_NewPlayerCopy(p,D_809C0C38_jp,4);
            //mem_copy(p,aftr_new_player_str,4);
            #else
            mem_copy(p,D_809C0C38_jp,4);
            #endif
        }
    
        str_p[next_choice_idx] = p;
        break;
    
    case 4:
        func_809BF070_jp(str, str_p, next_choice_idx);
    }


    mChoice_Set_choice_data(
        choice_win,
        str_p[0], Choice_CHOICE_STRING_LEN,
        str_p[1], Choice_CHOICE_STRING_LEN,
        str_p[2], Choice_CHOICE_STRING_LEN,
        str_p[3], Choice_CHOICE_STRING_LEN);
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF3E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF4C0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF528_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF584_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF690_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF7D4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF824_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF864_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF8A4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BF92C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFAEC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFBB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFD44_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFDF4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFE9C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809BFFA4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0050_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C01A8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0270_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C02BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0308_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C03B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0510_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C05BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0670_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0854_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C08AC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0934_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0964_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C09E4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0A08_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0A38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0A6C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0A98_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0ACC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0B24_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/actors/ovl_Npc_P_Sel2/ac_npc_p_sel2/func_809C0B94_jp.s")
