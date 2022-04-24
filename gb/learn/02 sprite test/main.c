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
        spriteIndex = spriteIndex ? 0 : 1;
        set_sprite_tile(0, spriteIndex);
        delay(1000);
        scroll_sprite(0, 10, 0);
    }
}