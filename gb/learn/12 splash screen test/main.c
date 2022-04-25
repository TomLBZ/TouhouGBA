#include "visuals/bgtiles.h"
#include "visuals/sprites.h"
#include "visuals/fonts.h"
#include "visuals/map.h"
#include "visuals/splash.h"
#include "main.h"
#include "obj.h"
#include "sound.h"
// in GB, use global vars and consts preferrably to local vars. use uint8 preferrably.
const unsigned char windowmap[] = {T_TOU, T_HOU, T_KOU, T_MA, T_KYOU, T_EMPTY,
T_T, T_O, T_U, T_H, T_O, T_U, T_EMPTY, T_G, T_B, T_A};

uint8_t playerLoc[2] = {PCENT_X, PCENT_Y};
uint8_t playerSprite = 0;   // 0: RM; 1: \MRS/
uint8_t joypadState = 0;    // keys
uint8_t gravity = 1;        // gravity
uint8_t speed_full = 3;     // full high speed
uint8_t speedMode = 0;      // 0 high speed, 1 low speed

void init(){
    // init sound:   // order of initiating regs matter
    NR52_REG = 0x80u; // 1000 0000 turns on sound
    NR50_REG = 0x77u; // 0111 0111 sets L and R channels to 0x77 (max)
    NR51_REG = 0xFFu; // 1111 1111 select both channels
    // splash screen
    set_bkg_data(0, 244u, splash_data);
    set_bkg_tiles(0, 0, 20, 18, splash_map);
    SHOW_BKG;
    DISPLAY_ON;
    waitpad(J_A|J_B|J_SELECT|J_START);
    // init background layer:
    set_bkg_data(0, 14, bgtiles);
    set_bkg_data(14, 42, fonts);
    init_bkg(T_EMPTY); // T_EMPTY is empty char
    set_bkg_tiles(0,0,32,18,map);
    // init sprites layer:
    set_sprite_data(0, 17, sprites);
    set_sprite_tile(0, playerSprite);
    move_sprite(0, playerLoc[0], playerLoc[1]);
    // setup meta sprites5
    initGameObjects();
    moveGameObject(&Oodama, playerLoc[0], 8);
    SHOW_SPRITES;
    // init window layer
    init_win(T_EMPTY); // empty char
    set_win_tiles(0,0,16,1,windowmap);
    move_win(7, 136);
    SHOW_WIN;
}

void nonBlockingDelay(uint8_t frames){
    for (uint8_t i = 0; i < frames; i++) wait_vbl_done();
}

uint8_t isMovable(uint8_t x, uint8_t y){
    uint8_t tilex = ((uint8_t)(x - SCR_TLX_PX) >> 3u); // row starts with 0
    uint8_t tiley = ((uint8_t)(y - SCR_TLY_PX) >> 3u); // row has offset, top rows are full
    uint16_t tileIndex = tiley * MAP_W_TILES + tilex;
    return (map[tileIndex] == T_EMPTY);
}

uint8_t getMovables(uint8_t loc[2], uint8_t oldloc[2], uint8_t scrolled){
    uint8_t newx = loc[0] + PLAYER_CDX + scrolled;
    uint8_t newy = loc[1] + PLAYER_CDY;
    uint8_t oldx = oldloc[0] + PLAYER_CDX + scrolled;
    uint8_t oldy = oldloc[1] + PLAYER_CDY;
    if(isMovable(newx, newy)) return 1; // completely movable, return true
    if(isMovable(newx, oldy)){ // only x is movable
        loc[1] = oldloc[1];         // suppress y movement, return true
        return 1;
    }
    if(isMovable(oldx, newy)){ // only y is movable
        loc[0] = oldloc[0];         // suppress x movement, return true
        return 1;
    }
    return 0;                       // completely not movable, return false
}

uint8_t trimXToBorder(uint8_t x){
    if (x < SCR_TLX_PX - PLAYER_CDX) return SCR_TLX_PX - PLAYER_CDX;
    if (x > SCR_MAXX - PLAYER_CDX) return SCR_MAXX - PLAYER_CDX;
    return x;
}

