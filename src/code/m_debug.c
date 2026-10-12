#include "m_debug.h"
#include "libc64/malloc.h"
#include "m_std_dma.h"
#include "segment_symbols.h"

Debug_mode* debug_mode;

#ifdef AFTR
// Same behavior as original, but smaller: the room is used to load include/aftr_tools.h
void new_Debug_mode() {
    debug_mode = malloc(sizeof(Debug_mode));

    bzero(debug_mode, sizeof(Debug_mode));

    DmaMgr_RequestSyncNoSize(SEGMENT_VRAM_START(aftr_tools), SEGMENT_ROM_START(aftr_tools));
    osInvalICache(SEGMENT_VRAM_START(aftr_tools), SEGMENT_ROM_SIZE_ALT(aftr_tools));
}

#pragma GLOBAL_ASM("src/code/m_debug_pad.inc")
#else
void new_Debug_mode() {
    s32 i;

    debug_mode = malloc(sizeof(Debug_mode));

    debug_mode->mode = 0;
    debug_mode->type = 0;
    debug_mode->inputR = 0;
    debug_mode->oldKey = 0;
    debug_mode->keyWait = 0;

    for (i = 0; i < DEBUG_REG_COUNT; i++) {
        debug_mode->r[i] = 0;
    }
}
#endif
