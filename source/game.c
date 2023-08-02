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
// game.c -- Actual gameplay and handling
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <raylib.h>

#include "clockfix.h"
#include "game.h"
#include "graphics.h"
#include "audio.h"
#include "player.h"

// Game's state
unsigned int game_state;

// Should the game close after
// this state ends?
bool close_after_state;

// SAVETHEM speech
int letter;
clock_t last_savethem;

// Looping BGS
clock_t last_looptime;

// What room are we in?
// ---
// Top to bottom, left to right based
// on this floor plan:
// static.wikia.nocookie.net/freddy-fazbears-pizza/images/1/13/Death-minigame-map.gif/revision/latest
unsigned int room;

// First room you spawn in has "WASD"
// Text follow player, so keep track
bool first_room;

// tells main loop to draw "YOU CANT"
bool draw_you_cant;

// Red/Blue Screen vars
bool screen_init;
unsigned int screen_time;
Rectangle screen_rects[2];
clock_t last_rectgen;
clock_t screen_starttime;

// The killer AI
bool purple_guy_active;
ai_t ai_killer;

// Puppet AI
ai_t ai_marionette;
bool puppet_in_room;
bool marionette_active;
bool visited_rooms[6];

// Mangle AI
ai_t ai_mangle;
bool mangle_go_left;
bool mangle_alternated_frame;

// Golden Freddy AI
ai_t ai_goldenfreddy;
bool golden_freddy_active;
bool golden_freddy_has_eyes;
clock_t golden_freddy_time;

// End counter
clock_t last_ticktime;

extern int frame_counter;

//
// Game_SAVETHEM()
// "SAVETHEM" sound triggering
//
void Game_SAVETHEM(void)
{
    double time_since_last;
    time_since_last = (double)(clock() - last_savethem) / CLOCKS_PER_SEC;

    // It's been ~3 seconds, so play a letter.
    if (time_since_last >= 0.28) {
        // Play it.
        switch(letter) {
            case 0: PlaySound(speak_s); break;
            case 1: PlaySound(speak_a); break;
            case 2: PlaySound(speak_v); break;
            case 3: PlaySound(speak_e); break;
            case 4: PlaySound(speak_t_); break;
            case 5: PlaySound(speak_h); break;
            case 6: PlaySound(speak_e); break;
            case 7: PlaySound(speak_m); break;
            default: break;
        }

        // Decide what letter to speak next
        letter++;
        if (letter > 7)
            letter = 0;

        // Reset the timer.
        last_savethem = clock();
   
    }
}

//
// Game_LoopSound()
// "loop" sound re-trigger
//
void Game_LoopSound(void)
{
    double time_since_last;
    time_since_last = (double)(clock() - last_looptime) / CLOCKS_PER_SEC;

    // 33 second loop
    if (time_since_last >= 3.33) {
        StopSound(loop);
        PlaySound(loop);
        last_looptime = clock();
    }
}

//
// Game_SetEndScreen(blue)
// Tells the game to end, and specify
// if the screen should be red or blue.
//
void Game_SetEndScreen(bool blue)
{
    if (blue)
        game_state = GAMESTATE_BLUESCREEN;
    else
        game_state = GAMESTATE_REDSCREEN;
    close_after_state = true;

    StopSound(loop);
    StopSound(speak_s);
    StopSound(speak_a);
    StopSound(speak_v);
    StopSound(speak_e);
    StopSound(speak_t_);
    StopSound(speak_h);
    StopSound(speak_m);
}


//
// Game_TickAway()
// Ticks away for chance to auto-end
// game.
//
void Game_TickAway(void)
{
    double time_since_last;
    time_since_last = (double)(clock() - last_ticktime) / CLOCKS_PER_SEC;

    // 20 second intervals
    if (time_since_last >= 2) {
        // 15% chance, do we end the game?
        int random = rand() % 100;

        if (random <= 15) {
            Game_SetEndScreen(false);
        }
        last_ticktime = clock();
    }
}

