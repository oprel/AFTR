#include "m_handbill.h"
#include "global.h"

#include "m_font.h"
#include "m_lib.h"
#include "m_msg_main.h"
#include "m_std_dma.h"

// DMA scratch types (8 byte aligned, sizes taken from the stack layouts).
typedef union {
    u64 align;
    u32 data[4]; // 2 or 3 table entries are read
} mHandbill_dma_super_table_dummy;

typedef union {
    u64 align;
    u32 data[4];
} mHandbill_dma_ps_table_dummy;

typedef union {
    u64 align;
    u32 data[4];
} mHandbill_dma_mail_table_dummy;

typedef union {
    u64 align;
    u8 data[13 + 16]; // biggest super string + alignment slack
} mHandbill_dma_super_string_dummy;

typedef union {
    u64 align;
    u8 data[18 + 16];
} mHandbill_dma_ps_string_dummy;

typedef union {
    u64 align;
    u8 data[104 + 16];
} mHandbill_dma_mail_string_dummy;

typedef s32 (*mHandbill_PUT_STRING_PROC)(u8*, s32, s32, s32, s32);

typedef struct mHandbillzDMA_s {
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 num;
    /* 0x08 */ u8* ramBuf;
    /* 0x0C */ size_t ramBufSize;
    /* 0x10 */ u32 dmaAddr;
    /* 0x14 */ u32 dmaSize;
} mHandbillzDMA_c; // size = 0x18

typedef union {
    u64 align;
    u32 data[2];
} mHandbillzIdxDMASTR;

typedef union {
    u64 align;
    u8 data[13 + 16];
} mHandbillzSuperDMASTR;

typedef union {
    u64 align;
    u8 data[18 + 16];
} mHandbillzPsDMASTR;

// ROM addresses
extern u8 D_D07000[];  // mail body data
extern u8 D_D10000[];  // mail body table
extern u8 D_D11000[];  // super string data
extern u8 D_D12000[];  // super string table
extern u8 D_D13000[];  // ps string data
extern u8 D_D15000[];  // ps string table
extern u8 D_D1A000[];  // handbillz archive
extern u8 D_6000000[]; // segment 6 base

// Original statics.
extern mHandbill_PUT_STRING_PROC mHandbill_proc[mFont_CONT_CODE_NUM];
extern u32 mHandbillz_dummy_size_tbl[mHandbillz_TYPE_NUM]; // { 13, 104, 104, 104, 18 }
extern u32 mHandbillz_table_pos[mHandbillz_TYPE_NUM];      // segment 6 address of the handbillz tables
extern u32 mHandbillz_data_pos[mHandbillz_TYPE_NUM];       // segment 6 address of the handbillz data
extern mHandbill_Data_c mHandbill_data;
extern mHandbill_dma_mail_string_dummy mHandbill_mail_buff;

extern s32 mFont_CodeSize_idx_get(u8* dataP, s32 index);

void mHandbill_Set_free_str(s32 strNum, char* str, s32 strLen) {
    mHandbill_Data_c* handbillDataP;
    s32 i;
    u8* freeStrP;

    if (strNum < 0 || strNum >= mHandbill_FREE_STR_NUM) {
        return;
    }

    if (str == NULL) {
        return;
    }

    handbillDataP = &mHandbill_data;
    freeStrP = handbillDataP->freeStr[strNum];

    if (strLen > mHandbill_FREE_STR_LEN) {
        strLen = mHandbill_FREE_STR_LEN;
    }

    for (i = 0; i < strLen; i++) {
        freeStrP[i] = str[i];
    }

    for (; i < mHandbill_FREE_STR_LEN; i++) {
        freeStrP[i] = CHAR_SPACE;
    }
}

