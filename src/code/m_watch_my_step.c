#include "global.h"
#include "m_common_data.h"
#include "m_watch_my_step.h"
#include "m_event.h"
#include "m_field_info.h"
#include "m_player_lib.h"
#include "m_scene_table.h"
#include "src/overlays/gamestates/ovl_play/m_play.h"
#include "src/overlays/actors/player_actor/m_player.h"
#include "6B3DC0.h"

#define CHAR_SPACE 32

enum {
    mSP_TANUKI_SHOP_STATUS_NORMAL,
    mSP_TANUKI_SHOP_STATUS_EVENT,
    mSP_TANUKI_SHOP_STATUS_HALLOWEEN,
    mSP_TANUKI_SHOP_STATUS_FUKUBIKI,
    mSP_TANUKI_SHOP_STATUS_HUKUBUKURO_SALE,
    mSP_TANUKI_SHOP_STATUS_NUM
};

static mWt_watch_my_step_c S_watch_my_step;
static mWt_navigate_c S_navigate;
static mWt_mybell_confirmation_c S_mybell_conf;


extern s32 D_8010EF10_jp[];
extern s32 S_se_play_flg;
extern u32 D_8010EF30_jp[6];
extern u16 D_8010EF48_jp[];
extern u32 D_8010EF50_jp[];

extern Gfx D_400C3D8[];
extern Gfx D_400C250[];
extern Gfx D_400C358[];
extern Gfx D_400C2D8[];
extern Gfx D_400C1D0[];
extern Gfx D_4000220[];
extern Gfx D_4000300[];
extern Gfx D_4000470[];
extern Gfx D_400B360[];
extern Gfx D_400B3E0[];
extern Gfx D_400B458[];

extern u16 func_800B5C60_jp(void);

extern s32 mPr_GetPossessionItemSumWithCond(PrivateInfo* priv, u16 item, u32 cond);
extern void mFont_SetLineStrings(Game_Play* play, u8* str, s32 len, f32 x, f32 y,
                              s32 r, s32 g, s32 b, s32 a, s32 unk1, s32 unk2,
                              f32 scaleX, f32 scaleY, s32 mode);

#define GRAPH_ALLOC_TYPE(graph, type, num) (GRAPH_ALLOC(graph, sizeof(type) * (num)))

void func_800CBF80_jp(void) {
    bzero(&S_watch_my_step, sizeof(mWt_watch_my_step_c));
    S_watch_my_step.item_no = 0;
    func_800CC9C4_jp();
    func_800CCE08_jp();
}

void func_800CC9C4_jp(void) {
    bzero(&S_navigate, sizeof(mWt_navigate_c));
}

void func_800CC9EC_jp(Game_Play* play) {
    S_navigate.draw_type = 0;

    switch (S_navigate.mode) {
        case 0:
            if (func_800B5D34_jp() != 0 && *(u8*)((u8*)play + 0x1EE0) == 0) {
                S_navigate.timer = 0x4B;
                S_navigate.mode++;
            }
            break;

        case 1:
            add_calc(&S_navigate.opacity, 1.0f, 0.2f, 0.15f, 0.01f);
            S_navigate.timer--;
            if (S_navigate.timer == 0 || play->submenu.moveProcIndex != 0) {
                S_navigate.mode++;
            }
            S_navigate.draw_type = 1;
            break;

        case 2:
            add_calc(&S_navigate.opacity, 0.0f, 0.2f, 0.1f, 0.01f);
            if (S_navigate.opacity < 0.0001f) {
                S_navigate.mode++;
            } else {
                S_navigate.draw_type = 1;
            }
            break;

        case 3:
        default:
            break;
    }
}

void func_800CCB44_jp(Game_Play* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    Mtx* font_mtx = GRAPH_ALLOC_TYPE(gfxCtx, Mtx, 1);
    Gfx* gfx;
    u8 a;
    s32 pad[2];

    if (S_navigate.draw_type == 0) {
        return;
    }

    
    OPEN_DISPS(gfxCtx);

    a = S_navigate.opacity * 255.0f;
    if (font_mtx != NULL) {
        guOrtho(font_mtx, -2560.0f, 2560.0f, -1920.0f, 1920.0f,
                1.0f, 2000.0f, 1.0f);
        gSPMatrix(FONT_DISP++, font_mtx,
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    }

    Matrix_scale(16.0f, 16.0f, 16.0f, 0);

    gfx = FONT_DISP;
    gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, D_400C3D8);
    gDPPipeSync(gfx++);

    gDPSetPrimColor(gfx++, 0, a, 40, 40, 140, a);
    gSPDisplayList(gfx++, D_4000220);
    gDPPipeSync(gfx++);

    gDPSetPrimColor(gfx++, 0, a, 255, 255, 30, a);
    gDPSetEnvColor(gfx++, 95, 55, 55, a);
    gSPDisplayList(gfx++, D_4000300);
    gDPPipeSync(gfx++);

    gDPSetPrimColor(gfx++, 0, a, 255, 255, 255, a);
    gDPSetEnvColor(gfx++, 155, 30, 30, a);
    gSPDisplayList(gfx++, D_4000470);
    FONT_DISP = gfx;

    CLOSE_DISPS(gfxCtx);
}