//
// Game_SpawnPuppet()
// Tells the game Puppet is here.
//
void Game_SpawnPuppet(void)
{
    marionette_active = true;
    ai_marionette.sprite = marionette;

    for(int i = 0; i < 6; i++) {
        visited_rooms[i] = false;
    }
}

//
// Game_PuppetUpdate()
// Move the Puppet towards the
// Prize Corner.
//
void Game_PuppetUpdate(void)
{
    if (puppet_in_room)
        Graphics_DrawSprite(ai_marionette.sprite, ai_marionette.origin.x, 
                            ai_marionette.origin.y, false);

    switch(ai_marionette.direction) {
        case PUPPET_GO_UP:
            ai_marionette.origin.y -= 0.60;
            break;
        case PUPPET_GO_DOWN:
            ai_marionette.origin.y += 0.60;
            break;
        case PUPPET_GO_RIGHT:
            ai_marionette.origin.x += 0.60;
            break;
        default:
            break;
    }
}

//
// Game_UpdatePuppetPath(index, dir)
// Updates the position of the Marionette
// and gives it a new direction to move
// towards.
//
void Game_UpdatePuppetPath(int index, unsigned char dir)
{
    // Avoid sequence breaking -- if we got here,
    // assume the player came from a previous room
    // too.
    for(int i = 0; i < index; i++) {
        visited_rooms[i] = true;
    }

    if (visited_rooms[index] == false) {
        visited_rooms[index] = true;
        puppet_in_room = true;
        ai_marionette.direction = dir;

        switch(dir) {
            case PUPPET_GO_UP:
                ai_marionette.origin.x = 144;
                ai_marionette.origin.y = 60;
                break;
            case PUPPET_GO_DOWN:
                ai_marionette.origin.x = 144;
                ai_marionette.origin.y = 128;
                break;
            case PUPPET_GO_RIGHT:
                ai_marionette.origin.x = 175;
                ai_marionette.origin.y = 83;
                break;
            default:
                break;
        }
    }
}

//
// Game_SpawnPurpleGuy()
// Tells the game Purple Guy is here.
//
void Game_SpawnPurpleGuy(void)
{
    purple_guy_active = true;
    ai_killer.origin.x = 230;
    ai_killer.origin.y = 90;
    ai_killer.bounds.x = 0 * GRAPHICS_SCALEFACTOR;
    ai_killer.bounds.y = 0 * GRAPHICS_SCALEFACTOR;
    ai_killer.bounds.width = killer.bounds.width * GRAPHICS_SCALEFACTOR;
    ai_killer.bounds.height = killer.bounds.height * GRAPHICS_SCALEFACTOR;
    ai_killer.sprite = killer;
}

//
// Game_PurpleGuyUpdate()
// Moves the killer towards you, 
// sets the blue screen on contact.
//
void Game_PurpleGuyUpdate(void)
{
    if (purple_guy_active == false)
        return;

    Graphics_DrawSprite(ai_killer.sprite, ai_killer.origin.x, ai_killer.origin.y, false);
    ai_killer.origin.y = player.origin.y - 20;
    ai_killer.origin.x -= 2;

    if (ai_killer.origin.x <= (player.origin.x + 36)) {
        Game_SetEndScreen(true);
    }
}

//
// Game_MangleUpdate()
// Has mangle crawl around on the floor.
//
void Game_MangleUpdate(void)
{
    ai_mangle.origin.y = 150;

    if (ai_mangle.origin.x == 0)
        ai_mangle.origin.x = 92;

    if (mangle_go_left) {
        ai_mangle.origin.x -= 0.15;
        if (ai_mangle.origin.x <= 92) {
            mangle_go_left = !mangle_go_left;
            ai_mangle.origin.x = 92;
        }
    }
    else {
        ai_mangle.origin.x += 0.15;
        if (ai_mangle.origin.x >= 150) {
            mangle_go_left = !mangle_go_left;
            ai_mangle.origin.x = 150;
        }
    }

    // Time to change its sprite
    if (frame_counter % 40 == 0) {
        if (!mangle_alternated_frame)
            ai_mangle.sprite = mangle_1;
        else
            ai_mangle.sprite = mangle_0;

        mangle_alternated_frame = !mangle_alternated_frame;
    }

    if (room == 13)
        Graphics_DrawSprite(ai_mangle.sprite, ai_mangle.origin.x, ai_mangle.origin.y, mangle_go_left);
}

