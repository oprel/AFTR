#include "global.h"
#include "m_tag_ovl.h"
#include "m_common_data.h"
#include "m_lib.h"
#include "m_item_name.h"
#include "m_mail.h"
#include "m_npc.h"
#include "m_private.h"
#include "m_submenu.h"
#include "overlays/submenu/submenu_ovl/m_submenu_ovl.h"

// DATA

/* ---- columns / rows ---- */
static s16 mTG_item_col_pos[6] = { -77, -53, -29, -5, 19, 0 };
static s16 mTG_mail_col_pos[2] = { 55, 79 };
static s16 mTG_money_col_pos[2] = { 16, 0 };
static s16 mTG_player_col_pos[2] = { -55, 0 };
static s16 mTG_bg_col_pos[2] = { 19, 0 };
static s16 mTG_mbox_col_pos[2] = { -36, -12 };
static s16 mTG_haniwa_col_pos[4] = { -35, -11, 13, 37 };
static s16 mTG_collect_col_pos[8] = { -90, -66, -42, -18, 6, 30, 54, 78 };
static s16 mTG_wchange_col_pos[2] = { 105, 0 };
static s16 mTG_cpmail_col_pos[4] = { -16, 8, 32, 56 };
static s16 mTG_cpmail_wc_col_pos[2] = { 98, 0 };
static s16 mTG_cpmail_ti_col_pos[2] = { 24, 0 };
static s16 mTG_cpedit_col_pos[2] = { 20, 0 };
static s16 mTG_cpedit_end_col_pos[2] = { 20, 0 };
static s16 mTG_catalog_col_pos[2] = { 65, 0 };
static s16 mTG_catalog_wc_col_pos[2] = { 93, 0 };
 
static s16 mTG_item_line_pos[4] = { -10, -34, -58, 0 };
static s16 mTG_mail_line_pos[6] = { 38, 14, -10, -34, -58, 0 };
static s16 mTG_money_line_pos[2] = { 18, 0 };
static s16 mTG_player_line_pos[2] = { 51, 0 };
static s16 mTG_bg_line_pos[2] = { -90, 0 };
static s16 mTG_mbox_line_pos[6] = { 39, 15, -9, -33, -57, 0 };
static s16 mTG_haniwa_line_pos[2] = { 46, 0 };
static s16 mTG_collect_line_pos[4] = { 24, 0, -24, -48 };
static s16 mTG_wchange_line_pos[4] = { 37, 0, -37, 0 };
static s16 mTG_cpmail_wc_line_pos[8] = { 50, 35, 20, 5, -10, -25, -40, -55 };
static s16 mTG_cpmail_ti_line_pos[2] = { 71, 0 };
static s16 mTG_cpedit_line_pos[4] = { 50, 18, -14, 0 };
static s16 mTG_cpedit_end_line_pos[2] = { -49, 0 };
static s16 mTG_catalog_line_pos[8] = { 60, 42, 24, 6, -12, -30, -48, 0 };
static s16 mTG_catalog_wc_line_pos[10] = { 64, 48, 32, 16, 0, -16, -32, -48, -64, 0 };
 
mTG_table_c mTG_table_data[16] = {
    { 5, 3, mTG_item_col_pos, mTG_item_line_pos }, /* mTG_TABLE_ITEM */
    { 2, 5, mTG_mail_col_pos, mTG_mail_line_pos }, /* mTG_TABLE_MAIL */
    { 1, 1, mTG_money_col_pos, mTG_money_line_pos }, /* mTG_TABLE_MONEY */
    { 1, 1, mTG_player_col_pos, mTG_player_line_pos }, /* mTG_TABLE_PLAYER */
    { 1, 1, mTG_bg_col_pos, mTG_bg_line_pos }, /* mTG_TABLE_BG */
    { 2, 5, mTG_mbox_col_pos, mTG_mbox_line_pos }, /* mTG_TABLE_MBOX */
    { 4, 1, mTG_haniwa_col_pos, mTG_haniwa_line_pos }, /* mTG_TABLE_HANIWA */
    { 8, 4, mTG_collect_col_pos, mTG_collect_line_pos }, /* mTG_TABLE_COLLECT */
    { 1, 3, mTG_wchange_col_pos, mTG_wchange_line_pos }, /* mTG_TABLE_WCHANGE */
    { 4, 5, mTG_cpmail_col_pos, mTG_mail_line_pos }, /* mTG_TABLE_CPMAIL */
    { 1, 8, mTG_cpmail_wc_col_pos, mTG_cpmail_wc_line_pos }, /* mTG_TABLE_CPMAIL_WC */
    { 1, 1, mTG_cpmail_ti_col_pos, mTG_cpmail_ti_line_pos }, /* mTG_TABLE_CPMAIL_TI */
    { 1, 3, mTG_cpedit_col_pos, mTG_cpedit_line_pos }, /* mTG_TABLE_CPEDIT */
    { 1, 1, mTG_cpedit_end_col_pos, mTG_cpedit_end_line_pos }, /* mTG_TABLE_CPEDIT_END */
    { 1, 7, mTG_catalog_col_pos, mTG_catalog_line_pos }, /* mTG_TABLE_CATALOG */
    { 1, 9, mTG_catalog_wc_col_pos, mTG_catalog_wc_line_pos }, /* mTG_TABLE_CATALOG_WC */
};
 
