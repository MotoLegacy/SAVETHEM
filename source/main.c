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
// main.c -- Initialization stuffs
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif // PLATFORM_WEB

#include "graphics.h"
#include "player.h"
#include "audio.h"
#include "game.h"

#define WINDOW_TITLE                "SAVETHEM"

int frame_counter;

//
// RunFrame()
// Now abstracted out of main(),
// for web support.
//
static void RunFrame(void)
{
    // Start of Draw
    BeginDrawing();
    ClearBackground(BLACK);

    // Game Update Loop
    Game_Update();

    // Draw some lines over the game.
    Graphics_DrawScanLines();

    // And some static, if permitted
    if (game_state == GAMESTATE_GAMEPLAY)
        Graphics_DrawStatic();

    if (draw_you_cant == true)
        Graphics_DrawYouCant();

    frame_counter++; 
        
    EndDrawing();
}

int main(void)
{
    // Initialization of the Raylib window
    int win_w = GRAPHICS_WINDOWWIDTH * GRAPHICS_SCALEFACTOR;
    int win_h = GRAPHICS_WINDOWHEIGHT * GRAPHICS_SCALEFACTOR;
    InitWindow(win_w, win_h, WINDOW_TITLE);

    InitAudioDevice();
    SetMasterVolume(0.80f);

#ifndef PLATFORM_WEB

    SetTargetFPS(60);

#endif // PLATFORM_WEB

    // Initalize our graphics and audio into memory
    int res;
    res = Graphics_Init();
    if (res == -1) {
        printf("\n\n[ERROR] %s: Could not initialize graphics.\n\n\n", 
        WINDOW_TITLE);
        goto exit;
    }

    res = Audio_Init();
    if (res == -1) {
        printf("\n\n[ERROR] %s: Could not initialize audio.\n\n\n", 
        WINDOW_TITLE);
        goto exit;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Game start-up function
    Game_Init();
    Player_Init();

#ifdef PLATFORM_WEB

    emscripten_set_main_loop(RunFrame, 60, 1);

#else

    // Run until we return that the window can close
    // (or the exit window event, ha).
    while (!WindowShouldClose()) {
        RunFrame();
    }

#endif // PLATFORM_WEB

exit:
    // Unload assets
    UnloadSound(speak_s);
    UnloadSound(speak_a);
    UnloadSound(speak_v);
    UnloadSound(speak_e);
    UnloadSound(speak_t_);
    UnloadSound(speak_h);
    UnloadSound(speak_m);
    UnloadSound(error);
    UnloadSound(loop);
    UnloadTexture(sheet_0);
    UnloadTexture(sheet_1);
    UnloadTexture(_static);

    // you can't.
    CloseAudioDevice();
    CloseWindow();
    return 0;
}