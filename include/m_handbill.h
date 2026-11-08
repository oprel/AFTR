#ifndef M_HANDBILL_H
#define M_HANDBILL_H

#include "ultra64.h"
#include "m_mail.h"

#define mHandbill_HEADER_LEN MAIL_HEADER_LEN
#define mHandbill_FOOTER_LEN MAIL_FOOTER_LEN
#define mHandbill_BODY_LEN MAIL_BODY_LEN

// Length of one free string.
#define mHandbill_FREE_STR_LEN 10

// Total number of handbill (mail / super / ps) entries.
#define mHandbill_MAIL_NUM 544
// Total number of handbillz entries.
#define TOTAL_HANDBILLZ_DATA_NUMBER 384

enum {
    mHandbill_FREE_STR0,
    mHandbill_FREE_STR1,
    mHandbill_FREE_STR2,
    mHandbill_FREE_STR3,
    mHandbill_FREE_STR4,
    mHandbill_FREE_STR5,
    mHandbill_FREE_STR6,
    mHandbill_FREE_STR7,
    mHandbill_FREE_STR8,
    mHandbill_FREE_STR9,
    mHandbill_FREE_STR10,
    mHandbill_FREE_STR11,
    mHandbill_FREE_STR12,
    mHandbill_FREE_STR13,
    mHandbill_FREE_STR14,
    mHandbill_FREE_STR15,
    mHandbill_FREE_STR16,
    mHandbill_FREE_STR17,
    mHandbill_FREE_STR18,
    mHandbill_FREE_STR19,

    mHandbill_FREE_STR_NUM
};

// fillType of mHandbill_MoveDataCut and friends.
enum {
    mHandbill_FILL_NONE,   // leave the freed tail untouched
    mHandbill_FILL_RETURN, // fill the freed tail with the newline code
    mHandbill_FILL_SPACE,  // fill the freed tail with the space code

    mHandbill_FILL_NUM
};

typedef struct handbill_data_s {
    /* 0x00 */ u8 freeStr[mHandbill_FREE_STR_NUM][mHandbill_FREE_STR_LEN];
} mHandbill_Data_c; // size = 0xC8

enum {
    mHandbillz_TYPE_SUPER,
    mHandbillz_TYPE_MAILA,
    mHandbillz_TYPE_MAILB,
    mHandbillz_TYPE_MAILC,
    mHandbillz_TYPE_PS,

    mHandbillz_TYPE_NUM
};

// Handbillz load info. The buffer sizes must be large enough for the string that gets loaded into them.
// headerBackStart is an output.
typedef struct mHandbillz_info_s {
    /* 0x00 */ u8* superBufP;
    /* 0x04 */ size_t superBufSize;
    /* 0x08 */ u8* mailBufP;
    /* 0x0C */ size_t mailBufSize;
    /* 0x10 */ u8* psBufP;
    /* 0x14 */ size_t psBufSize;
    /* 0x18 */ s32 superNo;
    /* 0x1C */ s32 mailaNo;
    /* 0x20 */ s32 mailbNo;
    /* 0x24 */ s32 mailcNo;
    /* 0x28 */ s32 psNo;
    /* 0x2C */ s32 headerBackStart;
} mHandbillz_Info_c; // size = 0x30

// Sets free string strNum to the first strLen codes of str.
void mHandbill_Set_free_str(s32 strNum, char* str, s32 strLen);
void mHandbill_CopyString(u8* dst, u8* src, s32 len);
s32 mHandbill_MoveDataCut(u8* data, s32 bufSize, s32 dstIdx, s32 srcIdx, s32 dataLen, s32 fillType);
s32 mHandbill_Put_String_FREE(u8* str, s32 bufSize, s32 startIdx, s32 strLen, s32 strNo, s32 fillType);
s32 mHandbill_Put_String_FREE0(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE1(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE2(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE3(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE4(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE5(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE6(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE7(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE8(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE9(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE10(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE11(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE12(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE13(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE14(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE15(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE16(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE17(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE18(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String_FREE19(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
s32 mHandbill_Put_String(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType);
void mHandbill_Change_ControlCode(u8* buf, s32 bufSize, s32 strLen, s32 fillType);
void mHandbill_Change_ControlCode2(u8* buf, s32 bufSize, s32 strLen, s32* headerBackStart, s32 fillType);
void mHandbill_Get_SuperStringDataAddressAndSize(s32 superNo, u32* addr, u32* size);
void mHandbill_Get_PsStringDataAddressAndSize(s32 psNo, u32* addr, u32* size);
void mHandbill_Get_MailStringDataAddressAndSize(s32 mailNo, u32* addr, u32* size);
void mHandbill_CheckSuperStringBorderAndCopy(u8* dst, s32 dstSize, s32* headerBackStart, u8* src, s32 srcSize);
void mHandbill_Load_SuperStringFromRom(u8* buf, s32 bufSize, s32* headerBackStart, s32 superNo);
void mHandbill_Load_PsStringFromRom(u8* buf, s32 bufSize, s32 psNo);
void mHandbill_Load_MailFromRom(u8* buf, s32 mailNo);
void mHandbill_Load_HandbillFromRom(u8* header, s32* headerBackStart, u8* footer, u8* body, s32 mailNo);
void mHandbill_Load_HandbillFromRom2(u8* header, s32 headerSize, s32* headerBackStart, u8* footer,
                                     s32 footerSize, u8* body, s32 mailNo);
// Handbillz functions below are only used by mHandbillz_load.
// s32 mHandbillzDMA_body_addr_set(mHandbillzDMA_c* dmaInfo);
// s32 mHandbillzDMA_body_load(mHandbillzDMA_c* dmaInfo);
s32 mHandbillz_super_load(mHandbillz_Info_c* info);
s32 mHandbillz_mail_load(mHandbillz_Info_c* info);
s32 mHandbillz_ps_load(mHandbillz_Info_c* info);
// Returns TRUE on success, FALSE on failure.
s32 mHandbillz_load(mHandbillz_Info_c* info);

#endif