void mHandbill_CopyString(u8* dst, u8* src, s32 len) {
    s32 i;

    for (i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}

s32 mHandbill_MoveDataCut(u8* data, s32 bufSize, s32 dstIdx, s32 srcIdx, s32 dataLen, s32 fillType) {
    s32 newLen = dataLen;
    u8 fillChar;
    s32 moveSize;
    s32 addTotalData;
    s32 cutTotalData;
    s32 i;
    u8* dst;
    u8* src;

    if (dstIdx < srcIdx) {
        /* pull the data forwards */
        while (srcIdx < dataLen) {
            data[dstIdx] = data[srcIdx];
            dstIdx++;
            srcIdx++;
        }

        newLen -= srcIdx - dstIdx;

        if (fillType != mHandbill_FILL_NONE) {
            if (fillType == mHandbill_FILL_RETURN) {
                fillChar = CHAR_NEW_LINE;
            } else {
                fillChar = CHAR_SPACE;
            }

            while (dstIdx < dataLen) {
                data[dstIdx] = fillChar;
                dstIdx++;
            }
        }
    } else if (dstIdx > srcIdx) {
        /* push the data backwards */
        moveSize = dataLen - srcIdx;
        addTotalData = dstIdx - srcIdx;
        newLen += addTotalData;

        if (newLen > bufSize) {
            cutTotalData = newLen - bufSize;
            dataLen -= cutTotalData;
            moveSize -= cutTotalData;
            newLen = bufSize;
        }

        dst = data + newLen - 1;
        src = data + dataLen - 1;
        for (i = 0; i < moveSize; i++) {
            *dst-- = *src--;
        }
    }

    return newLen;
}

s32 mHandbill_Put_String_FREE(u8* str, s32 bufSize, s32 startIdx, s32 strLen, s32 strNo, s32 fillType) {
    s32 codeSize;
    mHandbill_Data_c* handbillDataP;
    u8* stringP;
    s32 freeStrLen;
    s32 cutLen;

    if (strNo < mHandbill_FREE_STR0 || strNo >= mHandbill_FREE_STR_NUM) {
        strNo = mHandbill_FREE_STR0;
    }

    codeSize = mFont_CodeSize_idx_get(str, startIdx);
    handbillDataP = &mHandbill_data;
    stringP = handbillDataP->freeStr[strNo];
    freeStrLen = mMsg_Get_Length_String((char*)stringP, mHandbill_FREE_STR_LEN);

    cutLen =
        mHandbill_MoveDataCut(str, bufSize, startIdx + freeStrLen, startIdx + codeSize, strLen, fillType);

    if (cutLen >= bufSize && freeStrLen > bufSize - startIdx) {
        freeStrLen = bufSize - startIdx;
    }

    mHandbill_CopyString(str + startIdx, stringP, freeStrLen);

    return cutLen;
}

s32 mHandbill_Put_String_FREE0(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR0, fillType);
}

s32 mHandbill_Put_String_FREE1(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR1, fillType);
}

s32 mHandbill_Put_String_FREE2(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR2, fillType);
}

s32 mHandbill_Put_String_FREE3(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR3, fillType);
}

s32 mHandbill_Put_String_FREE4(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR4, fillType);
}

s32 mHandbill_Put_String_FREE5(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR5, fillType);
}

s32 mHandbill_Put_String_FREE6(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR6, fillType);
}

s32 mHandbill_Put_String_FREE7(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR7, fillType);
}

s32 mHandbill_Put_String_FREE8(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR8, fillType);
}

s32 mHandbill_Put_String_FREE9(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR9, fillType);
}

s32 mHandbill_Put_String_FREE10(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR10, fillType);
}

s32 mHandbill_Put_String_FREE11(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR11, fillType);
}

s32 mHandbill_Put_String_FREE12(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR12, fillType);
}

s32 mHandbill_Put_String_FREE13(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR13, fillType);
}

s32 mHandbill_Put_String_FREE14(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR14, fillType);
}

s32 mHandbill_Put_String_FREE15(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR15, fillType);
}

s32 mHandbill_Put_String_FREE16(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR16, fillType);
}

s32 mHandbill_Put_String_FREE17(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR17, fillType);
}

s32 mHandbill_Put_String_FREE18(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR18, fillType);
}

s32 mHandbill_Put_String_FREE19(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    return mHandbill_Put_String_FREE(buf, bufSize, startIdx, strLen, mHandbill_FREE_STR19, fillType);
}

s32 mHandbill_Put_String(u8* buf, s32 bufSize, s32 startIdx, s32 strLen, s32 fillType) {
    s32 type;

    type = buf[startIdx + 1];

    if (type >= mFont_CONT_CODE_BEGIN && type < mFont_CONT_CODE_NUM) {
        mHandbill_PUT_STRING_PROC p = mHandbill_proc[type];

        if (p != NULL) {
            return p(buf, bufSize, startIdx, strLen, fillType);
        }
    }

    return strLen;
}

void mHandbill_Change_ControlCode(u8* buf, s32 bufSize, s32 strLen, s32 fillType) {
    s32 len;
    s32 pos;
    u8 code;

    len = strLen;
    pos = 0;

    while (pos < len) {
        code = buf[pos];

        if (code == CHAR_CONTROL_CODE) {
            len = mHandbill_Put_String(buf, bufSize, pos, len, fillType);
        } else {
            pos++;
        }
    }
}

