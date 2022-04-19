#include <gb/gb.h>
#include <stdio.h>
#include "sprite.h"

// screen layer
// sprite layer
// background layer

void main(){
    uint8_t spriteIndex = 0;
    set_sprite_data(0, 2, RM);
    set_sprite_tile(0, 0);
    move_sprite(0, 88, 78);
    SHOW_SPRITES;

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
            /* code */
            break;
        case J_B:
            /* code */
            break;
        case J_SELECT:
            spriteIndex = spriteIndex ? 0 : 1;
            break;
        case J_START:
            /* code */
            break;
        default:
            break;
        }
        delay(200);
    }
}