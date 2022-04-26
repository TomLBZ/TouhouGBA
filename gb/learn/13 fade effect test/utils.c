#include "utils.h"
#include "visuals/map.h"
#include "visuals/windowmap.h"

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

uint8_t isMovable(uint8_t x, uint8_t y){
    uint8_t tilex = ((uint8_t)(x - SCR_TLX_PX) >> 3u); // row starts with 0
    uint8_t tiley = ((uint8_t)(y - SCR_TLY_PX) >> 3u); // row has offset, top rows are full
    uint16_t tileIndex = tiley * MAP_W_TILES + tilex;
    return (map[tileIndex] == T_EMPTY);
}

uint8_t getMovables(Pointu8* loc, Pointu8* oldloc, uint8_t scrolled){
    uint8_t newx = loc->x + PLAYER_CDX + scrolled;
    uint8_t newy = loc->y + PLAYER_CDY;
    uint8_t oldx = oldloc->x + PLAYER_CDX + scrolled;
    uint8_t oldy = oldloc->y + PLAYER_CDY;
    if(isMovable(newx, newy)) return 1; // completely movable, return true
    if(isMovable(newx, oldy)){ // only x is movable
        loc->y = oldloc->y;         // suppress y movement, return true
        return 1;
    }
    if(isMovable(oldx, newy)){ // only y is movable
        loc->x = oldloc->x;         // suppress x movement, return true
        return 1;
    }
    return 0;                       // completely not movable, return false
}