void func_800CCE08_jp(void) {
    bzero(&S_mybell_conf, sizeof(mWt_mybell_confirmation_c));
    S_mybell_conf.all_money = func_800CD59C_jp();
}

void func_800CCE40_jp(Game_Play* play) {
    s32 i;
    s32 pad;
    f32 money;
    u32 now_money;

    S_mybell_conf.draw_type = 0;

    if (S_mybell_conf.coin_sfx_timer != 0) {
        if (S_mybell_conf.coin_sfx_timer == 1) {
            sAdo_SysLevStop(0x42);
        }
        S_mybell_conf.coin_sfx_timer--;
    }

    for (i = 0; i < 7; i++) {
        if (SAVE_GET(sceneNo) == D_8010EF10_jp[i]) {
            break;
        }
    }

    if (i == 7) {
        return;
    }

    if (mEv_CheckFirstJob() == TRUE) {
        return;
    }

    if (i < 5 && COMMON_GET(tanuki_shop_status) == mSP_TANUKI_SHOP_STATUS_FUKUBIKI) {
        return;
    }

    switch (S_mybell_conf.mode) {
        case 0:
            if (play->submenu.moveProcIndex == 0) {
                S_mybell_conf.mode++;
            }
            break;

        case 1:
            money = S_mybell_conf.all_money;
            now_money = func_800CD59C_jp();

            if (now_money != S_mybell_conf.all_money) {
                if (S_se_play_flg == 0) {
                    S_se_play_flg = 1;
                    S_mybell_conf.coin_sfx_timer = 0x96;
                    sAdo_SysLevStart(0x42);
                }
            }

            if (play->submenu.moveProcIndex != 0) {
                S_mybell_conf.mode++;
            }

            if ((now_money == S_mybell_conf.all_money ||
                 S_mybell_conf.mode == 2) &&
                S_se_play_flg == 1) {
                S_se_play_flg = 0;
                sAdo_SysLevStop(0x42);
                S_mybell_conf.coin_sfx_timer = 0;
                if (now_money == S_mybell_conf.all_money) {
                    sAdo_SysTrgStart(0x1050);
                }
            }

            add_calc(&money, now_money, 0.2f, 10000.0f, 1.0f);
            S_mybell_conf.all_money = money;
            add_calc(&S_mybell_conf.opacity, 1.0f, 0.2f, 0.15f, .01f);
            S_mybell_conf.draw_type = 1;
            break;
        case 2:
            add_calc(&S_mybell_conf.opacity, 0.0f, 0.2f, 0.1f, 0.01f);
            if (S_mybell_conf.opacity < 0.0001f) {
                S_mybell_conf.mode = 0;
            } else {
                S_mybell_conf.draw_type = 1;
            }
            break;
        case 3:
        default:
            break;
    }
}