void mHandbill_Change_ControlCode2(u8* buf, s32 bufSize, s32 strLen, s32* headerBackStart, s32 fillType) {
    s32 len;
    s32 nowStrLen;
    s32 pos;
    u8 code;

    len = strLen;
    pos = 0;

    while (pos < len) {
        code = buf[pos];

        if (code == CHAR_CONTROL_CODE) {
            nowStrLen = len;
            len = mHandbill_Put_String(buf, bufSize, pos, len, fillType);

            /* the tail position moves along when the replacement is in front of it */
            if (pos < *headerBackStart) {
                *headerBackStart += len - nowStrLen;
            }
        } else {
            pos++;
        }
    }
}

void mHandbill_Get_SuperStringDataAddressAndSize(s32 superNo, u32* addr, u32* size) {
    s32 offset;
    s32 strSize;

    if (superNo >= 0 && superNo < mHandbill_MAIL_NUM) {
        mHandbill_dma_super_table_dummy ram;
        UNUSED u32 unused;

        /* the table holds the end offset of every string, reads are 8 byte aligned */
        if (superNo & 1) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D12000 + (superNo * 4 - 4), 8, "m_handbill.c", 827);
            offset = ram.data[0];
            strSize = ram.data[1] - ram.data[0];
        } else if (superNo == 0) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D12000, 8, "m_handbill.c", 834);
            offset = 0;
            strSize = ram.data[0];

        } else {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D12000 + (superNo * 4 - 8), 16, "m_handbill.c", 840);
            offset = ram.data[1];
            strSize = ram.data[2] - ram.data[1];
        }

        if (strSize < 14) {
            *addr = (u32)D_D11000 + offset;
            *size = strSize;
        } else {
            *size = 0;
            *addr = 0;
        }
    } else {
        *addr = 0;
        *size = 0;
    }
}

void mHandbill_Get_PsStringDataAddressAndSize(s32 psNo, u32* addr, u32* size) {
    s32 offset;
    s32 strSize;

    if (psNo >= 0 && psNo < mHandbill_MAIL_NUM) {
        mHandbill_dma_super_table_dummy ram;
        UNUSED u32 unused;

        /* the table holds the end offset of every string, reads are 8 byte aligned */
        if (psNo & 1) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D15000 + (psNo * 4 - 4), 8, "m_handbill.c", 895);
            offset = ram.data[0];
            strSize = ram.data[1] - ram.data[0];
        } else if (psNo == 0) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D15000, 8, "m_handbill.c", 902);
            offset = 0;
            strSize = ram.data[0];
        } else {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D15000 + (psNo * 4 - 8), 16, "m_handbill.c", 908);
            offset = ram.data[1];
            strSize = ram.data[2] - ram.data[1];
        }

        if (strSize < 19) {
            *addr = (u32)D_D13000 + offset;
            *size = strSize;
        } else {
            *addr = 0;
            *size = 0;
        }
    } else {
        *addr = 0;
        *size = 0;
    }
}

void mHandbill_Get_MailStringDataAddressAndSize(s32 mailNo, u32* addr, u32* size) {
    s32 offset;
    s32 strSize;

    if (mailNo >= 0 && mailNo < mHandbill_MAIL_NUM) {
        mHandbill_dma_super_table_dummy ram;
        UNUSED u32 unused;

        /* the table holds the end offset of every string, reads are 8 byte aligned */
        if (mailNo & 1) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D10000 + (mailNo * 4 - 4), 8, "m_handbill.c", 963);
            offset = ram.data[0];
            strSize = ram.data[1] - ram.data[0];
        } else if (mailNo == 0) {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D10000, 8, "m_handbill.c", 970);
            offset = 0;
        } else {
            DmaMgr_RequestSyncDebug(&ram, (u32)D_D10000 + (mailNo * 4 - 8), 16, "m_handbill.c", 976);
            offset = ram.data[1];
            strSize = ram.data[2] - ram.data[1];
        }

        if (strSize < 105) {
            *addr = (u32)D_D07000 + offset;
            *size = strSize;
        } else {
            *addr = 0;
            *size = 0;
        }
    } else {
        *addr = 0;
        *size = 0;
    }
}

/*
 * Copies srcP into domainP, dropping the newline code. The (single) newline
 * position is the border between the two lines of a super string and is stored
 * in *tailIndexP. If there isn't exactly one newline, *tailIndexP = totalSrcCode.
 */
