#ifndef M_ITEM_NAME_H
#define M_ITEM_NAME_H

#include "ultra64.h"
#include "other_types.h"
#ifdef AFTR
#define ITEM_NAME_LEN 16
#else
#define ITEM_NAME_LEN 10
#endif

void mIN_copy_name_str(char*, u32);
void func_80096710(char* dst, RomOffset src);

#endif
