/*
Copyright 2023 Ivy Bowling <motolegacy@proton.me>

Permission is hereby granted, free of charge, to 
any person obtaining a copy of this software and 
associated documentation files (the “Software”), 
to deal in the Software without restriction, 
including without limitation the rights to use, 
copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is 
furnished to do so, subject to the following 
conditions:

The above copyright notice and this permission 
notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY 
OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
// graphics.c -- Main graphics handling
//

#include <stddef.h>
#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphics.h"
#include "rooms.h"

// The raw texture files being used
Texture2D sheet_0;
Texture2D sheet_1;
Texture2D _static;

// Sprites that utilize the textures
// Sheet 0
sprite_t tiles_gray;
sprite_t tiles_red;
sprite_t wall;
sprite_t filth_0;
sprite_t filth_1;
sprite_t golden_bonnie;
sprite_t blood;
sprite_t body;
sprite_t golden_freddy;
sprite_t golden_freddy_eyes;
sprite_t table_partyhat;
sprite_t table_fan;
sprite_t killer;
sprite_t marionette;
sprite_t present;
sprite_t letter_y;
sprite_t letter_o;
sprite_t letter_u;
sprite_t letter_c;
sprite_t letter_a;
sprite_t letter_n;
sprite_t letter_t_;
sprite_t letter_w;
sprite_t letter_s;
sprite_t letter_d;
// Sheet 1
sprite_t mangle_0;
sprite_t mangle_1;
sprite_t bonnie;
sprite_t foxy;
sprite_t stage;
sprite_t player_0;
sprite_t player_1;
sprite_t player_eyes_0;
sprite_t player_eyes_1;

//
// Graphics_Load()
// Loads all of the graphics data into memory
//
int Graphics_Load(void)
{
    sheet_0 = LoadTexture("assets/textures/sheet_0.png");
    sheet_1 = LoadTexture("assets/textures/sheet_1.png");
    _static = LoadTexture("assets/textures/static.png");

    if (sheet_0.id <= 0 || sheet_1.id <= 0 || _static.id <= 0)
        return -1;

    return 0;
}

//
// Graphics_CreateSprite(*tex, x, y, w, h)
// Returns a sprite_t struct with the
// texture and the sprite coordinates
// stored inside of it.
//
sprite_t Graphics_CreateSprite(Texture2D* tex, int x, int y, int w, int h)
{
    sprite_t sprite;
    Rectangle rect = {x, y, w, h};

    sprite.texture = tex;
    sprite.bounds = rect;

    return sprite;
}

//
// Graphics_SetUpSprites()
// Uses allocated textures to set up the
// sprites inside of them.
//
void Graphics_SetUpSprites(void)
{
    tiles_gray          = Graphics_CreateSprite(&sheet_0, 0, 0, 47, 47);
    tiles_red           = Graphics_CreateSprite(&sheet_0, 47, 0, 47, 47);
    wall                = Graphics_CreateSprite(&sheet_0, 94, 0, 22, 22);
    filth_0             = Graphics_CreateSprite(&sheet_0, 117, 0, 24, 24);
    filth_1             = Graphics_CreateSprite(&sheet_0, 141, 0, 5, 5);
    golden_bonnie       = Graphics_CreateSprite(&sheet_0, 192, 0, 50, 46);
    blood               = Graphics_CreateSprite(&sheet_0, 0, 47, 123, 48);
    body                = Graphics_CreateSprite(&sheet_0, 156, 8, 25, 33);
    golden_freddy       = Graphics_CreateSprite(&sheet_0, 123, 41, 69, 46);
    golden_freddy_eyes  = Graphics_CreateSprite(&sheet_0, 192, 46, 18, 5);
    table_partyhat      = Graphics_CreateSprite(&sheet_0, 0, 108, 111, 69);
    table_fan           = Graphics_CreateSprite(&sheet_0, 130, 160, 111, 96);
    killer              = Graphics_CreateSprite(&sheet_0, 192, 51, 50, 84);
    marionette          = Graphics_CreateSprite(&sheet_0, 117, 88, 46, 72);
    present             = Graphics_CreateSprite(&sheet_0, 0, 177, 130, 79);
    letter_y            = Graphics_CreateSprite(&sheet_0, 242, 0, 12, 16);
    letter_o            = Graphics_CreateSprite(&sheet_0, 242, 16, 12, 16);
    letter_u            = Graphics_CreateSprite(&sheet_0, 242, 32, 12, 16);
    letter_c            = Graphics_CreateSprite(&sheet_0, 242, 48, 12, 16);
    letter_a            = Graphics_CreateSprite(&sheet_0, 242, 64, 12, 16);
    letter_n            = Graphics_CreateSprite(&sheet_0, 242, 80, 12, 16);
    letter_t_           = Graphics_CreateSprite(&sheet_0, 242, 96, 12, 16);
    letter_w            = Graphics_CreateSprite(&sheet_0, 242, 112, 12, 16);
    letter_s            = Graphics_CreateSprite(&sheet_0, 242, 128, 12, 16);
    letter_d            = Graphics_CreateSprite(&sheet_0, 242, 144, 12, 16);
    mangle_0            = Graphics_CreateSprite(&sheet_1, 0, 0, 85, 46);
    mangle_1            = Graphics_CreateSprite(&sheet_1, 85, 0, 85, 46);
    bonnie              = Graphics_CreateSprite(&sheet_1, 170, 0, 49, 58);
    foxy                = Graphics_CreateSprite(&sheet_1, 199, 58, 57, 57);
    stage               = Graphics_CreateSprite(&sheet_1, 0, 58, 196, 121);
    player_0            = Graphics_CreateSprite(&sheet_1, 196, 115, 36, 49);
    player_1            = Graphics_CreateSprite(&sheet_1, 196, 164, 36, 49);
    player_eyes_0       = Graphics_CreateSprite(&sheet_1, 0, 179, 13, 8);
    player_eyes_1       = Graphics_CreateSprite(&sheet_1, 13, 179, 13, 8);
}

//
// Graphics_DrawSprite(sprite, x, y, flipped)
// Draws the sprite at the designated relative
// location (accounts for scale).
//
void Graphics_DrawSprite(sprite_t sprite, int x, int y, bool flipped)
{
    Rectangle src = {
        sprite.bounds.x, 
        sprite.bounds.y, 
        sprite.bounds.width, 
        sprite.bounds.height 
    };
    
    Rectangle dest = {
        x * GRAPHICS_SCALEFACTOR,
        y * GRAPHICS_SCALEFACTOR,
        sprite.bounds.width * GRAPHICS_SCALEFACTOR,
        sprite.bounds.height * GRAPHICS_SCALEFACTOR
    };

    if (flipped) {
        src.width *= -1;
    }

    Vector2 org = {0, 0};
    DrawTexturePro(*(sprite.texture), src, dest, org, 0, RAYWHITE);
}

//
// Graphics_Init()
// Kicks off the process of allocating spritesheets
// and setting up the bounds for each sprite.
//
int Graphics_Init(void)
{
    int ret;
    ret = Graphics_Load();

    // Allocation Passed
    if (ret == 0)
        Graphics_SetUpSprites();

    return ret;
}

//
// Graphics_DrawRoom(room)
// Redirects to draw the room the player
// is currently in.
//
void Graphics_DrawRoom(unsigned int room)
{
    switch(room) {
        case 0: Graphics_DrawRoom0(); break;
        case 1: Graphics_DrawRoom1(); break;
        case 2: Graphics_DrawRoom2(); break;
        case 3: Graphics_DrawRoom3(); break;
        case 4: Graphics_DrawRoom4(); break;
        case 5: Graphics_DrawRoom5(); break;
        case 6: Graphics_DrawRoom6(); break;
        case 7: Graphics_DrawRoom7(); break;
        case 8: Graphics_DrawRoom8(); break;
        case 9: Graphics_DrawRoom9(); break;
        case 10: Graphics_DrawRoom10(); break;
        case 11: Graphics_DrawRoom11(); break;
        case 12: Graphics_DrawRoom12(); break;
        case 13: Graphics_DrawRoom13(); break;
        case 14: Graphics_DrawRoom14(); break;
        default: break;
    }
}

//
// Graphics_DrawScanLines()
// Fakes some "scanlines" on the screen to
// mimic old CRT functionality.. loosely.
//
void Graphics_DrawScanLines(void)
{
    int height = GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR;
    int width = GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR;
    for (int i = 0; i < height; i++) {
        if (i % (GRAPHICS_SCALEFACTOR * 4) == 0) {
            DrawRectangle(0, i, width, GRAPHICS_SCALEFACTOR * 2, BLACK);
        }
    }
}

//
// Graphics_DrawStatic()
// Draws static with random opacity over
// the screen.
//
void Graphics_DrawStatic(void)
{
    // Opacity can be 0-50
    int random = rand() % 50;

    Rectangle src = {
        0,
        0,
        256,
        256
    };

    Rectangle dest = {
        0,
        0,
        GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR,
        GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR
    };

    Vector2 org = {0, 0};
    Color col = {255, 255, 255, (unsigned char)random};

    DrawTexturePro(_static, src, dest, org, 0, col);
}

//
// Graphics_DrawYouCant
// Draws "YOU CANT" on the bottom left
// of the window.
//
void Graphics_DrawYouCant(void)
{
    Graphics_DrawSprite(letter_y, 3, GRAPHICS_WINDOWHEIGHT - 20, false);
    Graphics_DrawSprite(letter_o, 17, GRAPHICS_WINDOWHEIGHT - 20, false);
    Graphics_DrawSprite(letter_u, 31, GRAPHICS_WINDOWHEIGHT - 20, false);

    Graphics_DrawSprite(letter_c, 56, GRAPHICS_WINDOWHEIGHT - 20, false);
    Graphics_DrawSprite(letter_a, 70, GRAPHICS_WINDOWHEIGHT - 20, false);
    Graphics_DrawSprite(letter_n, 84, GRAPHICS_WINDOWHEIGHT - 20, false);
    Graphics_DrawSprite(letter_t_, 98, GRAPHICS_WINDOWHEIGHT - 20, false);
}

//
// Graphics_DrawRect(x, y, w, h, col)
// Draws a rectangle relative to scale factor.
//
void Graphics_DrawRect(int x, int y, int w, int h, Color col)
{
    x *= GRAPHICS_SCALEFACTOR;
    y *= GRAPHICS_SCALEFACTOR;
    w *= GRAPHICS_SCALEFACTOR;
    h *= GRAPHICS_SCALEFACTOR;
    DrawRectangle(x, y, w, h, col);
}