//
// Game_SpawnGoldenFreddy()
// Puts Golden Freddy in the room.
//
void Game_SpawnGoldenFreddy(void)
{
    int spawnx = 0;
    int spawny = 0;

    while(spawnx <= 20) {
        spawnx = rand() % 200;
    }

    while (spawny <= 20) {
        spawny = rand() % 120;
    }

    // 20% chance of eyes
    if (rand() % 100 >= 80) {
        golden_freddy_has_eyes = true;
        golden_freddy_time = clock();
    } else {
        golden_freddy_has_eyes = false;
    }

    ai_goldenfreddy.sprite = golden_freddy;
    ai_goldenfreddy.origin.x = spawnx;
    ai_goldenfreddy.origin.y = spawny;
}

//
// Game_GoldenFreddyUpdate()
// Update Golden Freddy.
//
void Game_GoldenFreddyUpdate(void)
{
    Graphics_DrawSprite(ai_goldenfreddy.sprite, ai_goldenfreddy.origin.x, ai_goldenfreddy.origin.y, false);

    if (golden_freddy_has_eyes) {
        Graphics_DrawSprite(golden_freddy_eyes, ai_goldenfreddy.origin.x + 18, ai_goldenfreddy.origin.y + 8, false);

        double time_since_last;
        time_since_last = (double)(clock() - golden_freddy_time) / CLOCKS_PER_SEC;

        // Despawn timer
        if (time_since_last >= 0.1) {
            golden_freddy_active = false;
        }
    }
}

//
// Game_PlayerIsInNewRoom(dir)
// A Callback executed when the player
// interacts with a room trigger.
//
void Game_PlayerIsInNewRoom(unsigned char dir)
{
    golden_freddy_active = false;

    // There's a chance for the killer to spawn if you're
    // coming in from the left.
    if (dir == 3) {
        int purple_guy = rand() % 100;

        // 6% chance, actual rate is undocumented.
        if (purple_guy >= 94) {
            Game_SpawnPurpleGuy();
        }
    }

    // Update the Marionette's path
    puppet_in_room = false;
    switch (room) {
        case 11:
            Game_UpdatePuppetPath(0, PUPPET_GO_UP);
            break;
        case 6:
            Game_UpdatePuppetPath(1, PUPPET_GO_UP);
            break;
        case 2:
            Game_UpdatePuppetPath(2, PUPPET_GO_RIGHT);
            break;
        case 3:
            Game_UpdatePuppetPath(3, PUPPET_GO_RIGHT);
            break;
        case 4:
            Game_UpdatePuppetPath(4, PUPPET_GO_DOWN);
            break;
        case 8:
            Game_UpdatePuppetPath(5, PUPPET_GO_RIGHT);
            break;
        default:
            break;
    }
    // To be honest I have NO idea how golden freddy works,
    // I don't think he's able to end the game, just kinda
    // shows up, sometimes he'll have eyes and if he does,
    // vanishes. If someone knows better, please let me know lol
    int golden_freddy = rand() & 100;
    // 20%
    if (golden_freddy >= 80) {
        golden_freddy_active = true;
        Game_SpawnGoldenFreddy();
    }
}

