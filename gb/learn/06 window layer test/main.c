#include <gb/gb.h>
#include <gbdk/font.h>
#include <stdio.h>
#include "sprite.h"
#include "map.h"

unsigned char windowmap[] = {
    0x12, 0x0F, 0x16, 0x16, 0x19
};

void init(){
    // init sound:   // order of initiating regs matter
    NR52_REG = 0x80; // 1000 0000 turns on sound
    NR50_REG = 0x77; // 0111 0111 sets L and R channels to 0x77 (max)
    NR51_REG = 0xFF; // 1111 1111 select both channels
    // init font (min): 36 tiles
    font_init();
    font_set(font_load(font_min));
    // init background layer:
    set_bkg_data(37, 12, tiles);
    set_bkg_tiles(0,0,40,18,map);
    SHOW_BKG;
    DISPLAY_ON;
    // init sprites layer:
    set_sprite_data(0, 2, tiles);
    set_sprite_tile(0, 0);
    move_sprite(0, 88, 78);
    SHOW_SPRITES;
    // init window layer
    set_win_tiles(0,0,5,1,windowmap);
    move_win(7, 136);
    SHOW_WIN;
}

void main(){
    init();
    uint8_t spriteIndex = 0;
    while(1){
        scroll_bkg(1,0);
        set_sprite_tile(0, spriteIndex);

        switch (joypad())
        {
        case J_LEFT:
            scroll_sprite(0, -5, 0);
            break;
        case J_RIGHT:
            scroll_sprite(0, 5, 0);
            break;
        case J_UP:
            scroll_sprite(0, 0, -5);
            break;
        case J_DOWN:
            scroll_sprite(0, 0, 5);
            break;
        case J_A:
            NR10_REG = 0x16; // 0001 0110: freq sweep: time 1, dir increase, shift 110 (6 decimal)
            NR11_REG = 0x40; // 0100 0000: dut cycle 01 (25%), wave len 000000 (longest).
            NR12_REG = 0x73; // 0111 0011: vol 7, sweep down, step=3
            NR13_REG = 0x00; // lsbs for channel 1
            NR14_REG = 0xC3; // 1100 0011: init, no conseq, freq = 011 [NR13] = 0x300
            break;
        case J_B:
            break;
        case J_SELECT:
            spriteIndex = spriteIndex ? 0 : 1;
            break;
        case J_START:
            break;
        default:
            break;
        }
        delay(200);
    }

}