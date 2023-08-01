#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdbool.h>
#include <raylib.h>

// Window scale information
#define GRAPHICS_WINDOWWIDTH        320
#define GRAPHICS_WINDOWHEIGHT       240
#define GRAPHICS_SCALEFACTOR        3

typedef struct {
    Texture2D* texture;
    Rectangle bounds;
} sprite_t;

extern int  Graphics_Init(void);
extern void Graphics_DrawSprite(sprite_t sprite, int x, int y, bool flipped);
extern void Graphics_DrawRoom(unsigned int room);
extern void Graphics_DrawScanLines(void);
extern void Graphics_DrawStatic(void);
extern void Graphics_DrawYouCant(void);
extern void Graphics_DrawRect(int x, int y, int w, int h, Color col);

extern sprite_t tiles_gray;
extern sprite_t tiles_red;
extern sprite_t wall;
extern sprite_t filth_0;
extern sprite_t filth_1;
extern sprite_t golden_bonnie;
extern sprite_t blood;
extern sprite_t body;
extern sprite_t golden_freddy;
extern sprite_t golden_freddy_eyes;
extern sprite_t table_partyhat;
extern sprite_t table_fan;
extern sprite_t killer;
extern sprite_t marionette;
extern sprite_t present;
extern sprite_t letter_y;
extern sprite_t letter_o;
extern sprite_t letter_u;
extern sprite_t letter_c;
extern sprite_t letter_a;
extern sprite_t letter_n;
extern sprite_t letter_t_;
extern sprite_t letter_w;
extern sprite_t letter_s;
extern sprite_t letter_d;
extern sprite_t mangle_0;
extern sprite_t mangle_1;
extern sprite_t bonnie;
extern sprite_t foxy;
extern sprite_t stage;
extern sprite_t player_0;
extern sprite_t player_1;
extern sprite_t player_eyes_0;
extern sprite_t player_eyes_1;

#endif // _GRAPHICS_H_