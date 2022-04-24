#include "sprite.h"
#include "map.h"
#include "main.h"
// in GB, use global vars and consts preferrably to local vars. use uint8 preferrably.
unsigned char windowmap[] = {
    CHAR_T, CHAR_O, CHAR_U, CHAR_H, CHAR_O, CHAR_U, CHAR__, CHAR_G, CHAR_B, CHAR_A
};

Sprite16 bigBullet; // test meta sprite

uint8_t playerLoc[2] = {88,78};
uint8_t playerSprite = 0;   // 0: RM; 1: \MRS/
uint8_t joypadState = 0;    // keys
uint8_t gravity = 1;        // gravity
uint8_t speed_full = 3;     // full high speed
uint8_t speedMode = 0;      // 0 high speed, 1 low speed
uint8_t ground = 104;       // ground level for now

void setupBigBullet(){
    bigBullet.x = 0;
    bigBullet.y = 0;
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    set_sprite_tile(4, 4);
    set_sprite_tile(5, 5);
    bigBullet.sprIds[0] = 2;
    bigBullet.sprIds[1] = 3;
    bigBullet.sprIds[2] = 4;
    bigBullet.sprIds[3] = 5;
}

void moveSprite16(Sprite16* spr, uint8_t x, uint8_t y){
    spr->x = x;
    spr->y = y;
    move_sprite(spr->sprIds[0],x,y);
    move_sprite(spr->sprIds[1],x+8,y);
    move_sprite(spr->sprIds[2],x,y+8);
    move_sprite(spr->sprIds[3],x+8,y+8);
}

void init(){
    // init sound:   // order of initiating regs matter
    NR52_REG = 0x80; // 1000 0000 turns on sound
    NR50_REG = 0x77; // 0111 0111 sets L and R channels to 0x77 (max)
    NR51_REG = 0xFF; // 1111 1111 select both channels
    // init font (min): 36 tiles
    font_init();
    font_set(font_load(font_min));
    // init background layer:
    set_bkg_data(37, 16, tiles);
    set_bkg_tiles(0,0,40,18,map);
    SHOW_BKG;
    DISPLAY_ON;
    // init sprites layer:
    set_sprite_data(0, 6, tiles);
    set_sprite_tile(0, 0);
    move_sprite(0, playerLoc[0], playerLoc[1]);
    SHOW_SPRITES;
    // init window layer
    set_win_tiles(0,0,10,1,windowmap);
    move_win(7, 136);
    SHOW_WIN;
    // setup meta sprites
    setupBigBullet();
    moveSprite16(&bigBullet, playerLoc[0] - 8, 0);
}

void nonBlockingDelay(uint8_t frames){
    for (uint8_t i = 0; i < frames; i++) wait_vbl_done();
}

void spark(){
    NR41_REG = 0x1F; // 0001 1111 sound len max 11111
    NR42_REG = 0xF1; // 1111 0001 vol start full 1111, fade down by 1 sweep
    NR43_REG = 0x30; // 0011 0000: shift freq 3, setp 0, div ratio 0
    NR44_REG = 0xC0; // 1100 0000: start sound, not continuous
}

void seal(){
    NR10_REG = 0x16; // 0001 0110: freq sweep: time 1, dir increase, shift 110 (6 decimal)
    NR11_REG = 0x40; // 0100 0000: dut cycle 01 (25%), wave len 000000 (longest).
    NR12_REG = 0x73; // 0111 0011: vol 7, sweep down, step=3
    NR13_REG = 0x00; // lsbs for channel 1
    NR14_REG = 0xC3; // 1100 0011: init, no conseq, freq = 011 [NR13] = 0x300
}

void main(){
    init();
    while(1){
        scroll_bkg(1,0);
        set_sprite_tile(0, playerSprite);
        joypadState = joypad();
        // action switch are mutually exclusive
        switch (joypad())
        {
        case J_A: playerSprite ? spark() : seal();
            break;
        case J_B: moveSprite16(&bigBullet, playerLoc[0]-8, 0);
            break;
        default:
            break;
        }
        // moving states are bit flags, so swich cannot improve efficiency... shame
        if (joypadState & J_LEFT) playerLoc[0] -= speed_full - speedMode;
        if (joypadState & J_RIGHT) playerLoc[0] += speed_full - speedMode;
        if (joypadState & J_UP) playerLoc[1] -= speed_full - speedMode + playerSprite * speed_full; // MRS jumps, RM flies
        if (joypadState & J_DOWN) playerLoc[1] += speed_full - speedMode;
        if (joypadState & J_SELECT) playerSprite = playerSprite ? 0 : 1;
        if (joypadState & J_START) speedMode = 1; else speedMode = 0;
        if(playerLoc[1] < ground) playerLoc[1] += playerSprite * gravity; // RM flies, MRS falls
        move_sprite(0, playerLoc[0], playerLoc[1]);
        if(bigBullet.y < 160) moveSprite16(&bigBullet, --bigBullet.x, ++bigBullet.y);
        nonBlockingDelay(5);
    }

}