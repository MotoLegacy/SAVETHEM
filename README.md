# SAVETHEM: Five Nights at Freddy's 2 Minigame re-implementation

## About

This repository holds the source and assets for "SAVETHEM" - a recreation of the popular minigame from [Five Nights at Freddy's 2](https://store.steampowered.com/app/332800/Five_Nights_at_Freddys_2/), implemented using [raylib](https://github.com/raysan5/raylib) and written in C. The game was made to be as accurate as possible to the best of my ability without decompilation of the Multimedia Fusion project, with a few non-obtrusive enhancements:

* To better replicate the early 80s game aesthetic, sprites were re-done to fit and display on a 320x240 resolution canvas. Some sprites also gained additional coloration.
* The "YOU CAN'T" text was made larger and more visible, using a less intrusive bitmap font.
* General memory efficiencies: Sprites were combined into tilemaps and reduced, audio WAVs had their rate lowered.

Work and analysis on this project also allowed me to make discoveries concerning potential inaccuracies on the [Fandom](https://freddy-fazbears-pizza.fandom.com/wiki/SAVETHEM) entry for this minigame. These are as follows:

* "Purple Guy"/The Killer does not actually restrict the ability to move between rooms, this means if caught quickly enough you can [evade him](https://youtu.be/iulxZPLfyX4?t=2). Additionally, The Killer can spawn in any room that is accessed from the left side, always approaching from the right. You can not "dodge" him by moving around him.
* [Touching the bounding box for Mangle will not cause the game to end](https://youtu.be/lEvNYyphIJ0?t=141).
* The Puppet's destination is fixed, always moving to the Prize Counter, and [reaching it does not end the game](https://youtu.be/-DoZl_25WOM?t=58).
* You can not spawn in the Parts & Service room (needs citation).
* [Golden Freddy encounters do not cause the game to end](https://youtu.be/-DoZl_25WOM?t=63).

## In Action

https://youtu.be/4dNPxZwRUsQ
(Note that the Puppet pathing bug present here was fixed for 1.0.0).

## License

The source code for SAVETHEM is licensed under the [MIT License](LICENSE). Game textures and audio do not have a license applied, as the textures were based on the ones used in Five Nights at Freddy's 2, making them property of Scott Cawthon and the owners of the soundpacks used to source them.

## Building

Building is done via a Linux environment, utilizing cross-compilation. The following cross-platforms are supported by the `Makefile`:
```
WEB       : WASM/HTML5 (emscripten)
WINDOWS32 : 32-bit Windows (mingw)
WINDOWS64 : 64-bit Windows (mingw)
LINUX32   : 32-bit Linux (native, -m32)
```

You can specify a platform target with `make PLATFORM=X`. Ensure you have [raylib](https://github.com/raysan5/raylib) installed before attempting to build.

Prebuilt binaries are available for supported systems on the [releases page](https://github.com/MotoLegacy/SAVETHEM/releases).

## Support Me!

This was a quick and dirty project made to draw attention to [OpenFNaF](https://github.com/MotoLegacy/OpenFNaF), an attempt at re-implementing the first few Five Nights at Freddy's titles using a memory efficient, port-able engine. The project had been abandoned due to time constraints, and never got very far. If you would like to support me and see development start again, consider supporting me on [Patreon](https://www.patreon.com/CypressImplex) or donating to my [PayPal](https://paypal.me/ibdev). I also have a lot of other projects I work on that may interest you.