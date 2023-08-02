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
// player.c -- Player movement and rendering
//

#include <time.h>
#include <stdbool.h>

#include <raylib.h>

#include "clockfix.h"
#include "player.h"
#include "game.h"
#include "rooms.h"
#include "graphics.h"

player_t player;
clock_t last_framechange;
bool alternated_frame;
// hacky fix for killer not activating
bool player_going_up_or_down;

//
// Player_Draw()
// Draw that little guy!
//
void Player_Draw(void)
{
    // clockwise, 0 = down
    switch(player.direction) {
        case 0:
            Graphics_DrawSprite(player.sprite, player.origin.x, player.origin.y, false);
            Graphics_DrawSprite(player_eyes_0, player.origin.x + 11, player.origin.y + 6, false);
            break;
        case 1:
            Graphics_DrawSprite(player.sprite, player.origin.x, player.origin.y, false);
            Graphics_DrawSprite(player_eyes_1, player.origin.x + 11, player.origin.y + 6, true);
            break;
        case 2:
            Graphics_DrawSprite(player.sprite, player.origin.x, player.origin.y, true);
            break;
        case 3:
            Graphics_DrawSprite(player.sprite, player.origin.x, player.origin.y, false);
            Graphics_DrawSprite(player_eyes_1, player.origin.x + 11, player.origin.y + 6, false);
            break;
        default:
            break;
    }

    // Display "WASD" for a quick movement tutorial
    if (first_room) {
        Graphics_DrawSprite(letter_w, player.origin.x + 12, player.origin.y - 50, false);
        Graphics_DrawSprite(letter_s, player.origin.x + 12, player.origin.y - 25, false);
        Graphics_DrawSprite(letter_a, player.origin.x - 8, player.origin.y - 25, false);
        Graphics_DrawSprite(letter_d, player.origin.x + 32, player.origin.y - 25, false);
    }

    // Update the player sprite
    double time_since_last;
    time_since_last = (double)(clock() - last_framechange) / CLOCKS_PER_SEC;
    if (time_since_last >= 0.04) {
        alternated_frame = !alternated_frame;

        if (alternated_frame) {
            player.sprite = player_1;
        } else {
            player.sprite = player_0;
        }

        last_framechange = clock();
    }
}

//
// Player_CheckIfRoomTrigger()
// Pretty identical to FixCollisions, but instead
// checks to see if we need to move the player and
// change the room.
//
void Player_CheckIfRoomTrigger(void)
{
    double left_plr = (double)player.hitbox.x + (player.origin.x * GRAPHICS_SCALEFACTOR);
    double right_plr = left_plr + player.hitbox.width;
    double top_plr = (double)player.hitbox.y + (player.origin.y * GRAPHICS_SCALEFACTOR);
    double bottom_plr = top_plr + player.hitbox.height;

    for (int i = 0; i < 4; i++) {
        double left_obj = (double)room_triggers[i].bounds.x - 1;
        double right_obj = left_obj + room_triggers[i].bounds.width + 1;
        double top_obj = (double)room_triggers[i].bounds.y - 1;
        double bottom_obj = top_obj + room_triggers[i].bounds.height + 1;

        // They're in it, teleport them.
        if (top_plr <= bottom_obj && bottom_plr >= top_obj &&
        right_plr >= left_obj && left_plr <= right_obj) {
            // If we leave the room the killer was in,
            // de-spawn him.
            purple_guy_active = false;

            // Change the room
            room = room_triggers[i].roomid;
            player.origin.x = room_triggers[i].spawncoords.x;
            player.origin.y = room_triggers[i].spawncoords.y;
            first_room = false;

            if (player_going_up_or_down == false)
                Game_PlayerIsInNewRoom(player.direction);
            else
                Game_PlayerIsInNewRoom(0);
        }
    }
}

// 
// Player_FixCollisions(dir)
// Returns true if the player is colliding with the
// world given a specific direction. (AABB)
//
bool Player_FixCollisions(unsigned char dir) 
{
    double left_plr = (double)player.hitbox.x + (player.origin.x * GRAPHICS_SCALEFACTOR);
    double right_plr = left_plr + player.hitbox.width;
    double top_plr = (double)player.hitbox.y + (player.origin.y * GRAPHICS_SCALEFACTOR);
    double bottom_plr = top_plr + player.hitbox.height;

    for (int i = 0; i < MAX_COLLISIONRECTS; i++) {
        double left_obj = (double)bboxes[i].bounds.x - 1;
        double right_obj = left_obj + bboxes[i].bounds.width + 1;
        double top_obj = (double)bboxes[i].bounds.y - 1;
        double bottom_obj = top_obj + bboxes[i].bounds.height + 1;

        // Push the player back if they're colliding.
        if (top_plr <= bottom_obj && bottom_plr >= top_obj &&
        right_plr >= left_obj && left_plr <= right_obj) {
            switch (dir) {
                case 0: player.origin.y -= 0.5; break;
                case 1: player.origin.x += 0.55; break;
                case 2: player.origin.y += 0.5; break;
                case 3: player.origin.x -= 0.55; break;
            }

            // If this collision box is set to end the game, do that
            if (bboxes[i].end_game == true) {
                Game_SetEndScreen(false);
            }
        }
    }

    return false;
}

//
// Player_Update()
// Handles updating the player input and
// rendering calls.
//
void Player_Update(void)
{
    Player_Draw();
    player_going_up_or_down = false;

    unsigned char dir;

    // Input
    // Luckily movement is super simple ha

    // Up
    if (IsKeyDown(KEY_W)) {
        dir = 2;
        player.origin.y -= 0.5;
        player_going_up_or_down = true;

        Player_FixCollisions(dir);
    } 
    // Down
    else if (IsKeyDown(KEY_S)) {
        dir = 0;
        player.origin.y += 0.5;
        player_going_up_or_down = true;

        Player_FixCollisions(dir);
    }

    // Left
    if (IsKeyDown(KEY_A)) {
        dir = 1;
        player.origin.x -= 0.55;

        Player_FixCollisions(dir);
    } 
    // Right
    else if (IsKeyDown(KEY_D)) {
        dir = 3;
        player.origin.x += 0.55;

        Player_FixCollisions(dir);
    }

    player.direction = dir;
    Player_CheckIfRoomTrigger();
}

//
// Player_Init()
// Player struct initialization
//
void Player_Init(void) 
{
    // Graphic
    player.sprite = player_0;

    // Bounding box
    player.hitbox.x = 0 * GRAPHICS_SCALEFACTOR;
    player.hitbox.y = 16 * GRAPHICS_SCALEFACTOR;
    player.hitbox.width = 36 * GRAPHICS_SCALEFACTOR;
    player.hitbox.height = 34 * GRAPHICS_SCALEFACTOR;
}