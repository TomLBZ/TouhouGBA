#include <gb/gb.h>
#include <gbdk/font.h>
#include <stdio.h>

#if !defined(MAIN_MACROS)
#define MAIN_MACROS

#define TILESIZE    16u

typedef struct sprite16_t{
    uint8_t x;
    uint8_t y;
    uint8_t sprIds[4];
} Sprite16;

#endif // MAIN_MACROS


