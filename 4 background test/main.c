#include <gb/gb.h>
#include "sprite.c"
#include "map.c"

void main(){
    //uint8_t spriteIndex = 0;
    set_bkg_data(0, 12, tiles);
    set_bkg_tiles(0,0,40,18,map);
    //set_sprite_data(0, 13, tiles);
    //set_sprite_tile(0, 0);
    //move_sprite(0, 88, 78);
    SHOW_BKG;
    //SHOW_SPRITES;
    DISPLAY_ON;

    while(1){
        scroll_bkg(1,0);
        delay(100);
    }

/*
    while(1){
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
*/
}