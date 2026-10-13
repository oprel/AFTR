#ifndef M_FUUSEN_H
#define M_FUUSEN_H

#include "ultra64.h"

void Balloon_init(struct Game_Play* game_play);
void Balloon_move(struct Game_Play* game_play);
void Balloon_kill(void);
void Balloon_look_up(void);

#endif