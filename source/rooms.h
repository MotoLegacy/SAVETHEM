#ifndef _ROOMS_H_
#define _ROOMS_H_

#include <stdbool.h>

#define MAX_COLLISIONRECTS          16

typedef struct
{
    unsigned int    roomid;         // Room the trigger takes you to.
    Vector2         spawncoords;    // Origin of the player in the new room.
    Rectangle       bounds;         // Collision/trigger size for the room bbox.
} roomtrigger_t;

typedef struct
{
    Rectangle   bounds;
    bool        end_game;
} collision_t;

extern roomtrigger_t    room_triggers[4];   // There can only ever be 4 triggers period, no use macro-ing.
extern collision_t      bboxes[MAX_COLLISIONRECTS];

void Graphics_DrawRoom0(void);
void Graphics_DrawRoom1(void);
void Graphics_DrawRoom2(void);
void Graphics_DrawRoom3(void);
void Graphics_DrawRoom4(void);
void Graphics_DrawRoom5(void);
void Graphics_DrawRoom6(void);
void Graphics_DrawRoom7(void);
void Graphics_DrawRoom8(void);
void Graphics_DrawRoom9(void);
void Graphics_DrawRoom10(void);
void Graphics_DrawRoom11(void);
void Graphics_DrawRoom12(void);
void Graphics_DrawRoom13(void);
void Graphics_DrawRoom14(void);

#endif // _ROOMS_H_