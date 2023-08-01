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
// rooms.c -- Room rendering and collision setup
//

#include <stdbool.h>

#include "graphics.h"
#include "rooms.h"

collision_t bboxes[MAX_COLLISIONRECTS];
int current_col_index;

roomtrigger_t    room_triggers[4];
int current_room_trigger;

//
// Rooms_ResetCollision()
// Sets all collision boxes to zero.
//
void Rooms_ResetCollision(void)
{
    current_col_index = 0;

    for (int i = 0; i < MAX_COLLISIONRECTS; i++) {
        bboxes[i].bounds.x = bboxes[i].bounds.y =
        bboxes[i].bounds.width = bboxes[i].bounds.height = 0;
        bboxes[i].end_game = false;
    }

    current_room_trigger = 0;
    for (int i = 0; i < 4; i++) {
        room_triggers[i].roomid = -1;
        room_triggers[i].bounds.x = room_triggers[i].bounds.y =
        room_triggers[i].bounds.width = room_triggers[i].bounds.height = 0;
    }
}

//
// Rooms_DrawFloor(color)
// Draws either a series of red or gray tiles
// for the floor.
//
void Rooms_DrawFloor(int color)
{
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 6; y++) {
            x *= 47;
            y *= 47;

            if (color)
                Graphics_DrawSprite(tiles_red, x, y, false);
            else
                Graphics_DrawSprite(tiles_gray, x, y, false);

            x /= 47;
            y /= 47;
        }
    }
}

//
// Rooms_DrawBackWall()
// Draws the back wall for a room.
//
void Rooms_DrawBackWall(void)
{
    for (int i = 0; i < 15; i++) {
        int x = i * 22;
        Graphics_DrawSprite(wall, x, 0, false);
    }

    bboxes[current_col_index].bounds.x = bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.width = GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;
}

