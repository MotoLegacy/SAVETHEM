#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "graphics.h"

extern void Player_Update(void);
extern void Player_Init(void);

typedef struct {
    sprite_t sprite;
    Vector2 origin;
    Rectangle hitbox;
    unsigned char direction;
} player_t;

extern player_t player;

#endif // _PLAYER_H_