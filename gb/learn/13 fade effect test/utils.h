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
uint8_t isMovable(uint8_t x, uint8_t y);
uint8_t getMovables(Pointu8* loc, Pointu8* oldloc, uint8_t scrolled);

#endif // UTILS_MACROS
