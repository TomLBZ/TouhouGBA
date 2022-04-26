#include "utils.h"
#include "visuals/bgmap.h"
#include "visuals/chardef.h"

uint8_t userName[USERNAME_LEN];
uint8_t userNameIndex = 0;

void nonBlockingDelay(uint8_t frames){
    for (uint8_t i = 0; i < frames; i++) wait_vbl_done();
}

void trimToBorder(Pointu8* pt){
    uint8_t minx = SCR_TLX_PX - PLAYER_CDX;
    uint8_t maxx = SCR_MAXX - PLAYER_CDX;
    uint8_t miny = SCR_TLY_PX - PLAYER_CDY;
    uint8_t maxy = SCR_MAXY - PLAYER_CDY;
    if (pt->x < minx) pt->x = minx;
    else if (pt->x > maxx) pt->x = maxx;
    if (pt->y < miny) pt->y = miny;
    else if (pt->y > maxy) pt->y = maxy;
}

uint8_t isMovable(uint8_t x, uint8_t y, uint8_t bgTileOffset){
    uint8_t tilex = ((uint8_t)(x - SCR_TLX_PX) >> 3u); // row starts with 0
    uint8_t tiley = ((uint8_t)(y - SCR_TLY_PX) >> 3u); // row has offset, top rows are full
    uint16_t tileIndex = tiley * MAP_W_TILES + tilex;
    return (bgmap[tileIndex] == C__ + bgTileOffset);
}

uint8_t getMovables(Pointu8* loc, Pointu8* oldloc, uint8_t scrolled, uint8_t bgTileOffset){
    uint8_t newx = loc->x + PLAYER_CDX + scrolled;
    uint8_t newy = loc->y + PLAYER_CDY;
    uint8_t oldx = oldloc->x + PLAYER_CDX + scrolled;
    uint8_t oldy = oldloc->y + PLAYER_CDY;
    if(isMovable(newx, newy, bgTileOffset)) return 1; // completely movable, return true
    if(isMovable(newx, oldy, bgTileOffset)){ // only x is movable
        loc->y = oldloc->y;         // suppress y movement, return true
        return 1;
    }
    if(isMovable(oldx, newy, bgTileOffset)){ // only y is movable
        loc->x = oldloc->x;         // suppress x movement, return true
        return 1;
    }
    return 0;                       // completely not movable, return false
}

void updateUserName(uint8_t tilex, uint8_t tiley){
    if (tilex == KEYBOARDMAXX - 1 && tiley == KEYBOARDMAXY){
        if (userNameIndex > 0) userName[--userNameIndex] = C__;
    } else {
        if (userNameIndex == USERNAME_LEN) return;
        uint8_t preindex = KEYXY2PREINDEX(tilex - KEYBOARDMINX, tiley - KEYBOARDMINY);
        uint8_t index = preindex < KEY_LTTRMAX ? preindex + C_LTR_OFFSET: 
            preindex < KEY_NUMMAX ? preindex - KEY_LTTRMAX + C_NUM_OFFSET : 
            preindex < KEY_BRKMAX ? preindex - KEY_NUMMAX + C_BRK_OFFSET : 
            preindex - KEY_BRKMAX + C_ARR_OFFSET + 3u;
        userName[userNameIndex++] = index;
    }
    set_bkg_tiles(2u, 5u, USERNAME_LEN, 1, userName);
}

void keypad(Pointu8* loc){
    uint8_t joypadState = 0;
    while(1){
        joypadState = joypad();
        if(joypadState & J_UP) loc->y -= (uint8_t)(loc->y > KEYBOARDMINY);
        if(joypadState & J_DOWN) loc->y += (uint8_t)(loc->y < KEYBOARDMAXY);
        if(joypadState & J_LEFT) loc->x -= (uint8_t)(loc->x > KEYBOARDMINX);
        if(joypadState & J_RIGHT) loc->x += (uint8_t)(loc->x < KEYBOARDMAXX);
        if(joypadState & J_B){
            if (loc->x == KEYBOARDMAXX && loc->y == KEYBOARDMAXY) break;
            updateUserName(loc->x, loc->y);
        }
        move_sprite(0, TILE2PX(loc->x), TILE2PX(loc->y));
        while(joypad() == joypadState) wait_vbl_done();
    }
    loc->x = PCENT_X;
    loc->y = PCENT_Y;
}