void mHandbill_CheckSuperStringBorderAndCopy(u8* dst, s32 dstSize, s32* headerBackStart, u8* src, s32 srcSize) {
    s32 i;
    s32 lines;
    s32 dstPos;

    lines = 0;
    dstPos = 0;

    for (i = 0; i < srcSize; i++) {
        if (*src == CHAR_NEW_LINE) {
            lines++;
            *headerBackStart = i;
        } else if (dstPos < dstSize) {
            *dst = *src;
            dst++;
            dstPos++;
        }

        src++;
    }

    if (lines != 1) {
        *headerBackStart = srcSize;
    }
}

void mHandbill_Load_SuperStringFromRom(u8* buf, s32 bufSize, s32* headerBackStart, s32 superNo) {
    u32 superAddress;
    u32 superSize;

    if (superNo >= 0 && superNo < mHandbill_MAIL_NUM) {
        mHandbill_dma_super_string_dummy buff;
        u32 alignedAddr;
        u32 dataOfs;
        u32 size;
        u8 superBuf[23];
        s32 tempHeaderBackStart;
        size_t stoaSize;

        mHandbill_Get_SuperStringDataAddressAndSize(superNo, &superAddress, &superSize);
        mem_clear(buf, bufSize, CHAR_SPACE);

        if (superAddress != 0 && superSize != 0) {
            /* DMA needs 8 byte alignment, so read a bit more and move the string to the front */
            alignedAddr = superAddress & ~7;
            dataOfs = superAddress - alignedAddr;
            size = (dataOfs + superSize + 7) & ~7;

            bzero(&buff, sizeof(buff.data));
            DmaMgr_RequestSyncDebug(&buff, alignedAddr, size, "m_handbill.c", 1118);

            {
                int moveSize = superSize;
                u8* dst = (u8*)&buff;
                u8* src = (u8*)&buff + dataOfs;
                int i;

                for (i = 0; i < moveSize; i++) {
                    *dst++ = *src++;
                }
            }

            mem_clear(superBuf, 23, CHAR_SPACE);
            mHandbill_CheckSuperStringBorderAndCopy(superBuf, 23, &tempHeaderBackStart, (u8*)&buff, superSize);
            mHandbill_Change_ControlCode2(superBuf, 23, superSize - 1, &tempHeaderBackStart, mHandbill_FILL_SPACE);

            stoaSize = (bufSize < 23) ? bufSize : 23;
            bcopy(superBuf, buf, stoaSize);
            *headerBackStart = tempHeaderBackStart;
        }
    }
}

void mHandbill_Load_PsStringFromRom(u8* buf, s32 bufSize, s32 psNo) {
    u32 psAddress;
    u32 psSize;

    if (psNo >= 0 && psNo < mHandbill_MAIL_NUM) {
        mHandbill_Get_PsStringDataAddressAndSize(psNo, &psAddress, &psSize);

        if (psSize == 0) {
            mem_clear(buf, bufSize, CHAR_SPACE);
        } else if (psAddress != 0) {
            mHandbill_dma_ps_string_dummy buff;
            u32 alignedAddr;
            u32 dataOfs;
            u32 size;
            s32 i;
            s32 sz;
            u8* dst;
            u8* src;

            alignedAddr = psAddress & ~7;
            dataOfs = psAddress - alignedAddr;
            size = (dataOfs + psSize + 7) & ~7;

            DmaMgr_RequestSyncDebug(&buff, alignedAddr, size, "m_handbill.c", 1222);

            sz = psSize;
            dst = buf;
            src = buff.data + dataOfs;
            for (i = 0; i < sz; i++) {
                *dst++ = *src++;
            }

            /* pad the rest of the buffer */
            for (; i < bufSize; i++) {
                *dst++ = CHAR_SPACE;
            }

            mHandbill_Change_ControlCode(buf, bufSize, psSize, mHandbill_FILL_SPACE);
        }
    }
}

