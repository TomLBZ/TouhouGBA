#if !defined(UTILS_MACROS)
#define UTILS_MACROS

#include <gb/gb.h>

#define MAP_W_TILES 32u
#define SCR_W_TILES 20u
#define SCR_H_TILES 18u
#define SCR_W_PX    (SCR_W_TILES << 3u)
#define SCR_H_PX    (SCR_H_TILES << 3u)
#define SCR_TLX_PX  8u
#define SCR_TLY_PX  16u
#define SCR_MAXX    (SCR_W_PX + SCR_TLX_PX)
#define SCR_MAXY    (SCR_H_PX + SCR_TLY_PX)
#define MAX_SCROLL  ((MAP_W_TILES - SCR_W_TILES) << 3u)
#define PLAYER_CDX  4u
#define PLAYER_CDY  4u
#define CENT_X      (SCR_TLX_PX + (SCR_W_TILES << 2u))
#define CENT_Y      (SCR_TLY_PX + (SCR_H_TILES << 2u))
#define PCENT_X     (CENT_X - PLAYER_CDX)
#define PCENT_Y     (CENT_Y - PLAYER_CDY)

typedef struct uint8_t_point_t{
    uint8_t x;
    uint8_t y;
} Pointu8;

void nonBlockingDelay(uint8_t frames);
void trimToBorder(Pointu8* pt);
uint8_t isMovable(uint8_t x, uint8_t y, uint8_t bgTileOffset);
uint8_t getMovables(Pointu8* loc, Pointu8* oldloc, uint8_t scrolled, uint8_t bgTileOffset);

#define USERNAME_LEN    16u
#define CURSOR_SPRITE   17u
#define TILE2PX(x)      ((uint8_t)((x) << 3u))
#define KEYXY2PREINDEX(x,y) ((x) + ((y) << 3u))
#define KEYBOARDMINX    7u
#define KEYBOARDMAXX    14u
#define KEYBOARDMINY    13u
#define KEYBOARDMAXY    17u
#define KEY_LTTRMAX     26u
#define KEY_NUMMAX      36u
#define KEY_BRKMAX      38u

extern uint8_t userName[USERNAME_LEN];

void updateUserName(uint8_t tilex, uint8_t tiley);
void keypad(Pointu8* loc);
#endif // UTILS_MACROS