void func_800CD194_jp(Game_Play* play) {
    s32 pad;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    Mtx* font_mtx = GRAPH_ALLOC_TYPE(gfxCtx, Mtx, 1);
    Gfx* gfx;
    u8 a;
    u8 bell_str[6];
    s32 pad2[2];

    if (S_mybell_conf.draw_type == 0) {
        return;
    }

    OPEN_DISPS(gfxCtx);

    a = (u8)(S_mybell_conf.opacity * 255.0f);


    if (font_mtx != NULL) {
        guOrtho(font_mtx, -2560.0f, 2560.0f, -1920.0f, 1920.0f,
                1.0f, 2000.0f, 1.0f);
        gSPMatrix(FONT_DISP++, font_mtx,
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    }

    Matrix_scale(16.0f, 16.0f, 16.0f, 0);

    gfx = FONT_DISP;
    gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, D_400C3D8);

    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_CLD_SURF2);
    gDPSetPrimColor(gfx++, 0, a, 95, 50, 175, a);
    gSPDisplayList(gfx++, D_400B360);

    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gfx++, 0, a, 255, 245, 255, a);
    gSPDisplayList(gfx++, D_400B3E0);

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, a, 255, 245, 255, a);
    gSPDisplayList(gfx++, D_400B458);
    gDPPipeSync(gfx++);

    Matrix_scale(1.0f, 1.0f, 1.0f, 0);
    gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    FONT_DISP = gfx;

    CLOSE_DISPS(gfxCtx);

    {
        u32 value;
        u8 fill;
        u32 digit;
        s32 i;
        value = S_mybell_conf.all_money;
        fill = ' ';
    
        for (i = 0; i < 6; i++) {
            digit = value / D_8010EF30_jp[i];
            if (digit != 0) {
                bell_str[i] = digit + '0';
                fill = '0';
            } else if (i == 5) {
                bell_str[i] = '0';
            } else {
                bell_str[i] = fill;
            }
    
            value -= digit * D_8010EF30_jp[i];
        }
    
        bell_str[i] = '\0';
    
        mFont_SetLineStrings(play, bell_str, 6, 214.0f, 48.0f,
                         255, 245, 0, (s32)(a), 0, 0,
                         0.75f, 0.75f, 1);
    }
}

u32 func_800CD59C_jp(void) {
    CommonData* common = &common_data;
    u32 money = common_data.privateInfo->inventory.wallet;
    s32 i;

    for (i = 0; i < 4; i++) {
        money += mPr_GetPossessionItemSumWithCond(common_data.privateInfo, D_8010EF48_jp[i], 0) * D_8010EF50_jp[i];
    }

    return money;
}

// New File ?