void mHandbill_Load_MailFromRom(u8* buf, s32 mailNo) {
    u32 mailAddress;
    u32 mailSize;

    if (mailNo >= 0 && mailNo < mHandbill_MAIL_NUM) {
        mHandbill_Get_MailStringDataAddressAndSize(mailNo, &mailAddress, &mailSize);

        if (mailSize == 0) {
            mem_clear(buf, mHandbill_BODY_LEN, CHAR_NEW_LINE);
        } else if (mailAddress != 0) {
            u32 alignedAddr;
            u32 dataOfs;
            u32 size;
            s32 sz;
            s32 i;
            u8* dst;
            u8* src;

            alignedAddr = mailAddress & ~7;
            dataOfs = mailAddress - alignedAddr;
            size = (dataOfs + mailSize + 7) & ~7;

            DmaMgr_RequestSyncDebug(&mHandbill_mail_buff, alignedAddr, size, "m_handbill.c", 1309);

            dst = buf;
            src = mHandbill_mail_buff.data + dataOfs;
            sz = (mailSize < mHandbill_BODY_LEN) ? mailSize : mHandbill_BODY_LEN;
            for (i = 0; i < sz; i++) {
                *dst++ = *src++;
            }

            /* pad the rest of the body with newlines */
            for (; i < mHandbill_BODY_LEN; i++) {
                *dst++ = CHAR_NEW_LINE;
            }

            mHandbill_Change_ControlCode(buf, mHandbill_BODY_LEN, sz, mHandbill_FILL_RETURN);
        }
    }
}

void mHandbill_Load_HandbillFromRom(u8* header, s32* headerBackStart, u8* footer, u8* body, s32 mailNo) {
    mHandbill_Load_SuperStringFromRom(header, mHandbill_HEADER_LEN, headerBackStart, mailNo);
    mHandbill_Load_PsStringFromRom(footer, mHandbill_FOOTER_LEN, mailNo);
    mHandbill_Load_MailFromRom(body, mailNo);
}

void mHandbill_Load_HandbillFromRom2(u8* header, s32 headerSize, s32* headerBackStart, u8* footer,
                                     s32 footerSize, u8* body, s32 mailNo) {
    mHandbill_Load_SuperStringFromRom(header, headerSize, headerBackStart, mailNo);
    mHandbill_Load_PsStringFromRom(footer, footerSize, mailNo);
    mHandbill_Load_MailFromRom(body, mailNo);
}

s32 mHandbillzDMA_body_addr_set(mHandbillzDMA_c* dmaInfo) {
    mHandbillzIdxDMASTR ram;
    s32 numZeroFlag;
    u32 idxLoadOffset;
    u32 idxRomAddr;
    u32 bodyLoadOffset;

    numZeroFlag = (dmaInfo->num == 0);

    if (numZeroFlag) {
        idxLoadOffset = 0;
    } else {
        idxLoadOffset = (dmaInfo->num - 1) << 2;
    }

    /* tables/data are segment 6 addresses, convert into an offset into the handbillz archive */
    idxRomAddr = mHandbillz_table_pos[dmaInfo->type] + idxLoadOffset - (u32)D_6000000 + (u32)D_D1A000;

    DmaMgr_RequestSyncDebug(&ram, idxRomAddr, 8, "m_handbill.c", 1567);

    bodyLoadOffset = mHandbillz_data_pos[dmaInfo->type] - (u32)D_6000000;

    if (numZeroFlag) {
        dmaInfo->dmaSize = ram.data[0];
    } else {
        bodyLoadOffset += ram.data[0];
        dmaInfo->dmaSize = ram.data[1] - ram.data[0];
    }

    dmaInfo->dmaAddr = (u32)D_D1A000 + bodyLoadOffset;

    if (dmaInfo->dmaSize <= mHandbillz_dummy_size_tbl[dmaInfo->type]) {
        return TRUE;
    }

    return FALSE;
}

s32 mHandbillzDMA_body_load(mHandbillzDMA_c* dmaInfo) {
    u32 alignedAddr;
    u32 size;
    u32 dataOfs;

    if (mHandbillzDMA_body_addr_set(dmaInfo)) {
        mem_clear(dmaInfo->ramBuf, dmaInfo->ramBufSize, CHAR_SPACE);

        if (dmaInfo->dmaSize != 0) {
            /* DMA only needs 2 byte alignment here */
            dataOfs = dmaInfo->dmaAddr & 1;
            alignedAddr = dmaInfo->dmaAddr & ~1;
            size = (dmaInfo->dmaSize + dataOfs + 1) & ~1;

            DmaMgr_RequestSyncDebug(dmaInfo->ramBuf, alignedAddr, size, "m_handbill.c", 1636);

            if (dataOfs != 0) {
                s32 i;
                s32 const iMax = dmaInfo->dmaSize;
                u8* src;
                u8* dst;

                dst = dmaInfo->ramBuf;
                src = dmaInfo->ramBuf + dataOfs;
                for (i = 0; i < iMax; i++, src++, dst++) {
                    *dst = *src;
                }
            }
        }

        return TRUE;
    } else {
        return FALSE;
    }
}