/* ---- small strings / byte data (symbol boundaries: D_80878AC0 +0x0, AC4 +0x4, ACC +0xC, AD4 +0x14, AD8 +0x18, ADC +0x1C, ADE +0x1E) ---- */
u8 mTG_str_80878AC0[32] = {
    0x04, 0x1F, 0x07, 0xED, /* D_80878AC0_jp */              // おみくじ (Fortune)
    0xAA, 0x8F, 0xE3, 0x90, 0xB9, 0x90, 0xB1, 0x00, /* D_80878AC4_jp */ // ハッピールーム (Happy Room)
    0x04, 0x13, 0xF5, 0x08, 0x5B, 0x18, 0x00, 0x00, /* D_80878ACC_jp */ // おとどけもの (Delivery)
    0x04, 0x12, 0xE7, 0x1F, /* D_80878AD4_jp */             // おてがみ (Letter)
    0x0A, 0xC3, 0x1C, 0x18, /* D_80878AD8_jp */             // さんへの (To / For)
    0x0A, 0xC3, /* D_80878ADC_jp */                         // さん (Mr./Ms.)
    0x60, 0x7C, /* D_80878ADE_jp */                         // より (From)
};


 
/* // JAPANESE
mTG_word_c mTG_word_80878AE0 = { { 0x00, 0x08, 0x7D, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80871ECC_jp }; // あける (Open)
mTG_word_c mTG_word_80878AEC = { { 0x00, 0xEA, 0x7D, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80871F74_jp }; // あげる (Give)
mTG_word_c mTG_word_80878AF8 = { { 0x01, 0x0F, 0xF1, 0x07, 0x20, 0x20, 0x20, 0x20 }, func_80872118_jp }; // いただく (Take it)
mTG_word_c mTG_word_80878B04 = { { 0x02, 0xC3, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80872580_jp }; // うん (Yes)
mTG_word_c mTG_word_80878B10 = { { 0x09, 0x7E, 0xC2, 0x02, 0x7D, 0x20, 0x20, 0x20 }, func_8087207C_jp }; // これをうる (Sell)
mTG_word_c mTG_word_80878B1C = { { 0x04, 0x07, 0x7D, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_808725C8_jp }; // おくる (Send)
mTG_word_c mTG_word_80878B28 = { { 0x05, 0x06, 0x14, 0x04, 0x0C, 0x20, 0x20, 0x20 }, func_80872684_jp }; // かきなおす (Rewrite)
mTG_word_c mTG_word_80878B34 = { { 0x05, 0xF9, 0x15, 0x19, 0x7D, 0x20, 0x20, 0x20 }, func_808726B0_jp }; // かべにはる (Spread)
mTG_word_c mTG_word_80878B40 = { { 0x09, 0x7E, 0xC2, 0x01, 0x7E, 0x7D, 0x20, 0x20 }, func_808727E0_jp }; // これをいれる (Put Away)
mTG_word_c mTG_word_80878B4C = { { 0xED, 0x24, 0xC3, 0x15, 0x02, 0x03, 0x7D, 0x20 }, func_8087287C_jp }; // じめんにうえる (Plant)
mTG_word_c mTG_word_80878B58 = { { 0xED, 0x24, 0xC3, 0x15, 0x04, 0x07, 0x20, 0x20 }, func_80872A34_jp }; // じめんにおく (Drop)
mTG_word_c mTG_word_80878B64 = { { 0x0C, 0x12, 0x7D, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80872DEC_jp }; // すてる (Discard)
mTG_word_c mTG_word_80878B70 = { { 0xA0, 0xD8, 0xF4, 0x00, 0xEA, 0x7D, 0x20, 0x20 }, func_80872E60_jp }; // タダであげる (Give)
mTG_word_c mTG_word_80878B7C = { { 0x11, 0x05, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80872E84_jp }; // つかむ (Grab)
mTG_word_c mTG_word_80878B88 = { { 0x12, 0xE7, 0x1F, 0xC2, 0x05, 0x07, 0x20, 0x20 }, func_808731EC_jp }; // てがみをかく (Write)
mTG_word_c mTG_word_80878B94 = { { 0x17, 0xF1, 0xC3, 0xC2, 0x11, 0x08, 0x7D, 0x20 }, func_80873278_jp }; // ねだんをつける (Price)
mTG_word_c mTG_word_80878BA0 = { { 0xE4, 0xBA, 0xD6, 0xBD, 0xA4, 0x20, 0x20, 0x20 }, func_80873348_jp }; // プレゼント (Present)
mTG_word_c mTG_word_80878BAC = { { 0x1F, 0x0D, 0x7D, 0xF1, 0x08, 0x20, 0x20, 0x20 }, func_80873428_jp }; // みせるだけ (Display)
mTG_word_c mTG_word_80878BB8 = { { 0x5D, 0xCC, 0xFB, 0x5D, 0x24, 0x7D, 0x20, 0x20 }, func_8087344C_jp }; // やっぱやめる (Quit)
mTG_word_c mTG_word_80878BC4 = { { 0x1C, 0x5D, 0x15, 0x04, 0x07, 0x20, 0x20, 0x20 }, func_80872B54_jp }; // へやにおく (Drop)
mTG_word_c mTG_word_80878BD0 = { { 0x5E, 0x05, 0x15, 0x0B, 0x07, 0x20, 0x20, 0x20 }, func_80872748_jp }; // ゆかにしく (Spread)
mTG_word_c mTG_word_80878BDC = { { 0x60, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873498_jp }; // よむ (Read)
mTG_word_c mTG_word_80878BE8 = { { 0xC1, 0x0F, 0x0C, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873510_jp }; // わたす (Give)
mTG_word_c mTG_word_80878BF4 = { { 0x31, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873694_jp }; // 100 (100)
mTG_word_c mTG_word_80878C00 = { { 0x31, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20 }, func_808736B8_jp }; // 1000 (1000)
mTG_word_c mTG_word_80878C0C = { { 0x31, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20 }, func_808736DC_jp }; // 10000 (10000)
mTG_word_c mTG_word_80878C18 = { { 0x33, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20 }, func_80873700_jp }; // 30000 (30000)
mTG_word_c mTG_word_80878C24 = { { 0x04, 0x05, 0x17, 0x3A, 0x20, 0x20, 0x20, 0x20 }, 0 }; // おかね (Price)
mTG_word_c mTG_word_80878C30 = { { 0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0xB9, 0x20 }, 0 }; // ベル (Bells)
mTG_word_c mTG_word_80878C3C = { { 0x04, 0x0A, 0x24, 0x7D, 0x20, 0x20, 0x20, 0x20 }, func_808727E0_jp }; // おさめる (Give)
mTG_word_c mTG_word_80878C48 = { { 0xEF, 0xC3, 0xF8, 0x11, 0x05, 0x23, 0x20, 0x20 }, func_80872E84_jp }; // ぜんぶつかむ (Grab All)
mTG_word_c mTG_word_80878C54 = { { 0x31, 0x1E, 0x01, 0x11, 0x05, 0x23, 0x20, 0x20 }, func_80873724_jp }; // 1まいつかむ (Grab One)
mTG_word_c mTG_word_80878C60 = { { 0x10, 0xCA, 0x02, 0x5B, 0xC3, 0x0C, 0x7D, 0x20 }, func_808737F4_jp }; // ちゅうもんする (Order)
mTG_word_c mTG_word_80878C6C = { { 0xED, 0x24, 0xC3, 0x15, 0x02, 0x24, 0x7D, 0x20 }, func_808738C8_jp }; // じめんにうめる (Bury)
mTG_word_c mTG_word_80878C78 = { { 0x15, 0xE7, 0x0C, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_808739B0_jp }; // にがす (Release)
mTG_word_c mTG_word_80878C84 = { { 0x00, 0x08, 0x7D, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873C88_jp }; // あける (Open)
mTG_word_c mTG_word_80878C90 = { { 0x08, 0x0C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80872DEC_jp }; // けす (Erase)
mTG_word_c mTG_word_80878C9C = { { 0x19, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873F38_jp }; // はい (Yes)
mTG_word_c mTG_word_80878CA8 = { { 0x01, 0x01, 0x03, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_8087344C_jp }; // いいえ (No)
*/
 
