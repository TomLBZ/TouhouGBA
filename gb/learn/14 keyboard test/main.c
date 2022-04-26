#include "visuals/splash.h"
#include "visuals/fonts.h"
#include "visuals/keyboardmap.h"
#include "visuals/bgtiles.h"
#include "visuals/bgmap.h"
#include "visuals/sprites.h"
#include "visuals/windowmap.h"
#include "visuals/palette.h"
#include "visuals/chardef.h"
#include "main.h"
#include "obj.h"
#include "sound.h"
#include "utils.h"
// in GB, use global vars and consts preferrably to local vars. use uint8 preferrably.
Pointu8 playerLoc = {.x = KEYBOARDMINX, .y = KEYBOARDMINY};
uint8_t playerSprite = 0;   // 0: RM; 1: MRS
uint8_t joypadState = 0;    // keys
uint8_t gravity = 1;        // gravity
uint8_t speed_full = 3;     // full high speed
uint8_t speedMode = 0;      // 0 high speed, 1 low speed
uint8_t isLooping = 1;      // 1 loops, 0 quits

void init(){
    // init sound:   // order of initiating regs matter
    NR52_REG = 0x80u; // 1000 0000 turns on sound
    NR50_REG = 0x77u; // 0111 0111 sets L and R channels to 0x77 (max)
    NR51_REG = 0xFFu; // 1111 1111 select both channels
    // splash screen
    blackOut();
    set_bkg_data(SPLASH_OFFSET, splashdataLen, splash_data);
    set_bkg_tiles(0, 0, splashmapWidth, splashmapHeight, splash_map);
    SHOW_BKG;
    DISPLAY_ON;
    fadeFromBlack();
    waitpad(J_A|J_B|J_SELECT|J_START);
    fadeToBlack();
    // keyboard
    inversePalette();
    set_bkg_data(FONTS_OFFSET, FONTS_LEN, fonts);
    set_bkg_tiles(0, 0, keyboardmapWidth, keyboardmapHeight, keyboardmap);
    set_sprite_data(SPRITES_OFFSET, SPRITES_LEN, sprites);
    set_sprite_tile(0, CURSOR_SPRITE);
    move_sprite(0, TILE2PX(playerLoc.x), TILE2PX(playerLoc.y));
    SHOW_SPRITES;
    fadeFromBlack();
    keypad(&playerLoc);
    fadeToWhite();
    inversePalette();
    // init background layer:
    set_bkg_data(BGTILES_OFFSET, BGTILES_LEN, bgtiles);
    set_bkg_tiles(0,0,bgmapWidth, bgmapHeight, bgmap);
    // init sprites layer:
    set_sprite_tile(0, playerSprite);
    move_sprite(0, playerLoc.x, playerLoc.y);
    // setup meta sprites5
    initGameObjects();
    moveGameObject(&Oodama, playerLoc.x, 8);
    fadeFromWhite();
    // init window layer
    set_win_tiles(0,0,windowmapWidth,windowmapHeight,windowmap);
    move_win(WINDOW_OFFSETX, WINDOW_OFFSETY);
    SHOW_WIN;
}

void main(){
    init();
    Pointu8 newLoc = {.x = 0, .y = 0};
    uint8_t bgScrolled = 0;
    int8_t bgScrollStep = 0;
    int8_t bgSpriteSync = 0;
    while(isLooping){
        set_sprite_tile(0, playerSprite);
        joypadState = joypad();
        // actions
        if (joypadState & J_A) playerSprite ? spark() : seal();
        if (joypadState & J_B) moveGameObject(&Oodama, playerLoc.x, 8);
        newLoc.x = playerLoc.x;
        newLoc.y = playerLoc.y;
        // update screen (relative) coordinates
        if (joypadState & J_LEFT) newLoc.x -= speed_full - speedMode;
        if (joypadState & J_RIGHT) newLoc.x += speed_full - speedMode;
        if (joypadState & J_UP) newLoc.y -= speed_full - speedMode + playerSprite * speed_full; // MRS jumps, RM flies
        if (joypadState & J_DOWN) newLoc.y += speed_full - speedMode;
        if (joypadState & J_SELECT) playerSprite = playerSprite ? 0 : 1;
        if (joypadState & J_START) speedMode = 1; else speedMode = 0;
        newLoc.y += playerSprite * gravity; // RM flies, MRS falls
        trimToBorder(&newLoc);
        if (getMovables(&newLoc, &playerLoc, bgScrolled, BGTILES_OFFSET)){
            // tries to move sprite to center first
            if (playerLoc.x < PCENT_X){ // player is in the left half
                if (newLoc.x > playerLoc.x) playerLoc.x += PCENT_X > newLoc.x ? newLoc.x - playerLoc.x : PCENT_X - playerLoc.x;
                else playerLoc.x = newLoc.x; // update player position
            } else if (playerLoc.x > PCENT_X){ // player in the right half
                if (newLoc.x < playerLoc.x) playerLoc.x -= PCENT_X < newLoc.x ? playerLoc.x - newLoc.x : playerLoc.x - PCENT_X;
                else playerLoc.x = newLoc.x; // update player position
            } // no action if player is on the center line
            bgScrollStep = newLoc.x - playerLoc.x;
            bgSpriteSync = 0;
            if(bgScrollStep < 0){ // player moves left
                if (bgScrolled >= (uint8_t)(-bgScrollStep)){ //scroll bg
                    scroll_bkg(bgScrollStep, 0);
                    bgScrolled -= (uint8_t)(-bgScrollStep);
                    bgSpriteSync = bgScrollStep;
                }else playerLoc.x = newLoc.x;// update player location
            } else if (bgScrollStep > 0) { // player moves right
                if (bgScrolled + bgScrollStep <= MAX_SCROLL){ // scroll bg
                    scroll_bkg(bgScrollStep, 0);
                    bgScrolled += (uint8_t)bgScrollStep;
                    bgSpriteSync = bgScrollStep;
                }else playerLoc.x = newLoc.x;// update player location
            } // no action if not scrolling
            playerLoc.y = newLoc.y;
            move_sprite(0, playerLoc.x, playerLoc.y);
        } 
        if (!Oodama.distfunc((void*)&Oodama, playerLoc.x + PLAYER_CDX, playerLoc.y + PLAYER_CDY)) biu(); // dist = 0 means collision
        if(Oodama.tly < SCR_MAXY) moveGameObject(&Oodama, Oodama.tlx + Oodama.cdx - bgSpriteSync, Oodama.tly + Oodama.cdy + gravity);
        nonBlockingDelay(3);
    }
}