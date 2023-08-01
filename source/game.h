#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>

#include "graphics.h"

#define GAMESTATE_REDSCREEN     0
#define GAMESTATE_GAMEPLAY      1
#define GAMESTATE_BLUESCREEN    2

#define PUPPET_GO_UP            0
#define PUPPET_GO_DOWN          1
#define PUPPET_GO_RIGHT         2

typedef struct {
    sprite_t        sprite;
    unsigned char   direction;
    Vector2         origin;
    Rectangle       bounds;
} ai_t;

extern unsigned int game_state;
extern unsigned int room;
extern bool first_room;
extern bool draw_you_cant;
extern bool purple_guy_active;

extern void Game_Update(void);
extern void Game_Init(void);
extern void Game_PlayerIsInNewRoom(unsigned char dir);
extern void Game_SetEndScreen(bool blue);

#endif // _GAME_H_