//ENGLISH

mTG_word_c mTG_word_80878AE0 = { { 0x4F, 0x70, 0x65, 0x6E, 0x20, 0x20, 0x20, 0x20 }, func_80871ECC_jp }; // あける (Open)
mTG_word_c mTG_word_80878AEC = { { 0x47, 0x69, 0x76, 0x65, 0x20, 0x20, 0x20, 0x20 }, func_80871F74_jp }; // あげる (Give)
mTG_word_c mTG_word_80878AF8 = { { 0x54, 0x61, 0x6B, 0x65, 0x20, 0x69, 0x74, 0x20 }, func_80872118_jp }; // いただく (Take it)
mTG_word_c mTG_word_80878B04 = { { 0x59, 0x65, 0x73, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80872580_jp }; // うん (Yes)
mTG_word_c mTG_word_80878B10 = { { 0x53, 0x65, 0x6C, 0x6C, 0x20, 0x20, 0x20, 0x20 }, func_8087207C_jp }; // これをうる (Sell)
mTG_word_c mTG_word_80878B1C = { { 0x53, 0x65, 0x6E, 0x64, 0x20, 0x20, 0x20, 0x20 }, func_808725C8_jp }; // おくる (Send)
mTG_word_c mTG_word_80878B28 = { { 0x52, 0x65, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20 }, func_80872684_jp }; // かきなおす (Rewrite)
mTG_word_c mTG_word_80878B34 = { { 0x53, 0x70, 0x72, 0x65, 0x61, 0x64, 0x20, 0x20 }, func_808726B0_jp }; // かべにはる (Spread)
mTG_word_c mTG_word_80878B40 = { { 0x50, 0x75, 0x74, 0x20, 0x41, 0x77, 0x61, 0x79 }, func_808727E0_jp }; // これをいれる (Put Away)
mTG_word_c mTG_word_80878B4C = { { 0x50, 0x6C, 0x61, 0x6E, 0x74, 0x20, 0x20, 0x20 }, func_8087287C_jp }; // じめんにうえる (Plant)
mTG_word_c mTG_word_80878B58 = { { 0x44, 0x72, 0x6F, 0x70, 0x20, 0x20, 0x20, 0x20 }, func_80872A34_jp }; // じめんにおく (Drop)
mTG_word_c mTG_word_80878B64 = { { 0x44, 0x69, 0x73, 0x63, 0x61, 0x72, 0x64, 0x20 }, func_80872DEC_jp }; // すてる (Discard)
mTG_word_c mTG_word_80878B70 = { { 0x47, 0x69, 0x76, 0x65, 0x20, 0x20, 0x20, 0x20 }, func_80872E60_jp }; // タダであげる (Give)
mTG_word_c mTG_word_80878B7C = { { 0x47, 0x72, 0x61, 0x62, 0x20, 0x20, 0x20, 0x20 }, func_80872E84_jp }; // つかむ (Grab)
mTG_word_c mTG_word_80878B88 = { { 0x57, 0x72, 0x69, 0x74, 0x65, 0x20, 0x20, 0x20 }, func_808731EC_jp }; // てがみをかく (Write)
mTG_word_c mTG_word_80878B94 = { { 0x50, 0x72, 0x69, 0x63, 0x65, 0x20, 0x20, 0x20 }, func_80873278_jp }; // ねだんをつける (Price)
mTG_word_c mTG_word_80878BA0 = { { 0x50, 0x72, 0x65, 0x73, 0x65, 0x6E, 0x74, 0x20 }, func_80873348_jp }; // プレゼント (Present)
mTG_word_c mTG_word_80878BAC = { { 0x44, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x20 }, func_80873428_jp }; // みせるだけ (Display)
mTG_word_c mTG_word_80878BB8 = { { 0x51, 0x75, 0x69, 0x74, 0x20, 0x20, 0x20, 0x20 }, func_8087344C_jp }; // やっぱやめる (Quit)
mTG_word_c mTG_word_80878BC4 = { { 0x44, 0x72, 0x6F, 0x70, 0x20, 0x20, 0x20, 0x20 }, func_80872B54_jp }; // へやにおく (Drop)
mTG_word_c mTG_word_80878BD0 = { { 0x53, 0x70, 0x72, 0x65, 0x61, 0x64, 0x20, 0x20 }, func_80872748_jp }; // ゆかにしく (Spread)
mTG_word_c mTG_word_80878BDC = { { 0x52, 0x65, 0x61, 0x64, 0x20, 0x20, 0x20, 0x20 }, func_80873498_jp }; // よむ (Read)
mTG_word_c mTG_word_80878BE8 = { { 0x47, 0x69, 0x76, 0x65, 0x20, 0x20, 0x20, 0x20 }, func_80873510_jp }; // わたす (Give)
mTG_word_c mTG_word_80878BF4 = { { 0x31, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873694_jp }; // 100 (100)
mTG_word_c mTG_word_80878C00 = { { 0x31, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20 }, func_808736B8_jp }; // 1000 (1000)
mTG_word_c mTG_word_80878C0C = { { 0x31, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20 }, func_808736DC_jp }; // 10000 (10000)
mTG_word_c mTG_word_80878C18 = { { 0x33, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20 }, func_80873700_jp }; // 30000 (30000)
mTG_word_c mTG_word_80878C24 = { { 0x50, 0x72, 0x69, 0x63, 0x65, 0x20, 0x20, 0x20 }, 0 }; // おかね (Price)
mTG_word_c mTG_word_80878C30 = { { 0x42, 0x65, 0x6C, 0x6C, 0x73, 0x20, 0x20, 0x20 }, 0 }; // ベル (Bells)
mTG_word_c mTG_word_80878C3C = { { 0x47, 0x69, 0x76, 0x65, 0x20, 0x20, 0x20, 0x20 }, func_808727E0_jp }; // おさめる (Give)
mTG_word_c mTG_word_80878C48 = { { 0x47, 0x72, 0x61, 0x62, 0x20, 0x41, 0x6C, 0x6C }, func_80872E84_jp }; // ぜんぶつかむ (Grab All)
mTG_word_c mTG_word_80878C54 = { { 0x47, 0x72, 0x61, 0x62, 0x20, 0x4F, 0x6E, 0x65 }, func_80873724_jp }; // 1まいつかむ (Grab One)
mTG_word_c mTG_word_80878C60 = { { 0x4F, 0x72, 0x64, 0x65, 0x72, 0x20, 0x20, 0x20 }, func_808737F4_jp }; // ちゅうもんする (Order)
mTG_word_c mTG_word_80878C6C = { { 0x42, 0x75, 0x72, 0x79, 0x20, 0x20, 0x20, 0x20 }, func_808738C8_jp }; // じめんにうめる (Bury)
mTG_word_c mTG_word_80878C78 = { { 0x52, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20 }, func_808739B0_jp }; // にがす (Release)
mTG_word_c mTG_word_80878C84 = { { 0x4F, 0x70, 0x65, 0x6E, 0x20, 0x20, 0x20, 0x20 }, func_80873C88_jp }; // あける (Open)
mTG_word_c mTG_word_80878C90 = { { 0x45, 0x72, 0x61, 0x73, 0x65, 0x20, 0x20, 0x20 }, func_80872DEC_jp }; // けす (Erase)
mTG_word_c mTG_word_80878C9C = { { 0x59, 0x65, 0x73, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_80873F38_jp }; // はい (Yes)
mTG_word_c mTG_word_80878CA8 = { { 0x4E, 0x6F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, func_8087344C_jp }; // いいえ (No)

/* ---- lists of tag words (menu choices) ---- */
mTG_word_c* mTG_list_80878CB4[3] = { &mTG_word_80878B7C, &mTG_word_80878B58, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878CC0[4] = { &mTG_word_80878B7C, &mTG_word_80878B58, &mTG_word_80878C6C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878CD0[4] = { &mTG_word_80878B7C, &mTG_word_80878B88, &mTG_word_80878B58, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878CE0[5] = { &mTG_word_80878B7C, &mTG_word_80878B88, &mTG_word_80878B58, &mTG_word_80878C6C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878CF4[4] = { &mTG_word_80878B7C, &mTG_word_80878C84, &mTG_word_80878B58, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D04[5] = { &mTG_word_80878B7C, &mTG_word_80878C84, &mTG_word_80878B58, &mTG_word_80878C6C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D18[3] = { &mTG_word_80878B7C, &mTG_word_80878C78, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D24[2] = { &mTG_word_80878B7C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D2C[4] = { &mTG_word_80878B7C, &mTG_word_80878B58, &mTG_word_80878B4C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D3C[3] = { &mTG_word_80878B7C, &mTG_word_80878B4C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D48[3] = { &mTG_word_80878B7C, &mTG_word_80878AE0, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D54[3] = { &mTG_word_80878B7C, &mTG_word_80878BC4, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D60[4] = { &mTG_word_80878B7C, &mTG_word_80878BC4, &mTG_word_80878B34, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D70[4] = { &mTG_word_80878B7C, &mTG_word_80878BC4, &mTG_word_80878BD0, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D80[4] = { &mTG_word_80878B7C, &mTG_word_80878B88, &mTG_word_80878BC4, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878D90[4] = { &mTG_word_80878B7C, &mTG_word_80878C84, &mTG_word_80878BC4, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DA0[3] = { &mTG_word_80878B7C, &mTG_word_80878B88, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DAC[3] = { &mTG_word_80878B7C, &mTG_word_80878C84, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DB8[4] = { &mTG_word_80878BDC, &mTG_word_80878B7C, &mTG_word_80878B64, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DC8[3] = { &mTG_word_80878BDC, &mTG_word_80878B7C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DD4[4] = { &mTG_word_80878BDC, &mTG_word_80878B7C, &mTG_word_80878BA0, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DE4[4] = { &mTG_word_80878B28, &mTG_word_80878B7C, &mTG_word_80878B64, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878DF4[3] = { &mTG_word_80878B28, &mTG_word_80878B7C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E00[4] = { &mTG_word_80878B28, &mTG_word_80878B7C, &mTG_word_80878BA0, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E10[2] = { &mTG_word_80878B04, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E18[2] = { &mTG_word_80878BF4, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E20[3] = { &mTG_word_80878BF4, &mTG_word_80878C00, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E2C[4] = { &mTG_word_80878BF4, &mTG_word_80878C00, &mTG_word_80878C0C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E3C[5] = { &mTG_word_80878BF4, &mTG_word_80878C00, &mTG_word_80878C0C, &mTG_word_80878C18, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E50[2] = { &mTG_word_80878BE8, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E58[2] = { &mTG_word_80878B10, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E60[2] = { &mTG_word_80878AEC, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E68[2] = { &mTG_word_80878B1C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E70[4] = { &mTG_word_80878C48, &mTG_word_80878C54, &mTG_word_80878B58, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E80[5] = { &mTG_word_80878C48, &mTG_word_80878C54, &mTG_word_80878B58, &mTG_word_80878C6C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878E94[4] = { &mTG_word_80878C48, &mTG_word_80878C54, &mTG_word_80878BC4, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EA4[3] = { &mTG_word_80878C48, &mTG_word_80878C54, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EB0[2] = { &mTG_word_80878B40, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EB8[2] = { &mTG_word_80878C3C, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EC0[5] = { &mTG_word_80878B7C, &mTG_word_80878B70, &mTG_word_80878B94, &mTG_word_80878BAC, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878ED4[3] = { &mTG_word_80878B70, &mTG_word_80878B94, &mTG_word_80878BAC };
mTG_word_c* mTG_list_80878EE0[2] = { &mTG_word_80878AF8, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EE8[2] = { &mTG_word_80878C24, &mTG_word_80878C30 };
mTG_word_c* mTG_list_80878EF0[2] = { &mTG_word_80878C60, &mTG_word_80878BB8 };
mTG_word_c* mTG_list_80878EF8[2] = { &mTG_word_80878BB8, &mTG_word_80878C90 };
mTG_word_c* mTG_list_80878F00[2] = { &mTG_word_80878CA8, &mTG_word_80878C9C };
 
typedef struct mTG_list_s {
    mTG_word_c** words;
    s32 lines;
} mTG_list_c;
 
mTG_list_c mTG_tag_list_data[44] = {
    { 0, 0 },
    { mTG_list_80878CB4, 3 },
    { mTG_list_80878CC0, 4 },
    { mTG_list_80878CD0, 4 },
    { mTG_list_80878CE0, 5 },
    { mTG_list_80878CF4, 4 },
    { mTG_list_80878D04, 5 },
    { mTG_list_80878D18, 3 },
    { mTG_list_80878D24, 2 },
    { mTG_list_80878D2C, 4 },
    { mTG_list_80878D3C, 3 },
    { mTG_list_80878D48, 3 },
    { mTG_list_80878D54, 3 },
    { mTG_list_80878D60, 4 },
    { mTG_list_80878D70, 4 },
    { mTG_list_80878D80, 4 },
    { mTG_list_80878D90, 4 },
    { mTG_list_80878DA0, 3 },
    { mTG_list_80878DAC, 3 },
    { mTG_list_80878DB8, 4 },
    { mTG_list_80878DC8, 3 },
    { mTG_list_80878DD4, 4 },
    { mTG_list_80878DE4, 4 },
    { mTG_list_80878DF4, 3 },
    { mTG_list_80878E00, 4 },
    { mTG_list_80878E10, 2 },
    { mTG_list_80878E2C, 4 },
    { mTG_list_80878E50, 2 },
    { mTG_list_80878E58, 2 },
    { mTG_list_80878E60, 2 },
    { mTG_list_80878E68, 2 },
    { mTG_list_80878E70, 4 },
    { mTG_list_80878E80, 5 },
    { mTG_list_80878E94, 4 },
    { mTG_list_80878EA4, 3 },
    { mTG_list_80878EB0, 2 },
    { mTG_list_80878EB8, 2 },
    { mTG_list_80878EC0, 5 },
    { mTG_list_80878ED4, 3 },
    { mTG_list_80878EE8, 2 },
    { mTG_list_80878EE0, 2 },
    { mTG_list_80878EF0, 2 },
    { mTG_list_80878EF8, 2 },
    { mTG_list_80878F00, 2 },
};
 
/* ---- misc bytes before the window table (D_80879068 +0x0, D_80879070 +0x8, D_80879074 +0xC) ---- */
u8 mTG_str_80879068[20] = {
    0x0C, 0x12, 0x7D, 0x18, 0x3F, 0x00, 0x00, 0x00, // D_80879068_jp  すてるの?  (Discard?)
    0xAE, 0xBD, 0xA4, 0x15, // D_80879070_jp ホントに (Really?)
    0x01, 0x01, 0xF4, 0x0C, 0x05, 0x3F, 0x00, 0x00, // D_80879074_jp  いいですか?  (Is that okay?)
};
 
/* ---- window layout table (edit these values) ---- */
mTG_win_data_c D_8087907C_jp[3] = {
    {   /* item name window */
        { 0.3409091f, 0.8125f },
        { -88.0f, 16.0f },
        1.125f,
        2,
        8,
        { 18.0f, -5.0f },
        { 10.0f, -3.0f },
        { -14.0f, 2.0f },
        { 0.0f, 9.0f },
        8.0f,
    },
    {   /* quest / mail window */
        { 0.47333333f, 0.8787879f },
        { -75.0f, 33.0f },
        1.0f,
        4, /* minWidth (characters) */
        6, /* widthRange (characters) */
        { 18.0f, -11.0f },
        { 12.0f, -4.0f },
        { -8.0f, 4.0f },
        { -2.0f, 25.0f },
        8.0f,
    },
    {   /* select window */
        { 0.5882353f, 0.5625f },
        { -68.0f, 64.0f },
        0.8333333f,
        3, /* minWidth (characters) */
        4, /* widthRange (characters) */
        { 26.0f, -20.0f },
        { 4.0f, -4.0f },
        { 1.0f, 16.0f },
        { 6.0f, 22.0f },
        12.0f,
    },
};
typedef char mTG_win_data_size_check[(sizeof(mTG_win_data_c) == 0x40) ? 1 : -1];
 
/* ---- strings ---- */
u8 D_8087913C_jp[8] = { 0xE4, 0xBA, 0xD6, 0xBD, 0xA4, 0x00, 0x00, 0x00 }; // "Present" 
u8 D_80879144_jp[4] = { 0x19, 0x19, 0x00, 0x00 }; // mother mail sender name (2 bytes + 2 pad)  -> Mama
u8 D_80879148_jp[9][6] = { /* catalog category names, 6 bytes each */
    { 0x05, 0xE9, 0x20, 0x20, 0x20, 0x20 }, // かぐ (Furniture) -> Furn.
    { 0x05, 0xF9, 0xE7, 0x1F, 0x20, 0x20 }, // かべがみ (Wallpaper) -> Wallp.
    { 0xED, 0xCA, 0x02, 0x0F, 0xC3, 0x20 }, // じゅうたん (Carpet)
    { 0x1B, 0x07, 0x20, 0x20, 0x20, 0x20 }, // ふく (Clothing) -> Shirts
    { 0x05, 0x0A, 0x20, 0x20, 0x20, 0x20 }, // かさ (Items)
    { 0xF7, 0xC3, 0x0D, 0xC3, 0x20, 0x20 }, // びんせん (Stationery) -> Paper
    { 0x19, 0x15, 0xC1, 0x20, 0x20, 0x20 }, // はにわ (Gyroids) -> Gyroid
    { 0x05, 0x0D, 0x06, 0x20, 0x20, 0x20 }, // かせき (Fossils) -> Fossil
    { 0xB0, 0x8D, 0x90, 0xD4, 0x8F, 0x98 }, // ミュージック (Music)
};
 
/* ---- float tables ---- */
f32 mTG_flt_80879180[6] = { -3.0f, 22.0f, 3.0f, -22.0f, -5.0f, -22.0f };
f32 mTG_flt_80879198[6] = { 0.0f, 18.0f, 1.0f, -20.0f, -2.0f, -24.0f };
f32 mTG_flt_808791B0[8] = { 32.0f, 5.0f, 22.0f, 0.0f, 41.0f, -3.0f, 24.0f, -1.0f };
f32 mTG_flt_808791D0[8] = { 40.0f, 7.0f, 8.0f, 3.0f, 54.0f, -8.0f, 6.0f, 0.0f };
f32 mTG_flt_808791F0[8] = { -22.0f, 10.0f, -6.0f, 8.0f, -33.0f, -9.0f, -9.0f, -7.0f };
f32 mTG_flt_80879210[16] = { 0.0f, 40.0f, 40.0f, 40.0f, 40.0f, 0.0f, 40.0f, -40.0f, 0.0f, -40.0f, -40.0f, -40.0f, -40.0f, 0.0f, -40.0f, 40.0f };
f32 mTG_flt_80879250[16] = { 0.0f, 40.0f, 40.0f, 40.0f, 40.0f, 0.0f, 40.0f, -40.0f, 0.0f, -40.0f, -40.0f, -40.0f, -40.0f, 0.0f, -40.0f, 40.0f };
 
/* ---- misc tables ---- */
s16 mTG_s16_80879290[4] = { 0x2103, 0x2100, 0x2101, 0x2102 };
s32 mTG_money_values_80879298[4] = { 100, 1000, 10000, 30000 };
s16 mTG_s16_808792A8[8] = { 0x1D28, 0x1D2C, 0x1D30, 0x1D34, 0x1D38, 0x1D3C, 0x1D40, 0x0000 };  /* last element is 0 */
s32 mTG_s32_808792B8[5] = { 10000, 1000, 100, 10, 1 };
s32 mTG_s32_808792CC[16] = { 3, 1, 1, 8, 1, 1, 1, 1, 1, 9, 1, 1, 31, 7, 5, 1 };
s32 mTG_s32_8087930C[16] = { 15, 12, 12, 12, 12, 12, 14, 13, 12, 12, 12, 12, 33, 12, 12, 12 };
s32 mTG_s32_8087934C[16] = { 17, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 34, 8, 18, 8 };
s32* mTG_s32_ptrs_8087938C[4] = { mTG_s32_808792CC, mTG_s32_8087930C, mTG_s32_8087934C, mTG_s32_8087934C };
s32 mTG_s32_8087939C[4] = { 22, 24, 19, 21 };

typedef void (*mTG_proc)();

mTG_proc mTG_proc_808793AC[16] = {
    func_808757C4_jp,
    func_80875888_jp,
    func_808759C8_jp,
    func_80875A84_jp,
    func_80875AB8_jp,
    func_80875888_jp,
    func_80875AD0_jp,
    func_80875B60_jp,
    func_80875B88_jp,
    func_80875888_jp,
    func_80875BF4_jp,
    func_80875C60_jp,
    func_80875CF0_jp,
    func_80875D38_jp,
    func_80875DB0_jp,
    func_80875E20_jp,
};

u8 mTG_rgba_808793EC[3][4] = {
    { 0x78, 0x23, 0xFF, 0xFF },
    { 0x46, 0x46, 0x64, 0xFF },
    { 0x87, 0x14, 0xE1, 0xFF },
};
u8 mTG_rgba_808793F8[3][4] = {
    { 0x8C, 0x96, 0xBE, 0xFF },
    { 0x82, 0x82, 0x96, 0xFF },
    { 0x91, 0x87, 0xC3, 0xFF },
};
u8 mTG_rgba_80879404[9][4] = {
    { 0xFF, 0xF5, 0x8C, 0xFF },
    { 0xBE, 0xEB, 0xF5, 0xFF },
    { 0xCD, 0xF5, 0xFF, 0xFF },
    { 0xFF, 0xF5, 0x8C, 0xFF },
    { 0xCD, 0xC3, 0xFF, 0xFF },
    { 0xD7, 0xCD, 0xFF, 0xFF },
    { 0xFF, 0xC3, 0xFF, 0xFF },
    { 0xD7, 0xD7, 0xFF, 0xFF },
    { 0xD7, 0xD7, 0xFF, 0xFF },
};
u8 mTG_rgba_80879428[9][4] = {
    { 0xD7, 0xA5, 0x3C, 0xFF },
    { 0x78, 0x78, 0xC8, 0xFF },
    { 0x6E, 0x6E, 0xC8, 0xFF },
    { 0xD7, 0xA5, 0x3C, 0xFF },
    { 0x6E, 0x6E, 0x8C, 0xFF },
    { 0x6E, 0x6E, 0x8C, 0xFF },
    { 0xC3, 0x41, 0x6E, 0xFF },
    { 0x96, 0x4B, 0xFA, 0xFF },
    { 0x96, 0x4B, 0xFA, 0xFF },
};

/* 0x0C...... values are segment-0C (display list / texture) addresses */
u32 mTG_seg_8087944C[15] = {
    0x0C002028, 0x0C001FC0, 0x0C002090, 0x0C0020F0, 0x0C002150,
    0x0C000228, 0x0C000298, 0x0C000300, 0x0C000360, 0x0C0003C0,
    0x0C004660, 0x0C004810, 0x0C0047B0, 0x0C0048C8, 0x0C004928,
};
u32 mTG_seg_80879488[5] = {
    0x0C000228, 0x0C000298, 0x0C000428, 0x0C000490, 0x0C0004F8,
};
u8 mTG_rgba_8087949C[3][4] = {
    { 0xD7, 0x1E, 0xD7, 0xFF },
    { 0xEB, 0x3C, 0x3C, 0xFF },
    { 0xFF, 0x00, 0xFF, 0xFF },
};
u32 mTG_u32_808794A8[3] = {
    0x000000FF, 0x00000014, 0x00000014,
};
u32 mTG_u32_808794B4[3] = {
    0x0000009B, 0x00000032, 0x0000005F,
};
u8 mTG_rgba_808794C0[8][4] = {  /* D_808794C0 .. D_808794DC, 4 bytes each */
    { 0xCD, 0x28, 0x28, 0xFF },
    { 0x64, 0x41, 0xC3, 0xFF },
    { 0x3C, 0x96, 0x41, 0xFF },
    { 0xA5, 0x1E, 0xFF, 0xFF },
    { 0x3C, 0x32, 0x9B, 0xFF },
    { 0xE1, 0x1E, 0xDC, 0xFF },
    { 0x5A, 0x3C, 0x32, 0xFF },
    { 0x28, 0x1E, 0x1E, 0xFF },
};
u32 mTG_u32_808794E0[4] = {
    0x00A6B000, 0x00A75B90, 0x00000000, 0x00000000,
};


// FUNCTIONS

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F310_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F35C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F44C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F4AC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F644_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F66C_jp.s")

//mTG_set_tag_win_scale_p
void func_8086F764_jp(mTG_tag_c* tag, s32 win_type, s32 width, s32 height) {
    mTG_win_data_c* win_data = &D_8087907C_jp[win_type];
    int i;

    tag->unk_04[0] = (f32)(width - win_data->minWidth) / (f32)win_data->widthRange;

    if (height != 0) {
        tag->unk_04[1] = (f32)(height - 2) / 3.0f;
    } else {
        tag->unk_04[1] = tag->unk_04[0];
    }

    for (i = 0; i < 2; i++) {
        tag->bodyScale[i] = win_data->unk_00[i] + tag->unk_04[i] * (1.0f - win_data->unk_00[i]);
        tag->arrowScale[i] = win_data->unk_10 + tag->unk_04[i] * (1.0f - win_data->unk_10);
        tag->textOfs[i] = win_data->unk_1C[i] + win_data->unk_24[i] * tag->unk_04[i] +
                          win_data->unk_08[i] * tag->bodyScale[i];
        tag->bodyOfs[i] = (win_data->unk_34[i] + win_data->unk_2C[i] * tag->unk_04[i]) -
                          win_data->unk_08[i] * tag->bodyScale[i];
    }

    tag->bodyOfs[1] += tag->arrowScale[1] * 0.5f * win_data->unk_3C;
}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F910_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F960_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086F9D8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FA30_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FA88_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FB04_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FB9C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FBE4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FCF4_jp.s")

//#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8086FD3C_jp.s")


/* SubmenuOverlay (submenu->overlay) isn't decompiled far enough yet, so use raw offsets at the use sites. */
typedef u16 (*SetCollectItemNoProc)(s32 idx, s32 pageOrder);

//mTG_init_tag_data_item_win
void func_8086FD3C_jp(Submenu* submenu) {
    mTG_tag_c* tag = (mTG_tag_c*)(*(u8**)((u8*)submenu->overlay + 0x106D0) + 0x8);
    u16 itemNo = 0;
    Mail_c* mail = NULL;
    s32 itemCond = mPr_ITEM_COND_NORMAL;
    s32 idx = func_8086F910_jp(tag);
    s32 width;
    s32 width1;
 
    func_8086F35C_jp(submenu, tag->basePos, tag->table, idx);
    mem_clear(tag->str0, TAG_STR0_LEN, CHAR_SPACE);
    mem_clear(tag->str1, TAG_STR1_LEN, CHAR_SPACE);
 
    /* == mTG_init_tag_data_set_itemNo() */
    switch (tag->table) {
        case mTG_TABLE_ITEM:
            itemNo = COMMON_GET(privateInfo)->inventory.pockets[idx];
            itemCond = mPr_GET_ITEM_COND(COMMON_GET(privateInfo)->inventory.item_conditions, idx);
            break;
 
        case mTG_TABLE_PLAYER:
            itemNo = *(u16*)((u8*)common_data.privateInfo + 0x3EC);
            break;
 
        case mTG_TABLE_HANIWA: {
            u8* menuInfo = (u8*)submenu->overlay + 0x10478;
            itemNo = SAVE_GET(homes)[*(s32*)(menuInfo + 0x3C)].haniwa.items[idx].item;
            break;
        }
 
        case mTG_TABLE_COLLECT: {
            u8* inventoryOvl = *(u8**)((u8*)submenu->overlay + 0x106DC);
 
            itemNo = (*(SetCollectItemNoProc*)(inventoryOvl + 0x5D8))(
                idx, *(u8*)(inventoryOvl + 0x3EE));
            break;
        }
 
        case mTG_TABLE_MAIL:
        case mTG_TABLE_MBOX:
        case mTG_TABLE_CPMAIL:
            mail = func_8086FBE4_jp(submenu, NULL);
            break;
 
        default:
            break;
    }
 
    if (itemNo != 0 || (mail != NULL && mMl_check_not_used_mail(mail) == FALSE)) {
        /* == mTG_init_tag_data_item_win_sub_mail_item() */
        if (itemNo == 0) {
            /* A letter: str0 = recipient, str1 = sender (or a special sender) */
            mem_copy(tag->str0, (u8*)mail->header.recipient.personalID.playerName, PLAYER_NAME_LEN);
 
            if (mail->content.mailType == 0) {
                mem_copy(tag->str1, (u8*)mail->header.sender.personalID.playerName, PLAYER_NAME_LEN);
 
                if (mail->header.recipient.type == 2) {
                    tag->str2Type = mTG_QSTR_TYPE_TO_MUSEUM;
                } else if (mail->header.sender.type == 2) {
                    tag->str2Type = mTG_QSTR_TYPE_FROM_MUSEUM;
                } else {
                    tag->str2Type = mTG_QSTR_TYPE_MAIL;
                }
            } else if (mail->content.mailType == 1) {
                tag->str2Type = mTG_QSTR_TYPE_XMAS_SNOWMAN_SPNPC;
                mNpc_GetNpcWorldNameP((char*)tag->str1, 0xD00F);
            } else if (mail->content.mailType == 2 || mail->content.mailType == 7) {
                tag->str2Type = mTG_QSTR_TYPE_SHOP;
                mNpc_GetNpcWorldNameP((char*)tag->str1, 0xD008);
            } else if (mail->content.mailType == 3) {
                tag->str2Type = mTG_QSTR_TYPE_SHOP;
                mNpc_GetNpcWorldNameP((char*)tag->str1, 0xD001);
            } else if (mail->content.mailType == 4) {
                tag->str2Type = mTG_QSTR_TYPE_MOTHER;
                mem_copy(tag->str1, D_80879144_jp, sizeof(D_80879144_jp));
            } else if (mail->content.mailType == 5) {
                tag->str2Type = mTG_QSTR_TYPE_ANGLER;
                mNpc_GetNpcWorldNameP((char*)tag->str1, 0xD03D);
            } else if (mail->content.mailType == 8) {
                tag->str2Type = mTG_QSTR_TYPE_XMAS_SNOWMAN_SPNPC;
                mNpc_GetNpcWorldNameP((char*)tag->str1, 0x800D);
            } else {
                tag->str2Type = mTG_QSTR_TYPE_OMIKUJI;
            }
        } else if (tag->table == mTG_TABLE_ITEM) {
            /* quest item: "delivery for X from Y" */
            tag->str2Type = func_8086FB9C_jp(tag, itemCond, idx);
        } else {
            tag->str2Type = mTG_QSTR_TYPE_NONE;
        }
 
        if (tag->str2Type != mTG_QSTR_TYPE_NONE) {
            #ifdef AFTR
            width = aftr_GetStringWidth((char*)tag->str0, PLAYER_NAME_LEN, TRUE) + 4;
            width1 = aftr_GetStringWidth((char*)tag->str1, PLAYER_NAME_LEN, TRUE) + 4;
            #else
            /* Two-line window (str0 over str1); widths are in characters on N64 */
            width = mMl_strlen((char*)tag->str0, PLAYER_NAME_LEN, CHAR_SPACE) + 4;
            width1 = mMl_strlen((char*)tag->str1, PLAYER_NAME_LEN, CHAR_SPACE) + 4;
            #endif
 
            if (tag->str2Type == mTG_QSTR_TYPE_TO_MUSEUM) {
                width -= 2;
            } else if (tag->str2Type == mTG_QSTR_TYPE_FROM_MUSEUM || tag->str2Type == mTG_QSTR_TYPE_MOTHER) {
                width1 -= 2;
            } else if (tag->str2Type == mTG_QSTR_TYPE_ANGLER) {
                width1 -= 4;
            } else if (tag->str2Type == mTG_QSTR_TYPE_OMIKUJI) {
                width1 = 7;
            }
 
            if (width < width1) {
                width = width1;
            }
 
            if (tag->str2Type == mTG_QSTR_TYPE_ITEM) {
                if (width < 6) {
                    width = 6;
                }
            } else {
                if (width < 4) {
                    width = 4;
                }
            }
 
            func_8086F764_jp(tag, 1, width, 0);
 
            if (func_8086FCF4_jp(tag) != 0) {
                tag->bodyOfs[1] += 7.0f;
                tag->bodyOfs[0] -= 0.6f + (width - 4) * 0.3f;
            }
        } else {
            /* Plain item name window */
            if (itemCond == mPr_ITEM_COND_PRESENT) {
                mem_copy(tag->str0, D_8087913C_jp, sizeof(D_8087913C_jp));
            } else {
                mIN_copy_name_str(tag->str0, itemNo);
            }
 
            #ifdef AFTR
            func_8086F764_jp(tag, 0, aftr_GetStringWidth((char*)tag->str0, TAG_ITEM_STR_LEN, TRUE), 0);
            #else
            func_8086F764_jp(tag, 0, mMl_strlen((char*)tag->str0, TAG_STR0_LEN, CHAR_SPACE), 0);
            #endif
        }
 
        if (func_8086F960_jp(tag) != 0) {
            tag->arrowDir = 1;
        } else {
            tag->arrowDir = 2;
            tag->bodyOfs[0] *= -1.0f;
        }
 
        *(s16*)(*(u8**)((u8*)submenu->overlay + 0x106D0) + 0x11C) = func_8086F644_jp(tag);
    } else if (tag->table == mTG_TABLE_CATALOG_WC) {
        /* Catalog category label (Furniture, Wallpaper, ...) */
        mem_copy(tag->str0, D_80879148_jp[idx], 6);

        #ifdef AFTR
        func_8086F764_jp(tag, 0, aftr_GetStringWidth((char*)tag->str0, TAG_STR0_LEN, TRUE), 0);
        #else
        func_8086F764_jp(tag, 0, mMl_strlen((char*)tag->str0, TAG_STR0_LEN, CHAR_SPACE), 0);
        #endif

        tag->bodyOfs[0] *= -1.0f;
        tag->arrowDir = 2;
        tag->basePos[0] += -10.0f;
        *(s16*)(*(u8**)((u8*)submenu->overlay + 0x106D0) + 0x11C) = func_8086F644_jp(tag);
    } else {
        tag->arrowDir = 0;
    }
}
 

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870334_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087047C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087072C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087080C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870974_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870A1C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870AC4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870B20_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870BA0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870BCC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80870C6C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871570_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808715C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871664_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871708_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871760_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808717BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871894_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808718E0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808719FC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871A60_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871B44_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871C18_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871CF8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871DB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871ECC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80871F74_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087207C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872118_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872580_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808725C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872684_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808726B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872748_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808727E0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087287C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872A34_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872B54_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872DEC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872E60_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80872E84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808731EC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873278_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873348_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873428_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087344C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873498_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873510_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873694_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808736B8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808736DC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873700_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873724_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808737F4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808738C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873968_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808739B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873ADC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873C88_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873F38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80873FB4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874184_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/D_808742A8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874328_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874394_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874444_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874680_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808746A8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808746F8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874720_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874770_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808747D0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808748B0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874B1C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874C8C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874E08_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80874EA4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808750DC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875180_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875214_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087529C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808753A4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875434_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875610_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808757C4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875888_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808759C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875A84_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875AB8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875AD0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875B60_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875B88_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875BF4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875C60_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875CF0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875D38_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875DB0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875E20_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80875EE4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876004_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876204_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808764BC_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876558_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876A94_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876B18_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876C50_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80876D90_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80877290_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808772C8_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_8087792C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80877B0C_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80877EC4_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80877FF4_jp.s")

//#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808782A4_jp.s")

extern u8 D_808794DC_jp[5];
extern u8 D_808794D8_jp[];

extern f32 mFont_SetLineStrings(Game*,u8*,s32,f32,f32,s32,s32,s32,s32,s32,s32,f32,f32,s32);
typedef void (*SetCharMatrixProc)(void*);

//mTG_set_character_item
void func_808782A4_jp(Submenu* submenu, Game* game, void* graph, mTG_tag_c* tag, f32 xOfs, f32 yOfs) {
    f32 scale_rate = tag->scale;
    f32 pos_x;
    f32 pos_y;
    u8* color_p;

    if (fabsf(scale_rate) < 0.008f) return;



    if (tag->table == mTG_TABLE_CATALOG_WC) {
        color_p = D_808794DC_jp;
    } else {
        color_p = D_808794D8_jp;
    }

    pos_x = 160.0f + (tag->basePos[0] + xOfs + scale_rate * (tag->bodyOfs[0] + tag->textOfs[0]));
    pos_y = 120.0f - (tag->basePos[1] + yOfs + scale_rate * (tag->bodyOfs[1] + tag->textOfs[1]));

    ((mTG_SetCharMatrixView*)((u8*)submenu->overlay + 0x10000))->set_char_matrix_proc(graph);
    mFont_SetLineStrings(game, tag->str0, TAG_ITEM_STR_LEN, pos_x, pos_y, color_p[0], color_p[1], color_p[2], 255, 0, 0, scale_rate, scale_rate, 0);
}

//void mTG_pad0(void) {}

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_808783F0_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80878508_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/func_80878648_jp.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/mTG_tag_ovl_construct.s")

#pragma GLOBAL_ASM("asm/jp/nonmatchings/overlays/submenu/tag_ovl/m_tag_ovl/mTG_tag_ovl_destruct.s")
