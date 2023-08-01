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
// audio.c -- Main audio handling
//

#include <stddef.h>
#include <raylib.h>

#include "audio.h"

Sound speak_s;
Sound speak_a;
Sound speak_v;
Sound speak_e;
Sound speak_t_;
Sound speak_h;
Sound speak_m;
Sound error;
Sound loop;

//
// Audio_LoadSound(snd, path)
// Wrapper for loading sounds and
// returning based on status.
//
int Audio_LoadSound(Sound* snd, const char* path)
{
    *snd = LoadSound(path);

    if (snd->frameCount <= 0)
        return -1;

    return 0;
}

//
// Audio_Init()
// Allocates all of the used audio.
//
int Audio_Init(void)
{
    int ret = 0;
    ret += Audio_LoadSound(&speak_s, "assets/audio/speak/S.wav");
    ret += Audio_LoadSound(&speak_a, "assets/audio/speak/A.wav");
    ret += Audio_LoadSound(&speak_v, "assets/audio/speak/V.wav");
    ret += Audio_LoadSound(&speak_e, "assets/audio/speak/E.wav");
    ret += Audio_LoadSound(&speak_t_, "assets/audio/speak/T.wav");
    ret += Audio_LoadSound(&speak_h, "assets/audio/speak/H.wav");
    ret += Audio_LoadSound(&speak_m, "assets/audio/speak/M.wav");
    ret += Audio_LoadSound(&error, "assets/audio/error.wav");
    ret += Audio_LoadSound(&loop, "assets/audio/loop.wav");

    if (ret < 0)
        return -1;

    return 0;
}