//
// Rooms_DrawBackEntrance(roomid)
// Draws the back entrance for a room, and places
// appropriate teleportation trigger.
//
void Rooms_DrawBackEntrance(unsigned int roomid)
{
    for (int i = 0; i < 5; i++) {
        int x = i * 22;
        Graphics_DrawSprite(wall, x, 0, false);
        Graphics_DrawSprite(wall, 320 - 22 - x, 0, false);
    }

    bboxes[current_col_index].bounds.x = bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.width = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (210 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    bboxes[current_col_index].bounds.x = 210 * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.width = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (210 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    room_triggers[current_room_trigger].roomid = roomid;
    room_triggers[current_room_trigger].spawncoords.x = 145;
    room_triggers[current_room_trigger].spawncoords.y = 189;
    room_triggers[current_room_trigger].bounds.x = 110 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.y = -10;
    room_triggers[current_room_trigger].bounds.width = 100 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.height = 2 * GRAPHICS_SCALEFACTOR;
    current_room_trigger++;
}

//
// Rooms_DrawLeftWall()
// Draws the left wall for a room.
//
void Rooms_DrawLeftWall(void)
{
    for (int i = 1; i < 10; i++) {
        int y = i * 22;
        Graphics_DrawSprite(wall, 0, y, false);
    }

    bboxes[current_col_index].bounds.x = bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.height = GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;
}

//
// Rooms_DrawLeftEntrance(roomid)
// Draws the left entrance for a room, and places
// appropriate teleportation trigger.
//
void Rooms_DrawLeftEntrance(unsigned int roomid)
{
    for (int i = 0; i < 4; i++) {
        int y = i * 22;
        // bad hack!
        if (i == 3)
            y -= 6;
        Graphics_DrawSprite(wall, 0, y, false);
        Graphics_DrawSprite(wall, 0, 240 - 22 - y, false);
    }

    bboxes[current_col_index].bounds.x = 0;
    bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.height = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (152 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    bboxes[current_col_index].bounds.x = 0;
    bboxes[current_col_index].bounds.y = 152 * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.height = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (152 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    room_triggers[current_room_trigger].roomid = roomid;
    room_triggers[current_room_trigger].spawncoords.x = GRAPHICS_WINDOWWIDTH - 39;
    room_triggers[current_room_trigger].spawncoords.y = 96;
    room_triggers[current_room_trigger].bounds.x = 0;
    room_triggers[current_room_trigger].bounds.y = 88 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.width = 2 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.height = 64 * GRAPHICS_SCALEFACTOR;
    current_room_trigger++;
}

//
// Rooms_DrawRightWall()
// Draws the right wall for a room.
//
void Rooms_DrawRightWall(void)
{
    for (int i = 1; i < 10; i++) {
        int y = i * 22;
        Graphics_DrawSprite(wall, 320 - 22, y, false);
    }

    bboxes[current_col_index].bounds.x = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.height = GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;
}

//
// Rooms_DrawRightEntrance(roomid)
// Draws the right entrance for a room, and places
// appropriate teleportation trigger.
//
void Rooms_DrawRightEntrance(unsigned int roomid)
{
    for (int i = 0; i < 4; i++) {
        int y = i * 22;
        // bad hack!
        if (i == 3)
            y -= 6;
        Graphics_DrawSprite(wall, 320 - 22, y, false);
        Graphics_DrawSprite(wall, 320 - 22, 240 - 22 - y, false);
    }

    bboxes[current_col_index].bounds.x = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.y = 0;
    bboxes[current_col_index].bounds.height = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (152 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    bboxes[current_col_index].bounds.x = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.y = 152 * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.height = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (152 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    room_triggers[current_room_trigger].roomid = roomid;
    room_triggers[current_room_trigger].spawncoords.x = 3;
    room_triggers[current_room_trigger].spawncoords.y = 96;
    room_triggers[current_room_trigger].bounds.x = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (2 * GRAPHICS_SCALEFACTOR);
    room_triggers[current_room_trigger].bounds.y = 88 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.width = 2 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.height = 64 * GRAPHICS_SCALEFACTOR;
    current_room_trigger++;
}

//
// Rooms_DrawFrontWall()
// Draws the front wall for a room.
//
void Rooms_DrawFrontWall(void)
{
    for (int i = 0; i < 15; i++) {
        int x = i * 22;
        Graphics_DrawSprite(wall, x, 240 - 22, false);
    }

    bboxes[current_col_index].bounds.x = 0;
    bboxes[current_col_index].bounds.y = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;
}

//
// Rooms_DrawFrontEntrance(roomid)
// Draws the front entrance for a room and places
// appropriate teleportation trigger.
//
void Rooms_DrawFrontEntrance(unsigned int roomid)
{
    for (int i = 0; i < 5; i++) {
        int x = i * 22;
        Graphics_DrawSprite(wall, x, 240 - 22, false);
        Graphics_DrawSprite(wall, 320 - 22 - x, 240 - 22, false);
    }

    bboxes[current_col_index].bounds.x = 0;
    bboxes[current_col_index].bounds.y = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (210 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    bboxes[current_col_index].bounds.x = 210 * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.y = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) - (22 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.width = (GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR) - (210 * GRAPHICS_SCALEFACTOR);
    bboxes[current_col_index].bounds.height = 22 * GRAPHICS_SCALEFACTOR;
    current_col_index++;

    room_triggers[current_room_trigger].roomid = roomid;
    room_triggers[current_room_trigger].spawncoords.x = 145;
    room_triggers[current_room_trigger].spawncoords.y = 0;
    room_triggers[current_room_trigger].bounds.x = 110 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.y = (GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR) + (20 * GRAPHICS_SCALEFACTOR);
    room_triggers[current_room_trigger].bounds.width = 100 * GRAPHICS_SCALEFACTOR;
    room_triggers[current_room_trigger].bounds.height = 2 * GRAPHICS_SCALEFACTOR;
    current_room_trigger++;
}

//
// Rooms_DrawObject(sprite, x, y, flipped, end_game)
// Wrapper for Graphics_DrawSprite that also sets up
// collision rectangles for said object.
//
void Rooms_DrawObject(sprite_t sprite, int x, int y, bool flipped, bool end_game)
{
    Graphics_DrawSprite(sprite, x, y, flipped);

    bboxes[current_col_index].bounds.x = x * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.y = y * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.width = sprite.bounds.width * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].bounds.height = sprite.bounds.height * GRAPHICS_SCALEFACTOR;
    bboxes[current_col_index].end_game = end_game;
    current_col_index++;
}

//
// Graphics_DrawRoom0()
// Draws and sets up collision boxes for
// room 0.
//
void Graphics_DrawRoom0(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(0);
    Rooms_DrawBackWall();
    Rooms_DrawLeftWall();
    Rooms_DrawRightWall();
    Rooms_DrawFrontEntrance(4);
    Graphics_DrawSprite(blood, 40, 126, false);
    Rooms_DrawObject(body, 35, 174, true, true);
    Rooms_DrawObject(stage, 62, 15, false, false);
}

//
// Graphics_DrawRoom1()
// Draws and sets up collision boxes for
// room 1.
//
void Graphics_DrawRoom1(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackWall();
    Rooms_DrawLeftWall();
    Rooms_DrawRightEntrance(2);
    Rooms_DrawFrontWall();
    Rooms_DrawObject(foxy, 240, 23, false, false);
    Rooms_DrawObject(bonnie, 48, 50, false, false);
    Rooms_DrawObject(golden_bonnie, 29, 135, false, false);
}

//
// Graphics_DrawRoom2()
// Draws and sets up collision boxes for
// room 2.
//
void Graphics_DrawRoom2(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(1);
    Rooms_DrawBackWall();
    Rooms_DrawLeftEntrance(1);
    Rooms_DrawRightEntrance(3);
    Rooms_DrawFrontEntrance(6);
}

//
// Graphics_DrawRoom3()
// Draws and sets up collision boxes for
// room 3.
//
void Graphics_DrawRoom3(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(1);
    Rooms_DrawBackWall();
    Rooms_DrawLeftEntrance(2);
    Rooms_DrawRightEntrance(4);
    Rooms_DrawFrontWall();
    Rooms_DrawObject(body, 263, 181, false, true);
    Graphics_DrawSprite(filth_0, 114, 171, false);
    Graphics_DrawSprite(filth_0, 234, 96, true);
    Graphics_DrawSprite(filth_0, 75, 55, false);
    Graphics_DrawSprite(filth_1, 197, 191, false);
    Graphics_DrawSprite(filth_1, 107, 76, false);
    Graphics_DrawSprite(filth_1, 259, 54, false);
}

//
// Graphics_DrawRoom4()
// Draws and sets up collision boxes for
// room 4.
//
void Graphics_DrawRoom4(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(0);
    Rooms_DrawBackEntrance(0);
    Rooms_DrawLeftEntrance(3);
    Rooms_DrawRightWall();
    Rooms_DrawFrontEntrance(8);
    Graphics_DrawSprite(filth_0, 114, 171, false);
    Graphics_DrawSprite(filth_0, 234, 96, true);
    Graphics_DrawSprite(filth_0, 75, 55, false);
    Graphics_DrawSprite(filth_1, 197, 191, false);
    Graphics_DrawSprite(filth_1, 107, 76, false);
    Graphics_DrawSprite(filth_1, 259, 54, false);
    Rooms_DrawObject(table_partyhat, 183, 31, false, false);
    Rooms_DrawObject(table_partyhat, 177, 102, false, false);
}

//
// Graphics_DrawRoom5()
// Draws and sets up collision boxes for
// room 5.
//
void Graphics_DrawRoom5(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackWall();
    Rooms_DrawLeftWall();
    Rooms_DrawRightEntrance(6);
    Rooms_DrawFrontWall();
    Rooms_DrawObject(table_partyhat, 30, 31, false, false);
    Rooms_DrawObject(table_partyhat, 28, 102, false, false);
}

//
// Graphics_DrawRoom6()
// Draws and sets up collision boxes for
// room 6.
//
void Graphics_DrawRoom6(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(0);
    Rooms_DrawBackEntrance(2);
    Rooms_DrawLeftEntrance(5);
    Rooms_DrawRightEntrance(7);
    Rooms_DrawFrontEntrance(11);
}

//
// Graphics_DrawRoom7()
// Draws and sets up collision boxes for
// room 7.
//
void Graphics_DrawRoom7(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackWall();
    Rooms_DrawLeftEntrance(6);
    Rooms_DrawRightWall();
    Rooms_DrawFrontWall();
    Rooms_DrawObject(table_partyhat, 183, 31, false, false);
    Rooms_DrawObject(table_partyhat, 177, 102, false, false);
}

//
// Graphics_DrawRoom8()
// Draws and sets up collision boxes for
// room 8.
//
void Graphics_DrawRoom8(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(0);
    Rooms_DrawBackEntrance(4);
    Rooms_DrawLeftWall();
    Rooms_DrawRightEntrance(9);
    Rooms_DrawFrontEntrance(13);
    Graphics_DrawSprite(blood, 39, 70, false);
    Graphics_DrawSprite(blood, 169, 132, false);
}

//
// Graphics_DrawRoom9()
// Draws and sets up collision boxes for
// room 9.
//
void Graphics_DrawRoom9(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(1);
    Rooms_DrawBackWall();
    Rooms_DrawLeftEntrance(8);
    Rooms_DrawRightWall();
    Rooms_DrawFrontWall();
    Rooms_DrawObject(body, 156, 183, false, true);
    Rooms_DrawObject(present, 167, 93, false, false);
}

//
// Graphics_DrawRoom10()
// Draws and sets up collision boxes for
// room 10.
//
void Graphics_DrawRoom10(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackWall();
    Rooms_DrawLeftWall();
    Rooms_DrawRightEntrance(11);
    Rooms_DrawFrontWall();
    Rooms_DrawObject(table_partyhat, 30, 31, false, false);
    Rooms_DrawObject(table_partyhat, 28, 102, false, false);
    Rooms_DrawObject(body, 50, 174, true, true);
}

//
// Graphics_DrawRoom11()
// Draws and sets up collision boxes for
// room 11.
//
void Graphics_DrawRoom11(void)
{
    Rooms_ResetCollision();
    Rooms_DrawFloor(0);
    Rooms_DrawBackEntrance(6);
    Rooms_DrawLeftEntrance(10);
    Rooms_DrawRightEntrance(12);
    Rooms_DrawFrontEntrance(14);
    Graphics_DrawSprite(filth_0, 114, 171, false);
    Graphics_DrawSprite(filth_0, 234, 96, true);
    Graphics_DrawSprite(filth_0, 75, 55, false);
    Graphics_DrawSprite(filth_1, 197, 191, false);
    Graphics_DrawSprite(filth_1, 107, 76, false);
    Graphics_DrawSprite(filth_1, 259, 54, false);
    Graphics_DrawSprite(blood, 39, 70, false);
    Graphics_DrawSprite(blood, 169, 132, false);
}

//
// Graphics_DrawRoom12()
// Draws and sets up collision boxes for
// room 12.
//
void Graphics_DrawRoom12(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackWall();
    Rooms_DrawLeftEntrance(11);
    Rooms_DrawRightWall();
    Rooms_DrawFrontWall();
    Rooms_DrawObject(table_partyhat, 183, 31, false, false);
    Rooms_DrawObject(table_partyhat, 177, 102, false, false);
}

//
// Graphics_DrawRoom13()
// Draws and sets up collision boxes for
// room 13.
//
void Graphics_DrawRoom13(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackEntrance(8);
    Rooms_DrawLeftWall();
    Rooms_DrawRightWall();
    Rooms_DrawFrontWall();
    Rooms_DrawObject(body, 35, 174, true, true);
}

//
// Graphics_DrawRoom14()
// Draws and sets up collision boxes for
// room 14.
//
void Graphics_DrawRoom14(void)
{
    Rooms_ResetCollision();
    Rooms_DrawBackEntrance(11);
    Rooms_DrawLeftWall();
    Rooms_DrawRightWall();
    Rooms_DrawFrontWall();
    Rooms_DrawObject(table_fan, 165, 114, false, false);
}