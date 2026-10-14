#ifndef aftr_tools_H
#define aftr_tools_H

#include "ultra64.h"
#include "m_mail.h"

#define CHAR_SPACE 32

void aftr_NewPlayerCopy(u8* dst, u8* unusedSrc, s32 unusedLen){
    static char new_player_str[8] = {'I', '\'', 'm', ' ', ' n', 'e', 'w', '!'};
    mem_copy(dst, new_player_str, sizeof(new_player_str));
}

s32 aftr_GetStringWidth(const char* string, s32 size, s32 divide_by_twelve){
    s32 len = mMl_strlen(string, size, CHAR_SPACE);
    len = mFont_GetStringWidth(string, len, FALSE);
    if (divide_by_twelve) len = (len+6)/12;
    return len;
}

void aftr_CopyString8(char* dst, char* src) {
    mem_copy((u8*)dst, (u8*)src, 8);
}

#endif