uint8_t trimYToBorder(uint8_t y){
    if (y < SCR_TLY_PX - PLAYER_CDY) return SCR_TLY_PX - PLAYER_CDY;
    if (y > SCR_MAXY - PLAYER_CDY) return SCR_MAXY - PLAYER_CDY;
    return y;
}

void main(){
    init();
    uint8_t newLoc[] = {0,0};
    uint8_t bgScrolled = 0;
    int8_t bgScrollStep = 0;
    int8_t bgSpriteSync = 0;
    while(1){
        set_sprite_tile(0, playerSprite);
        joypadState = joypad();
        // actions
        if (joypadState & J_A) playerSprite ? spark() : seal();
        if (joypadState & J_B) moveGameObject(&Oodama, playerLoc[0], 8);
        newLoc[0] = playerLoc[0];
        newLoc[1] = playerLoc[1];
        // update screen (relative) coordinates
        if (joypadState & J_LEFT) newLoc[0] -= speed_full - speedMode;
        if (joypadState & J_RIGHT) newLoc[0] += speed_full - speedMode;
        if (joypadState & J_UP) newLoc[1] -= speed_full - speedMode + playerSprite * speed_full; // MRS jumps, RM flies
        if (joypadState & J_DOWN) newLoc[1] += speed_full - speedMode;
        if (joypadState & J_SELECT) playerSprite = playerSprite ? 0 : 1;
        if (joypadState & J_START) speedMode = 1; else speedMode = 0;
        newLoc[1] += playerSprite * gravity; // RM flies, MRS falls
        newLoc[0] = trimXToBorder(newLoc[0]);
        newLoc[1] = trimYToBorder(newLoc[1]);
        if (getMovables(newLoc, playerLoc, bgScrolled)){
            // tries to move sprite to center first
            if (playerLoc[0] < PCENT_X){ // player is in the left half
                if (newLoc[0] > playerLoc[0]) playerLoc[0] += PCENT_X > newLoc[0] ? newLoc[0] - playerLoc[0] : PCENT_X - playerLoc[0];
                else playerLoc[0] = newLoc[0]; // update player position
            } else if (playerLoc[0] > PCENT_X){ // player in the right half
                if (newLoc[0] < playerLoc[0]) playerLoc[0] -= PCENT_X < newLoc[0] ? playerLoc[0] - newLoc[0] : playerLoc[0] - PCENT_X;
                else playerLoc[0] = newLoc[0]; // update player position
            } // no action if player is on the center line
            bgScrollStep = newLoc[0] - playerLoc[0];
            bgSpriteSync = 0;
            if(bgScrollStep < 0){ // player moves left
                if (bgScrolled >= (uint8_t)(-bgScrollStep)){ //scroll bg
                    scroll_bkg(bgScrollStep, 0);
                    bgScrolled -= (uint8_t)(-bgScrollStep);
                    bgSpriteSync = bgScrollStep;
                }else playerLoc[0] = newLoc[0];// update player location
            } else if (bgScrollStep > 0) { // player moves right
                if (bgScrolled + bgScrollStep <= MAX_SCROLL){ // scroll bg
                    scroll_bkg(bgScrollStep, 0);
                    bgScrolled += (uint8_t)bgScrollStep;
                    bgSpriteSync = bgScrollStep;
                }else playerLoc[0] = newLoc[0];// update player location
            } // no action if not scrolling
            playerLoc[1] = newLoc[1];
            move_sprite(0, playerLoc[0], playerLoc[1]);
        } 
        if (!Oodama.distfunc((void*)&Oodama, playerLoc[0] + PLAYER_CDX, playerLoc[1] + PLAYER_CDY)) biu(); // dist = 0 means collision
        if(Oodama.tly < SCR_MAXY) moveGameObject(&Oodama, Oodama.tlx + Oodama.cdx - bgSpriteSync, Oodama.tly + Oodama.cdy + 1);

        nonBlockingDelay(3);
    }
}