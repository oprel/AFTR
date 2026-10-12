#include "m_item_name.h"
#include "m_lib.h"
#include "m_room_type.h"
#include "m_std_dma.h"

#define ITEM_NAME_SEG 0x06000000

// The vanilla item bank has an 8 byte header followed by the categories. Every entry is
// ITEM_VANILLA_LEN bytes, but some categories end with a couple of padding bytes, so the
// vanilla category bases (D_801076BC_jp, D_6001D98) are not a flat grid.
#define ITEM_HEADER_LEN 8
#define ITEM_VANILLA_LEN 10

// translation_injector.py lays the translated bank out as a flat grid of ITEM_NAME_LEN sized
// slots, indexed by af_index: slot n is at ITEM_HEADER_LEN + n * ITEM_NAME_LEN. A category
// starts at the slot nearest to its vanilla position, (base - header) / ITEM_VANILLA_LEN rounded.
#define ITEM_BASE_IDX(base) (((base) - ITEM_NAME_SEG - ITEM_HEADER_LEN + ITEM_VANILLA_LEN / 2) / ITEM_VANILLA_LEN)

#if ITEM_NAME_LEN == ITEM_VANILLA_LEN
#define ITEM_REBASE(base) (base)
#define ITEM_NAME_BUF(n) u8 n[ITEM_NAME_LEN]
#define ITEM_NAME_PTR(n) (n)
#else
#define ITEM_REBASE(base) (ITEM_NAME_SEG + ITEM_HEADER_LEN + ITEM_BASE_IDX(base) * ITEM_NAME_LEN)
#define ITEM_NAME_BUF(n) union { u8 b[ITEM_NAME_LEN]; f64 align; } n
#define ITEM_NAME_PTR(n) ((n).b)
#endif


extern u8 D_6000000[];
extern u8 D_10F4000[];
extern u16 D_6001D98[];
extern u8* D_801076BC_jp[16];

extern u16 mRmTp_FtrItemNo2Item1ItemNo(u16 item);

void func_80096710(char* dst, RomOffset src) {
    DmaMgr_RequestSyncDebug(dst, src, ITEM_NAME_LEN, "../m_item_name.c", 0x55);
}

// func_80096710 + mIN_copy_name_str must stay 0x150 bytes, or everything after them in the
// code segment moves and the game stops booting. The GLOBAL_ASM block at the end pads it.
void mIN_copy_name_str(char* buf, u32 item) {
    u32 itm = item & 0xffff;
    ITEM_NAME_BUF(name);
    u16 item_no = mRmTp_FtrItemNo2Item1ItemNo(item);
    s32 type = (item_no & 0xF000) >> 12;
    RomOffset base;
    u32 idx;

    switch (type) {
        case 2:
            base = (RomOffset)D_801076BC_jp[(item_no & 0xF00) >> 8];
            idx = item_no & 0xFF;
            break;

        case 1:
            base = (RomOffset)D_6001D98;
            idx = item_no & 0xFFF;
            break;

        default:
            if (item_no == 0) {
                // the vanilla blank name (D_801076B0_jp) is only ITEM_VANILLA_LEN bytes long
                mem_clear((u8*)buf, ITEM_NAME_LEN, ' ');
            }
            return;
    }

    base = ITEM_REBASE(base) + idx * ITEM_NAME_LEN;
    func_80096710(ITEM_NAME_PTR(name), (base - (RomOffset)D_6000000) + (RomOffset)D_10F4000);
    mem_copy(buf, ITEM_NAME_PTR(name), ITEM_NAME_LEN);
}

//doesn't match original 100%, so we need the padding
void mIN_copy_name_str_pad0(void) {}
void mIN_copy_name_str_pad1(void) {}
void mIN_copy_name_str_pad2(void) {}
void mIN_copy_name_str_pad3(void) {}
void mIN_copy_name_str_pad4(void) {}