void watch_my_step_draw(Game_Play* play) {
    s32 pad;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    Mtx* font_mtx = GRAPH_ALLOC_TYPE(gfxCtx, Mtx, 1);
    Gfx* gfx;
    u8 a;
    s32 pad2[2];

    func_800CCB44_jp(play);
    func_800CD194_jp(play);

    if (S_watch_my_step.draw_type == 0) {
        return;
    }

    OPEN_DISPS(gfxCtx);

    if (font_mtx != NULL) {
        guOrtho(font_mtx, -2560.0f, 2560.0f, -1920.0f, 1920.0f,
                1.0f, 2000.0f, 1.0f);
        gSPMatrix(FONT_DISP++, font_mtx,
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    }
    Matrix_scale(16.0f, 16.0f, 16.0f, 0);
    Matrix_translate(S_watch_my_step.pos_x, -S_watch_my_step.pos_y, 0.0f, 1);

    gfx = FONT_DISP;

    gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, D_400C3D8);
    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_CLD_SURF2);

    switch (S_watch_my_step.mode) {
        case 3:
            Matrix_push();
            Matrix_scale(S_watch_my_step.opacity *
                             (S_watch_my_step.scale * 0.6666669846f + 0.3333329856f),
                         S_watch_my_step.opacity,
                         S_watch_my_step.opacity, 1);
            gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_pull();
            gSPDisplayList(gfx++, D_400C250);
            /* fallthrough */

        case 2:
            Matrix_push();
            Matrix_translate(S_watch_my_step.trans_x * -1.0f,
                             S_watch_my_step.trans_y * -20.0f, 0.0f, 1);
            gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_pull();
            gSPDisplayList(gfx++, D_400C358);
            /* fallthrough */

        case 1:
            Matrix_push();
            Matrix_translate(S_watch_my_step.trans_x * -13.0f,
                             S_watch_my_step.trans_y * -30.0f, 0.0f, 1);
            gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_pull();
            gSPDisplayList(gfx++, D_400C2D8);
            break;

        case 4:
            a = (u8)(S_watch_my_step.opacity * 255.0f);

            gDPPipeSync(gfx++);
            gDPSetPrimColor(gfx++, 0, a, 255, 255, 215, a);

            Matrix_push();
            Matrix_scale((S_watch_my_step.scale * 0.666667f + 0.333333f),
                         1.0f, 1.0f, 1);
            gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_pull();
            gSPDisplayList(gfx++, D_400C1D0);
            break;
    }

    Matrix_scale(1.0f, 1.0f, 1.0f, 0);
    gSPMatrix(gfx++, _Matrix_to_Mtx_new(gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    FONT_DISP = gfx;

    CLOSE_DISPS(gfxCtx);

    if (S_watch_my_step.mode >= 3) {
        f32 text_opacity = (S_watch_my_step.opacity - 0.5f) * 2.0f;

        if (text_opacity > 0.0f) {
            mFont_SetLineStrings(
                play, S_watch_my_step.item_name, ITEM_NAME_LEN,
                (S_watch_my_step.pos_x + 107.0f) +
                    (1.0f - S_watch_my_step.scale) * 43.0f,
                S_watch_my_step.pos_y + 113.0f,
                45, 45, 35, (s32)(255.0f * text_opacity),
                0, 0, 0.875f, 0.875f, 1);
        }
    }
}

void watch_my_step_move(Game_Play* play) {
    Actor* player_actor = (Actor*)get_player_actor_withoutCheck(play);
    xyz_t position;
    xyz_t screen_pos;
    u16 window_item;
    s32 pad;
    s32 can_show = FALSE;
    s32 pad2;

    if (play->camera.now_main_index == 1 &&
        play->camera.unk84 >= 0x15) {
        can_show = TRUE;
    }

    func_800CC9EC_jp(play);
    func_800CCE40_jp(play);

    S_watch_my_step.draw_type = 0;

    if (mEv_CheckTitleDemo() > 0) {
        return;
    }

    window_item = func_800B5C60_jp();

    switch (S_watch_my_step.mode) {
        case 0:
            if (window_item != 0 && !can_show) {
                S_watch_my_step.timer = 1;
                S_watch_my_step.mode++;
                S_watch_my_step.opacity = 0.0f;
            }
            break;

        case 1:
        case 2:
            if (S_watch_my_step.timer-- == 0) {
                S_watch_my_step.timer = 1;
                S_watch_my_step.mode++;
            }
            break;

        case 3:
            add_calc(&S_watch_my_step.opacity, 1.0f, 0.5f, 0.5f, 0.15f);
            if (window_item == 0 || can_show == TRUE) {
                S_watch_my_step.mode++;
            }
            break;

        case 4:
            add_calc(&S_watch_my_step.opacity, 0.0f, 0.5f, 0.2f, 0.1f);
            add_calc(&S_watch_my_step.opacity, 0.0f, 0.5f, 0.01f, 0.01f);
            if (S_watch_my_step.opacity < 0.01f) {
                S_watch_my_step.mode = 0;
            }
            break;

        default:
            S_watch_my_step.mode = 0;
            break;
    }

    if (S_watch_my_step.mode != 0) {

        position = player_actor->world.pos;
        position.y += 30.0f;
        Game_play_Projection_Trans(play, &position, &screen_pos);

        if (S_watch_my_step.mode < 4) {
            S_watch_my_step.trans_x = 1.0f;
            if (screen_pos.x > 200.0f) {
                S_watch_my_step.trans_x = -1.0f;
            } else if (screen_pos.x > 120.0f &&
                       *(s16*)((u8*)player_actor + 0xDE) >= 0) {
                S_watch_my_step.trans_x = -1.0f;
            }

            S_watch_my_step.trans_y = 1.0f;
            if (screen_pos.y < 68.0f) {
                S_watch_my_step.trans_y = -1.0f;
            }

            S_watch_my_step.pos_x =
                screen_pos.x - (-40.0f * S_watch_my_step.trans_x + 160.0f);
            S_watch_my_step.pos_y =
                screen_pos.y - (42.0f * S_watch_my_step.trans_y + 120.0f);
        }

        if (window_item != 0) {
            s32 len;

            if (S_watch_my_step.item_no != window_item ||
                S_watch_my_step.item_no == 0) {
                S_watch_my_step.item_no = window_item;
                mIN_copy_name_str(S_watch_my_step.item_name, window_item);
            }

        
#if AFTR
        len = aftr_GetStringWidth(S_watch_my_step.item_name, ITEM_NAME_LEN, TRUE);
#else
        len = mMl_strlen(S_watch_my_step.item_name, ITEM_NAME_LEN, CHAR_SPACE);
#endif
        S_watch_my_step.scale = ((f32)len - 2.0f) / 8;

        } else {
            S_watch_my_step.item_no = 0;
        }

        if (S_watch_my_step.mode == 0) {
            return;
        }

        if (((mFI_GetFieldId() & 0xF000) == 0 &&
            mEv_CheckFirstIntro() != TRUE) || (COMMON_GET(field_type) == 1 &&
                   !(window_item >= 0x1000 && window_item < 0x1ECD))) {
            S_watch_my_step.draw_type = 1;
        }
    }
}