s32 mHandbillz_super_load(mHandbillz_Info_c* info) {
    mHandbillzDMA_c dmaInfo;
    mHandbillzSuperDMASTR ram;

    bzero(&ram, sizeof(ram.data));

    dmaInfo.type = mHandbillz_TYPE_SUPER;
    dmaInfo.num = info->superNo;
    dmaInfo.ramBuf = ram.data;
    dmaInfo.ramBufSize = sizeof(ram.data);

    if (mHandbillzDMA_body_load(&dmaInfo)) {
        u8 superBuf[23];
        s32 headerBackStart;
        size_t stoaSize;

        mem_clear(superBuf, 23, CHAR_SPACE);
        mHandbill_CheckSuperStringBorderAndCopy(superBuf, 23, &headerBackStart, ram.data, dmaInfo.dmaSize);
        mHandbill_Change_ControlCode2(superBuf, 23, dmaInfo.dmaSize - 1, &headerBackStart, mHandbill_FILL_SPACE);

        mem_clear(info->superBufP, info->superBufSize, CHAR_SPACE);
        stoaSize = (info->superBufSize < 23) ? info->superBufSize : 23;
        bcopy(superBuf, info->superBufP, stoaSize);

        info->headerBackStart = headerBackStart;
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 mHandbillz_mail_load(mHandbillz_Info_c* info) {
    mHandbillzDMA_c dmaInfo[3];
    mHandbillzDMA_c* dmaP;
    s32 i;
    s32 success;
    u32 totalSize;
    u8* dst;

    dst = info->mailBufP;

    dmaInfo[0].type = mHandbillz_TYPE_MAILA;
    dmaInfo[0].num = info->mailaNo;
    dmaInfo[1].type = mHandbillz_TYPE_MAILB;
    dmaInfo[1].num = info->mailbNo;
    dmaInfo[2].type = mHandbillz_TYPE_MAILC;
    dmaInfo[2].num = info->mailcNo;

    success = TRUE;
    totalSize = 0;

    for (i = 0, dmaP = dmaInfo; i < 3 && success; i++, dmaP++) {
        bzero(&mHandbill_mail_buff, 120);
        dmaP->ramBuf = mHandbill_mail_buff.data;
        dmaP->ramBufSize = 120;

        if (mHandbillzDMA_body_load(dmaP)) {
            totalSize += dmaP->dmaSize;

            if (totalSize <= info->mailBufSize) {
                bcopy(&mHandbill_mail_buff, dst, dmaP->dmaSize);
                dst += dmaP->dmaSize;
            } else {
                success = FALSE;
            }
        } else {
            success = FALSE;
        }
    }

    if (success) {
        /* pad the rest of the buffer with newlines */
        if (info->mailBufSize > totalSize) {
            mem_clear(dst, info->mailBufSize - totalSize, CHAR_NEW_LINE);
        }

        mHandbill_Change_ControlCode(info->mailBufP, info->mailBufSize, totalSize, mHandbill_FILL_RETURN);
    }

    return success;
}

s32 mHandbillz_ps_load(mHandbillz_Info_c* info) {
    mHandbillzDMA_c dmaInfo;
    mHandbillzPsDMASTR ram;

    bzero(&ram, sizeof(ram.data));

    dmaInfo.type = mHandbillz_TYPE_PS;
    dmaInfo.num = info->psNo;
    dmaInfo.ramBuf = ram.data;
    dmaInfo.ramBufSize = sizeof(ram.data);

    if (mHandbillzDMA_body_load(&dmaInfo)) {
        bcopy(&ram, info->psBufP, dmaInfo.dmaSize);

        if (info->psBufSize > dmaInfo.dmaSize) {
            mem_clear(info->psBufP + dmaInfo.dmaSize, info->psBufSize - dmaInfo.dmaSize, CHAR_SPACE);
        }

        mHandbill_Change_ControlCode(info->psBufP, info->psBufSize, dmaInfo.dmaSize, mHandbill_FILL_SPACE);
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 mHandbillz_load(mHandbillz_Info_c* info) {
    s32 superRes;
    s32 mailRes;
    s32 psRes;
    s32 res;

    superRes = mHandbillz_super_load(info);
    mailRes = mHandbillz_mail_load(info);
    psRes = mHandbillz_ps_load(info);

    res = superRes && mailRes && psRes;

    return res;
}
