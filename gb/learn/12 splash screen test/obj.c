#include "obj.h"
#include "vmath.h"

uint8_t OodamaIds[] = {0x0Au, 0x0Au, 0x0Au, 0x0Au};
uint8_t OodamaOAMIds[] = {1u, 2u, 3u, 4u};
uint8_t OodamaOffsets[] = {OFFDIRX(0),OFFDIRX(1),OFFDIRY(1),OFFDIRY(1)|OFFDIRX(1)};
uint8_t OodamaProps[] = {0u, HFLIP, VFLIP, VFLIP | HFLIP};
GameObject Oodama = {
    .tlx = 0, .tly = 0, .cdx = 8, .cdy = 8, .distfunc = sdfOodama, .len = 4, 
    .oamIds = OodamaOAMIds, .sprIds = OodamaIds, .offsets = OodamaOffsets, .props = OodamaProps};

void initGameObjects(){
    // Oodama
    setGameObjectTiles(&Oodama);
}

void setGameObjectTiles(GameObject* obj){
    for (uint8_t i = 0; i < obj->len; i++){
        set_sprite_tile(obj->oamIds[i], obj->sprIds[i]);
        set_sprite_prop(obj->oamIds[i], obj->props[i]);
    }
}

void moveGameObject(GameObject* obj, uint8_t x, uint8_t y){
    obj->tlx = x - obj->cdx; // might overflow
    obj->tly = y - obj->cdy; // might overflow
    uint8_t offx, offy;
    for(uint8_t i = 0; i < obj->len; i++){
        offx = (obj->offsets[i] & 0x0F) << 3u;
        offy = (obj->offsets[i] & 0xF0) >> 1u;
        move_sprite(obj->oamIds[i], obj->tlx + offx, obj->tly + offy);
    }
}

int8_t sdfOodama(void* obj, uint8_t x, uint8_t y){
    GameObject* gptr = (GameObject*)obj;
    uint8_t cx = gptr->tlx + gptr->cdx;    // center x, depends on the obj
    uint8_t cy = gptr->tly + gptr->cdy;    // center y, depends on the obj
    uint8_t px = x > cx ? x - cx : cx - x; // equivalent input x
    uint8_t py = y > cy ? y - cy : cy - y; // equivalent input y
    uint8_t dx = px > 4 ? px - 4 : 0; // using x halfwidth = 8
    uint8_t dy = py > 4 ? py - 4 : 0; // using y halfwidth = 8
    return vlen(dx, dy); // dist from diagnal, considering sides too
}