//
// Game_ScreenUpdate()
// Game Update loop for the Blue/Red Screen
// sequence.
//
void Game_ScreenUpdate(bool blue)
{
    if (!screen_init) {
        // Start playing the error sound.
        PlaySound(error);
        // How long are we going to be here?
        // (1-3 seconds).
        screen_time = (rand() % 3) + 1;
        screen_starttime = clock();
        screen_init = true;
    }

    // Should we generate a new set of rectangles?
    double time_since_last = (double)(clock() - last_rectgen) / CLOCKS_PER_SEC;
    if (time_since_last >= 0.01) {
        for (int i = 0; i < 2; i++) {
            screen_rects[i].x = 0;
            screen_rects[i].width = GRAPHICS_WINDOWWIDTH;
            screen_rects[i].height = (rand() % (GRAPHICS_WINDOWHEIGHT/5)) + 8;
            screen_rects[i].y = (rand() % GRAPHICS_WINDOWHEIGHT) - screen_rects[i].height;
        }
        last_rectgen = clock();
    }

    // Is it blue or red?
    Color col;
    if (blue) {
        col = BLUE;
    } else {
        col = RED;
    }

    // Draw the rectangles.
    Graphics_DrawRect(screen_rects[0].x, screen_rects[0].y, screen_rects[0].width, 
                    screen_rects[0].height, col);
    Graphics_DrawRect(screen_rects[1].x, screen_rects[1].y, screen_rects[1].width, 
                    screen_rects[1].height, col);

    // YOU CAN'T.
    if (blue)
        draw_you_cant = true;

    // Is it time to bail?
    double time_since_start = (double)(clock() - screen_starttime) / CLOCKS_PER_SEC;
    if (time_since_start >= ((double)screen_time/10)) {
        StopSound(error);
        draw_you_cant = false;

        if (!close_after_state) {
            // Always start with a 3 second delay on
            // SAVETHEM speech.
            last_savethem = clock();
            last_ticktime = clock();
            game_state = GAMESTATE_GAMEPLAY;
            PlaySound(loop);
        } else {

#ifndef PLATFORM_WEB

            CloseWindow();

#else

            // Quick hack to re-start the game on WEB
            Game_Init();

#endif // PLATFORM_WEB

        }

        screen_init = false;
    }
}

//
// Game_GameplayUpdate()
// Game Update loop for the Gameplay
// sequence.
//
void Game_GameplayUpdate(void)
{
    // Play the "SAVETHEM" audio.
    Game_SAVETHEM();

    // Play the looping BGS
    Game_LoopSound();

    // Draw the Room we're currently in.
    Graphics_DrawRoom(room);

    // Let the Killer march towards you..
    if (purple_guy_active)
        Game_PurpleGuyUpdate();

    // Puppet needs to lead you to the gift
    if (marionette_active)
        Game_PuppetUpdate();

    // Golden Freddy.. exists..
    if (golden_freddy_active)
        Game_GoldenFreddyUpdate();

    Game_MangleUpdate();

    // Handle updating the player
    Player_Update();

    // Tick down, see if we should auto quit
    Game_TickAway();
}

//
// Game_Update()
// Runs every frame to update gameplay.
//
void Game_Update(void)
{
    switch(game_state) {
        case GAMESTATE_REDSCREEN: Game_ScreenUpdate(false); break;
        case GAMESTATE_GAMEPLAY: Game_GameplayUpdate(); break;
        case GAMESTATE_BLUESCREEN: Game_ScreenUpdate(true); break;
    }
}

void Game_Init(void)
{
    // Start "SAVETHEM" speech at "S"
    letter = 0;
    close_after_state = false;
    game_state = GAMESTATE_REDSCREEN;
    purple_guy_active = false;

    ai_mangle.sprite = mangle_0;

    // Marionette should be around
    Game_SpawnPuppet();


    // 4 Potential spawn points: Party Room 3, 
    // Party Room 4, Party Room 1, and the Office.
    // The Wiki lists for the potential to spawn
    // in Parts/Service as well, although I had
    // never been able to reproduce this. Also,
    // I haven't been able to produce a scenario
    // where you spawn in Party Room 2, this may
    // have been intentional because there is
    // a Corpse, and Scott did not want the player
    // to end instantly?
    int random = rand() % 4;

    switch(random) {
        // Party Room 3
        case 0: 
            room = 5;
            player.origin.x = 185;
            player.origin.y = 120;
            break;
        // Party Room 4
        case 1:
            room = 7;
            player.origin.x = 135;
            player.origin.y = 120;
            break;
        // Office
        case 2:
            room = 14;
            player.origin.x = 140;
            player.origin.y = 60;
            break;
        // Party Room 1
        case 3:
            room = 12;
            player.origin.x = 135;
            player.origin.y = 120;
            break;
        default: 
            break;
    }

    first_room = true;
}