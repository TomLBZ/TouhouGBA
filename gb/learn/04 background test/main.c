#include <gb/gb.h>
#include "sprite.h"
#include "map.h"

void main(){
    set_bkg_data(0, 12, tiles);
    set_bkg_tiles(0,0,40,18,map);
    SHOW_BKG;
    DISPLAY_ON;

    while(1){
        scroll_bkg(1,0);
        delay(100);
    }
}