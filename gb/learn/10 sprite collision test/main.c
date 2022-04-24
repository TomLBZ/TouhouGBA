#include "bgtiles.h"
#include "sprites.h"
#include "fonts.h"
#include "map.h"
#include "main.h"
#include "obj.h"
// in GB, use global vars and consts preferrably to local vars. use uint8 preferrably.
const unsigned char windowmap[] = {0x33, 0x34, 0x35, 0x36, 0x37, 0x0E,
0x2C, 0x27, 0x2D, 0x20, 0x27, 0x2D, 0x0E, 0x1F, 0x1A, 0x19};

uint8_t playerLoc[2] = {24,56};
uint8_t playerSprite = 0;   // 0: RM; 1: \MRS/
uint8_t joypadState = 0;    // keys
uint8_t gravity = 1;        // gravity
uint8_t speed_full = 3;     // full high speed
uint8_t speedMode = 0;      // 0 high speed, 1 low speed
uint8_t ground = 128;       // ground level for now

void init(){
    // init sound:   // order of initiating regs matter
    NR52_REG = 0x80u; // 1000 0000 turns on sound
    NR50_REG = 0x77u; // 0111 0111 sets L and R channels to 0x77 (max)
    NR51_REG = 0xFFu; // 1111 1111 select both channels
    // init background layer:
    set_bkg_data(0, 14, bgtiles);
    set_bkg_data(14, 42, fonts);
    init_bkg(0x0E); // 0x0E is empty char
    set_bkg_tiles(0,0,32,18,map);
    SHOW_BKG;
    DISPLAY_ON;
    // init sprites layer:
    set_sprite_data(0, 28, sprites);
    set_sprite_tile(0, playerSprite);
    move_sprite(0, playerLoc[0], playerLoc[1]);
    // setup meta sprites5
    initGameObjects();
    moveGameObject(&Oodama, playerLoc[0], 8);
    SHOW_SPRITES;
    // init window layer
    init_win(0x0E); // empty char
    set_win_tiles(0,0,16,1,windowmap);
    move_win(7, 136);
    SHOW_WIN;
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

void biu(){
    NR10_REG = 0x1E; // 0001 1110: freq sweep: time 1, dir decrease, shift 110 (6 decimal)
    NR11_REG = 0x80; // 1000 0000: dut cycle 10 (50%), wave len 000000 (longest).
    NR12_REG = 0x72; // 0111 0010: vol 7, sweep down, step=2
    NR13_REG = 0x00; // lsbs for channel 1
    NR14_REG = 0xC7; // 1100 0111: init, no conseq, freq = 111 [NR13] = 0x700
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
        case J_B: moveGameObject(&Oodama, playerLoc[0], 8);
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
        if (playerLoc[1] < ground) playerLoc[1] += playerSprite * gravity; // RM flies, MRS falls
        if (Oodama.distfunc((void*)&Oodama, playerLoc[0] + 4u, playerLoc[1] + 4u))
            move_sprite(0, playerLoc[0], playerLoc[1]);
        else biu(); // dist = 0 means collision
        if(Oodama.tly < 160) moveGameObject(&Oodama, Oodama.tlx + Oodama.cdx - 1, Oodama.tly + Oodama.cdy + 1);
        nonBlockingDelay(3);
    }

}